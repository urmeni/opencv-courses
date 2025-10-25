//processing.cpp

#include <processing.h>
#include <iostream>
#include <random>
#include "image_utils.h"
#include <math.h>

//remapping
void wave(const cv::Mat &image, cv::Mat &result) {
    cv::Mat srcX(image.rows, image.cols, CV_32F);
    cv::Mat srcY(image.rows,image.cols, CV_32F);

    for (int i=0; i<image.rows; i++) {
        for (int j=0; j<image.cols; j++) {

            srcX.at<float>(i,j) = j;
            srcY.at<float>(i,j) = i + 3*sin(j/6.0);

            //srcX.at<float>(i,j) = image.cols-j-i-1;
            //srcY.at<float>(i,j) = i;


        }
    }

    cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);
}


// fliping image
void flipImage(cv::Mat& image, cv::Mat& flip_image, int code) {
    if (image.empty()) {
        std::cerr << "Empty images was given \n";
        return;
    }

    cv::flip(image, flip_image, code);
}

// draws salt pepper
void salt(cv::Mat image, int n) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> randomRow(0, image.rows - 1);
    std::uniform_int_distribution<int> randomCol(0, image.cols - 1);

    int i,j;
    for (int k=0; k<n; k++) { // assigns randomly 255 value
        i = randomCol(generator);
        j = randomRow(generator);

        if(image.type() == CV_8U) {
            image.at<uchar>(j,i) = 255;

        } else if (image.type() == CV_8UC3) {
            image.at<cv::Vec3b>(j,i)[0] = 255;
            image.at<cv::Vec3b>(j,i)[1] = 255;
            image.at<cv::Vec3b>(j,i)[2] = 255;

            // or
            // image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
        }
    }
}

// Color Reduction with IO handling
void colorReduceIO(
    const cv::Mat &image, // means the image won't be modified
    cv::Mat &result,
    int div) {
    int nl = image.rows;
    int nc = image.cols * image.channels();

    // reallocation
    result.create(image.rows, image.cols, image.type());

    // scanning
    for (int j = 0; j < nl; j++) {
        const unsigned char *data_in = image.ptr<uchar>(j);
        uchar* data_out = result.ptr<uchar>(j);

        for (int i = 0; i < nc; i++) {
            data_out[i] = data_in[i] / div*div + div/2;
        }
    }
}

// Color Reduction Bitwise
void colorReduceB(cv::Mat image, int div) {
    int nl = image.rows;
    int nc = image.cols * image.channels();

    if (image.isContinuous()) {
        nc = nc*nl;
        nl = 1;
    }

    int n = static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);

    uchar mask = 0xFF<<n;
    uchar div2 = div >> 1;

    for (int j = 0; j < nl; j++) {
        uchar* data = image.ptr<uchar>(j);
        for (int i = 0; i < nc; i++) {
            *data &= mask;
            *data++ += div2;
        }
    }
}

// Color Reduction using Iterator Objects
void colorReduceIt(cv::Mat image, int div) {
    int n = static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);
    uchar mask = 0xFF<<n;
    uchar div2 = div >> 1;

    cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend = image.end<cv::Vec3b>();

    for ( ; it != itend; ++it) {
        (*it)[0] &= mask;
        (*it)[0] += div2;
        (*it)[1] &= mask;
        (*it)[1] += div2;
        (*it)[2] &= mask;
        (*it)[2] += div2;

    }
}

// Sharpenning
void sharpen(const cv::Mat &image, cv::Mat &result) {
    result.create(image.size(), image.type());
    int nchannels = image.channels();

    for (int j = 1; j < image.rows-1; j++) {
        const uchar* previous = image.ptr<const uchar>(j-1);
        const uchar* current = image.ptr<const uchar>(j);
        const uchar* next = image.ptr<const uchar>(j+1);
        uchar* output = result.ptr<uchar>(j);

        for (int i = nchannels; i < (image.cols-1)*nchannels; i++) {
            *output++= cv::saturate_cast<uchar>(
                 5*current[i]-current[i-nchannels]
                 -current[i+nchannels] - previous[i]-next[i]);
        }
    }

    result.row(0).setTo(cv::Scalar(0));
    result.row(result.rows-1).setTo(cv::Scalar(0));
    result.col(0).setTo(cv::Scalar(0));
    result.col(result.cols-1).setTo(cv::Scalar(0));
}

// Sharpen filtering
void sharpen2D(const cv::Mat &image, cv::Mat &result) {
    cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));

    kernel.at<float>(1,1) = 5.0;
    kernel.at<float>(0,1) = -1.0;
    kernel.at<float>(2,1) = -1.0;
    kernel.at<float>(1,0) = -1.0;
    kernel.at<float>(1,2) = -1.0;

    cv::filter2D(image,result,image.depth(),kernel);
}
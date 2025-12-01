//processing.cpp

#include <filesystem>
#include <processing.h>
#include <iostream>
#include <random>
#include "image_utils.h"
#include <math.h>


namespace fs = std::filesystem;

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

void detectHScolor(const cv::Mat& image,		// input image
    double minHue, double maxHue,	// Hue interval
    double minSat, double maxSat,	// saturation interval
    cv::Mat& mask) {				// output mask

    // convert into HSV space
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    // split the 3 channels into 3 images
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    // channels[0] is the Hue
    // channels[1] is the Saturation
    // channels[2] is the Value

    // Hue masking
    cv::Mat mask1; // below maxHue
    cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV);
    cv::Mat mask2; // over minHue
    cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY);

    cv::Mat hueMask; // hue mask
    if (minHue < maxHue)
        hueMask = mask1 & mask2;
    else // if interval crosses the zero-degree axis
        hueMask = mask1 | mask2;

    // Saturation masking
    // below maxSat
    cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV);
    // over minSat
    cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);

    cv::Mat satMask; // saturation mask
    satMask = mask1 & mask2;

    // combined mask
    mask = hueMask&satMask;
}


// Gamma expansion
cv::Mat computeRangeExpansion(const cv::Mat& image) {
    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        image.copyTo(grayImage);
    }

    cv::Mat expandedImage;

    cv::normalize(grayImage, expandedImage, 0, 255, cv::NORM_MINMAX, CV_8U);

    return expandedImage;
}

// Sobel Contour detection
cv::Mat detectEdgesSobel(const cv::Mat& grayImage, int ksize) {
    if (ksize % 2 == 0) ksize++; // ksize should be odd
    if (ksize < 3) ksize = 3;    // Minimum 3

    cv::Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
    cv::Mat grad;

    // Calculating x and y gradients
    cv::Sobel(grayImage, grad_x, CV_16S, 1, 0, ksize);
    cv::Sobel(grayImage, grad_y, CV_16S, 0, 1, ksize);


    cv::convertScaleAbs(grad_x, abs_grad_y);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    // Combining gradients approximation
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);


    return grad; // returns gradient image before binarisation
}

// Laplace Contour detection
cv::Mat detectEdgesLaplace(const cv::Mat& grayImage, int ksize) {
    if (ksize % 2 == 0) ksize++; // ksize should be odd
    if (ksize < 3) ksize = 3;    // Minimum of 3

    cv::Mat abs_dst, dst;

    // Applying operation
    cv::Laplacian(grayImage, dst, CV_16S, ksize);

    // Conversion
    cv::convertScaleAbs(dst, abs_dst);

    return abs_dst; // Returning gradient image
}

// Canny Contour detection
cv::Mat detectEdgesCanny(const cv::Mat& grayImage, double threshold1, double threshold2) {
    cv::Mat detectedEdges;

    // Detects de gradient, deletes non-maxima and 'seuillage' hystérésis)
    cv::Canny(grayImage, detectedEdges, threshold1, threshold2, 3, true);

    // ther resulting is already a binary image
    return detectedEdges;
}

// Global treatement and evaluation function
void processAndEvaluate(CallbackData* data) {
    if (data->batch.empty()) return;

    // Updating current original image
    const ImagePair& currentPair = data->batch[data->currentImageIndex];
    currentPair.originalImage.copyTo(data->originalImg);

    // Updating current reference image
    currentPair.referenceImage.copyTo(data->referenceImg);

    // Converting the image to grayscale for the detection
    cv::Mat grayImage;
    if (data->originalImg.channels() == 3) {
        cv::cvtColor(data->originalImg, grayImage, cv::COLOR_BGR2GRAY);
    } else {
        data->originalImg.copyTo(grayImage);
    }

    cv::Mat detectedEdges;
    int threshold = data->canny_threshold1; // Used for Sobel/Laplace
    int ksize = data->sobel_ksize * 2 + 1; // Kernel size: 1->3, 2->5, 3->7 always an odd number

    // Selecting and applying detecdtion algorithme
    if (data->detectorChoice == DETECTOR_SOBEL) {
        cv::Mat gradImage = detectEdgesSobel(grayImage, ksize);
        cv::threshold(gradImage, detectedEdges, threshold, 255, cv::THRESH_BINARY);

    } else if (data->detectorChoice == DETECTOR_LAPLACE) {
        cv::Mat gradImage = detectEdgesLaplace(grayImage, ksize);
        cv::threshold(gradImage, detectedEdges, threshold, 255, cv::THRESH_BINARY);

    } else if (data->detectorChoice == DETECTOR_CANNY) {
        detectedEdges = detectEdgesCanny(grayImage, data->canny_threshold1, data->canny_threshold2);
    }

    // Inverting
    cv::bitwise_not(detectedEdges, detectedEdges);

    // Copying the result to the structure
    detectedEdges.copyTo(data->displayImg);

    // Evaluation
    // Uses `data->displayImg` (detected contours) and `currentPair.referenceImage` (manual contours)
    data->currentMetrics = evaluateContours(data->displayImg, data->referenceImg, true);

    // Showing images
    showImage(data->origWin, data->originalImg);
    showImage(data->dispWin, data->displayImg);
    showImage(data->refWin, data->referenceImg);

    // Showing metrics
    cv::Mat metricsDisplay(900, 600, CV_8UC3, cv::Scalar(30, 30, 30));
    int y_offset = 30;
    int line_height = 30;

    // Information on the image and detector
    std::string filename = fs::path(currentPair.originalPath).filename().string();
    cv::putText(metricsDisplay, "Image: " + filename, cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
    y_offset += line_height;

    std::string detectorName = (data->detectorChoice == DETECTOR_SOBEL) ? "Sobel" :
                               (data->detectorChoice == DETECTOR_LAPLACE) ? "Laplace" : "Canny";
    cv::putText(metricsDisplay, "Detector: " + detectorName, cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(100, 200, 255), 2);
    y_offset += line_height * 4;

    // Five metrics
    cv::putText(metricsDisplay, "--- Base measurements ---", cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 100, 100), 1);
    y_offset += line_height;
    cv::putText(metricsDisplay, "Total Detectes (Dt): " + std::to_string(data->currentMetrics.detected), cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(200, 200, 200), 1);
    y_offset += line_height;
    cv::putText(metricsDisplay, "Total Reference (Rf): " + std::to_string(data->currentMetrics.reference), cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(200, 200, 200), 1);
    y_offset += line_height;
    cv::putText(metricsDisplay, "Corrects (Dt n Rf): " + std::to_string(data->currentMetrics.correct), cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(100, 255, 100), 1);
    y_offset += line_height;
    cv::putText(metricsDisplay, "False Positifs (FP): " + std::to_string(data->currentMetrics.falsePositive), cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(100, 100, 255), 1);
    y_offset += line_height;
    cv::putText(metricsDisplay, "False Negatifs (FN): " + std::to_string(data->currentMetrics.falseNegative), cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 100, 100), 1);
    y_offset += line_height;

    // Three performance metrics
    y_offset += line_height / 2;
    cv::putText(metricsDisplay, "--- Evaluation Metrics (P/TFP/TFN) ---", cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 100), 1);
    y_offset += line_height;

    cv::putText(metricsDisplay, "Performance (P): " + std::to_string(data->currentMetrics.P).substr(0, std::to_string(data->currentMetrics.P).find('.') + 4), cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(100, 255, 100), 2);
    y_offset += line_height;
    cv::putText(metricsDisplay, "rate Faux Positifs (TFP): " + std::to_string(data->currentMetrics.TFP).substr(0, std::to_string(data->currentMetrics.TFP).find('.') + 4), cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(100, 100, 255), 2);
    y_offset += line_height;
    cv::putText(metricsDisplay, "rate Faux Negatifs (TFN): " + std::to_string(data->currentMetrics.TFN).substr(0, std::to_string(data->currentMetrics.TFN).find('.') + 4), cv::Point(10, y_offset), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 100, 100), 2);

    // Updating result Image
    metricsDisplay.copyTo(data->resultImg);
    // Showing result Image
    cv::resizeWindow(data->resWin, cv::Size(600, 900));
    showImage(data->resWin, data->resultImg);
}

// Contour detection evaluation comared to a manually contour detected reference image
CallbackData::EvaluationMetrics evaluateContours(const cv::Mat& detectedImage, const cv::Mat& referenceImage, bool useNeighborhood) {
    CallbackData::EvaluationMetrics metrics;

    if (detectedImage.empty() || referenceImage.empty() || detectedImage.size() != referenceImage.size() || detectedImage.channels() > 1 || referenceImage.channels() > 1) {
        std::cerr << "Error : evaluation images are invalide (size, channels or empty)." << std::endl;
        return metrics;
    }

    int rows = detectedImage.rows;
    int cols = detectedImage.cols;

    // making sure the images are binary
    cv::Mat detectedBinary, referenceBinary;
    cv::threshold(detectedImage, detectedBinary, 128, 255, cv::THRESH_BINARY);
    cv::threshold(referenceImage, referenceBinary, 128, 255, cv::THRESH_BINARY);

    // Counting total pixels of the reference
    metrics.reference = cv::countNonZero(referenceBinary);

    // Iterating to calculate five measurements
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            bool isDetected = detectedBinary.at<uchar>(i, j) > 0;

            if (isDetected) {
                metrics.detected++;
            }

            // --- Intersection calcualation ---
            bool isCorrect = false;

            if (useNeighborhood) {
                // Neighbor strategy 3x3 : if the pixel at (i,j) is contour,
                // we verify if the pixel of reference is at least present in the neighborhood of 3x3.
                for (int ni = std::max(0, i - 1); ni <= std::min(rows - 1, i + 1); ++ni) {
                    for (int nj = std::max(0, j - 1); nj <= std::min(cols - 1, j + 1); ++nj) {
                        if (referenceBinary.at<uchar>(ni, nj) > 0) {
                            isCorrect = true;
                            break;
                        }
                    }
                    if (isCorrect) break;
                }
            } else {
                // Exact position strategy
                if (isDetected && referenceBinary.at<uchar>(i, j) > 0) {
                    isCorrect = true;
                }
            }

            if (isDetected && isCorrect) {
                metrics.correct++;
            }
        }
    }

    // False positif / negatif calculus
    // False positif = Detected - Correct
    metrics.falsePositive = metrics.detected - metrics.correct;
    // False negatif = Reference - Correct
    metrics.falseNegative = metrics.reference - metrics.correct;

    // 3 metrics calculus based on 3x3 neighboring
    long long denominator = metrics.correct + metrics.falsePositive + metrics.falseNegative;

    if (denominator > 0) {
        metrics.P = (double)metrics.correct / denominator;
        metrics.TFP = (double)metrics.falsePositive / denominator;
        metrics.TFN = (double)metrics.falseNegative / denominator;
    }

    return metrics;
}
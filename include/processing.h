//processing.h

#pragma once
#include <opencv2/opencv.hpp>


// fliping image
void flipImage(cv::Mat& image, cv::Mat& flip_image, int flipCode);

// draws salt pepper
void salt(cv::Mat image, int n);

// Color Reduction
void colorReduce(cv::Mat image, int div);

// Color Reduction with IO handling
void colorReduceIO(
    const cv::Mat &image, // means the image won't be modified
    cv::Mat &result,
    int div);

// Color Reduction Bitwise
void colorReduceB(cv::Mat image, int div);

// Color Reduction using Iterator Objects
void colorReduceIt(cv::Mat image, int div);

// Sharpenning
void sharpen(const cv::Mat &image, cv::Mat &result);


// Sharpen filtering
void sharpen2D(const cv::Mat &image, cv::Mat &result);
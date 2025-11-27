//processing.h

#pragma once
#include <opencv2/opencv.hpp>
#include <callback.h>

// Remapping
void wave(const cv::Mat &image, cv::Mat &result);

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

//
void detectHScolor(const cv::Mat& image,		// input image
    double minHue, double maxHue,	// Hue interval
    double minSat, double maxSat,	// saturation interval
    cv::Mat& mask);

// Gamma expansion
cv::Mat computeRangeExpansion(const cv::Mat& image);

// Sobel Contour detection
cv::Mat detectEdgesSobel(const cv::Mat& grayImage, int ksize);
// Laplace Contour detection
cv::Mat detectEdgesLaplace(const cv::Mat& grayImage, int ksize);
// Canny Contour detection
cv::Mat detectEdgesCanny(const cv::Mat& grayImage, double threshold1, double threshold2);


// Contour detection evaluation comared to a manually contour detected reference image
CallbackData::EvaluationMetrics evaluateContours(const cv::Mat& detectedImage, const cv::Mat& referenceImage, bool useNeighborhood = true);

// Global treatement and evaluation function
void processAndEvaluate(CallbackData* data);
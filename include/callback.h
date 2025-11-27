//callback.h

#pragma once
#include <opencv2/opencv.hpp>
#include <image_utils.h>

// Defnining deetector algorithme types
enum DetectorType {
    DETECTOR_SOBEL = 0,
    DETECTOR_LAPLACE = 1,
    DETECTOR_CANNY = 2
};

struct CallbackData {
    std::string origWin;
    std::string dispWin;
    std::string resWin;
    std::string refWin;

    cv::Mat originalImg;
    cv::Mat displayImg;
    cv::Mat resultImg;
    cv::Mat referenceImg;


    // Batch management
    ImageBatch batch;               // complete batch of pair images
    int currentImageIndex = 0;      // current image Index

    // Detection algorithme choice
    int detectorChoice = DETECTOR_SOBEL; // Choice via Trackbars (0=Sobel, 1=Laplace, 2=Canny)

    // Parameters
    int sobel_ksize = 1;            // Kernel size of Sobel/Laplace (should be odd, 1, 3, 5, 7)
    int canny_threshold1 = 100;     // First threshold for Canny
    int canny_threshold2 = 200;     // Second threshold for Canny

    // Results
    // Structure for storing three metrics (P, TFP, TFN) of current image
    struct EvaluationMetrics {
        double P = 0.0;
        double TFP = 0.0;
        double TFN = 0.0;
        // storing five base measurements as well
        long long detected = 0;
        long long reference = 0;
        long long correct = 0;
        long long falsePositive = 0;
        long long falseNegative = 0;
    } currentMetrics;
};


void onTrackbarChange(int, void* userData);

void onMouse(int event, int x, int y, int flags, void* userdata);

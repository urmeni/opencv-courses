//main.cpp
#include "image_utils.h"
#include <processing.h>
#include <iostream>
#include <filesystem>
#include <callback.h>
#include <colordetector.h>
#include "histogram.h"



std::string data_path = DATA_DIR;

const std::string ORIG_DIR = std::string(DATA_DIR) + "/lot-orig";
const std::string REF_DIR = std::string(DATA_DIR) + "/lot-manual-detected";


int main() {
    // Data Setup ============================
    CallbackData data;
    data.origWin = "Original Image";
    data.dispWin = "Display Image";
    data.resWin = "Result Evaluation";
    data.refWin = "Reference Image";

    // loading batch images
    data.batch = loadImageBatch(ORIG_DIR, REF_DIR);
    if (data.batch.empty()) {
        std::cerr << "Non image pairs loaded. End of the programme" << std::endl;
        return 1;
    }


    // Initializing with the first image of the batch
    data.batch[data.currentImageIndex].originalImage.copyTo(data.originalImg);
    data.displayImg = data.batch[data.currentImageIndex].originalImage;
    data.displayImg.copyTo(data.resultImg); // Initialising the result image


    // Creating windows
    cv::namedWindow(data.origWin); // Original image window
    cv::namedWindow(data.dispWin); // Display image window (Alogrithme Contour detection)
    cv::namedWindow(data.resWin, cv::WINDOW_FREERATIO); // Metrics windows
    cv::namedWindow(data.refWin); // Reference image window (Manual Contour detected)


    // Creating trackbars for Display window

    // Trackbar 1 : Detector choice
    cv::createTrackbar("Detector", data.dispWin, &data.detectorChoice, 2, onTrackbarChange, &data);
    cv::setTrackbarMin("Detector", data.dispWin, 0); // Tags : 0: Sobel, 1: Laplace, 2: Canny

    // Trackbar 2 : Parameters 1 (low threshold for Canny or kernel size for Sobel/Laplace)
    cv::createTrackbar("Parameter 1 (Threshold/Ksize)", data.dispWin, &data.canny_threshold1, 500, onTrackbarChange, &data);

    // Trackbar 3: Parameters 2 (high threshold for Canny)
    cv::createTrackbar("Parameter 2 (high threshold for Canny)", data.dispWin, &data.canny_threshold2, 500, onTrackbarChange, &data);


    // 5. Callbacks
    cv::setMouseCallback(data.dispWin, onMouse, &data);



    // ============================================
    // processing =================================
    // ============================================
    // First processing on initialised data
    processAndEvaluate(&data);


    waitDestroy(0);
    return 0;
}
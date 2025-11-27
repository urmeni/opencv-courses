//main.cpp
#include "image_utils.h"
#include <processing.h>
#include <iostream>
#include <filesystem>
#include <callback.h>
#include <colordetector.h>
#include "histogram.h"

std::string data_path = DATA_DIR;
std::string img_name = "dark.png";
std::string path = data_path + "/" + img_name;

int main() {
    // Setup ============================
    // data
    CallbackData data;
    data.origWin = "Original Image";
    data.dispWin = "Display Image";
    data.resWin = "Result Image";
    data.evalWin = "Evaluation Vue";

    // loading image
    cv::Mat img = loadImage(path);
    img.copyTo(data.originalImg);

    // ============================================
    // processing =================================
    // ============================================
    // The histogram object
    Histogram1D h;

    // Expanding gamma
    cv::Mat expandedImg = computeRangeExpansion(img);

    // Compute the histogram
    cv::Mat histoOrig = h.getHistogram(img);
    cv::Mat histoExpanded = h.getHistogram(expandedImg);



    // Display a histogram as an image
    cv::namedWindow("Histogramme Original");
    cv::imshow("Histogramme Original", h.getHistogramImage(img));

    cv::namedWindow("Histogramme Etendu");
    cv::imshow("Histogramme Etendu", h.getHistogramImage(expandedImg));


    data.displayImg = expandedImg;
    data.displayImg.copyTo(data.resultImg);
    cv::namedWindow(data.dispWin);
    // Callbacks ==================================
    cv::setMouseCallback("Display Image", onMouse, &data);

    showImage(data.origWin, data.originalImg);
    showImage(data.dispWin, data.displayImg);
    waitDestroy(0);
    return 0;
}
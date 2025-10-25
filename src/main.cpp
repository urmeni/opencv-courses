//main.cpp
#include "image_utils.h"
#include <processing.h>
#include <iostream>
#include <filesystem>
#include <callback.h>

int main() {
    CallbackData data;
    data.mainWin = "Display Image";
    data.resultWin = "Resulting Image";

    std::string path = "data/imageA.jpg";
    std::string secpath = "data/brick.jpg";
    std::cout << "CWD: " << std::filesystem::current_path() << "\n";

    // Loading images
    cv::Mat imgA = loadImage(path, cv::IMREAD_GRAYSCALE);
    if (imgA.empty()) return -1;
    cv::Mat blend = loadImage(secpath, cv::IMREAD_GRAYSCALE);
    if (blend.empty()) return -1;


    wave(imgA, imgA);


    data.originalImg = imgA;

    data.originalImg.copyTo(data.displayImage);

    cv::namedWindow(data.mainWin);

    // Callbacks
    cv::setMouseCallback("Display Image", onMouse, &data);

    showImage(data.mainWin, data.displayImage);
    waitDestroy(0);
    return 0;
}
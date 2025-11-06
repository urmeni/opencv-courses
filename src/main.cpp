//main.cpp
#include "image_utils.h"
#include <processing.h>
#include <iostream>
#include <filesystem>
#include <callback.h>

int main() {
    CallbackData data;
    data.origWin = "Original Image";
    data.dispWin = "Display Image";
    data.resWin = "Result Image";
    data.evalWin = "Evaluation Vue";

    std::string path = "/home/urmenus/CLionProjects/opencv-courses/data/imageA.jpg";
    std::string secpath = "/home/urmenus/CLionProjects/opencv-courses/data/brick.jpg";
    std::cout << "CWD: " << std::filesystem::current_path() << "\n";

    // Loading images
    cv::Mat imgA = loadImage(path, cv::IMREAD_GRAYSCALE);
    cv::Mat blend = loadImage(secpath, cv::IMREAD_GRAYSCALE);


    data.displayImg = imgA;
    data.displayImg.copyTo(data.originalImg);

    //processing
    wave(imgA, imgA);
    flipImage(imgA, imgA, 1);
    salt(imgA, 100);
    colorReduceIt(imgA, 50);
    sharpen(imgA, imgA);
    //sharpen2D(imgA, imgA);

    data.displayImg.copyTo(data.resultImg);


    // Callbacks
    cv::namedWindow(data.dispWin);
    cv::setMouseCallback("Display Image", onMouse, &data);

    showImage(data.origWin, data.originalImg);
    showImage(data.dispWin, data.displayImg);
    waitDestroy(0);
    return 0;
}
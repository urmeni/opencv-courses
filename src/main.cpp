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

    std::string path = "data/test.jpg";
    std::cout << "CWD: " << std::filesystem::current_path() << "\n";

    data.originalImg = loadImage(path, cv::IMREAD_GRAYSCALE);
    if (data.originalImg.empty()) return -1;

    data.originalImg.copyTo(data.displayImage);

    cv::namedWindow(data.mainWin);

    // Callbacks
    cv::setMouseCallback("Display Image", onMouse, &data);

    showImage(data.mainWin, data.displayImage);
    waitDestroy(0);
    return 0;
}
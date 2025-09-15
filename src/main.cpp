//main.cpp
#include "image_utils.h"
#include <iostream>
#include <filesystem>

int main() {
    std::string path = "data/test.jpg";
    std::cout << "CWD: " << std::filesystem::current_path() << "\n";

    cv::Mat img = loadImage(path, cv::IMREAD_COLOR);
    if (img.empty()) return -1;

    showImage("Test Window", img, 0);
    return 0;
}
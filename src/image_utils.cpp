//image_utils.cpp

#include "image_utils.h"
#include <iostream>
#include <filesystem>

cv::Mat loadImage(const std::string &path, int flags) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "File not found: " << path << "\n";
        return {};
    }
    cv::Mat img = cv::imread(path, flags);
    if (img.empty()) {
        std::cerr << "Failed to read image: " << path << "\n";
    }
    return img;
}

void showImage(const std::string &winName, const cv::Mat &img, int waitMs) {
    if (img.empty()) {
        std::cerr << "showImage called with empty Mat\n";
        return;
    }
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, img);
    cv::waitKey(waitMs); // 0 => block until key, >0 => wait that many ms
}

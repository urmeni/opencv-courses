//image_utils.h

#pragma once
#include <opencv2/opencv.hpp>
#include <string>

cv::Mat loadImage(const std::string &path, int flags = cv::IMREAD_COLOR);
void showImage(const std::string &winName, const cv::Mat &img, int waitMs = 0);
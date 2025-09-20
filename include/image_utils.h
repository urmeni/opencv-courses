//image_utils.h

#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

// loading an image
// returns cv::Mat
cv::Mat loadImage(const std::string &path, int flags = cv::IMREAD_COLOR);

// showing a single window image
void showImage(const std::string &winName, const cv::Mat &img);

// showing multiple images
void showImages(const std::vector<std::string> &winNames, const std::vector<cv::Mat>& images);

// waiting certain ms or key down until destruction
void waitDestroy (int ms);
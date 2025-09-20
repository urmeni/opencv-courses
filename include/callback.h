//callback.h

#pragma once
#include <opencv2/opencv.hpp>

struct CallbackData {
    cv::Mat originalImg;
    cv::Mat displayImage;
    std::string mainWin;
    std::string resultWin;
};

void onMouse(int event, int x, int y, int flags, void* userdata);
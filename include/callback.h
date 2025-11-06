//callback.h

#pragma once
#include <opencv2/opencv.hpp>

struct CallbackData {
    cv::Mat originalImg;
    cv::Mat resultImg;
    cv::Mat displayImg;
    cv::Mat evaluationImg;
    std::string origWin;
    std::string resWin;
    std::string dispWin;
    std::string evalWin;
};

void onMouse(int event, int x, int y, int flags, void* userdata);
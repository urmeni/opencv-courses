//callback.cpp

#include <callback.h>
#include <iostream>
#include "image_utils.h"


static void plotRowIntensity(const cv::Mat &img, int row, const std::string &winName) {
    CV_Assert(img.type() == CV_8UC1);

    int nc = img.cols;
    cv::Mat plotImg(256, nc, CV_8UC3, cv::Scalar(255, 255, 255));

    for (int x = 1; x < nc; x++) {
        int y1 = 255 - img.at<uchar>(row, x-1);
        int y2 = 255 - img.at<uchar>(row, x);
        cv::line(plotImg, cv::Point(x - 1, y1), cv::Point(x, y2), cv::Scalar(0, 0, 0),1);
    }

    showImage(winName, plotImg);
}

void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event != cv::EVENT_LBUTTONDOWN) return;

    auto *data = reinterpret_cast<CallbackData*>(userdata);

    std::cout << "Clicked at : " << x << ", " << y << " Value = "
        << static_cast<int>(data->displayImg.at<uchar>(y, x)) << std::endl;

    // Resetting from result image
    data->resultImg.copyTo(data->displayImg);

    // Draw horizontal line
    cv::line(
        data->displayImg,
        cv::Point(0, y),
        cv::Point(data->displayImg.cols, y),
        cv::Scalar(255, 0, 0), 1);

    cv::imshow(data->dispWin, data->displayImg);

    // Plot Row intensity
    plotRowIntensity(data->resultImg, y, data->evalWin);
}

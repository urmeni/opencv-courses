//callback.cpp

#include <callback.h>
#include <iostream>
#include "image_utils.h"

void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event != cv::EVENT_LBUTTONDOWN) return;

    auto *data = reinterpret_cast<CallbackData*>(userdata);

    std::cout << "Clicked at : " << x << ", " << y << " Value = "
        << static_cast<int>(data->displayImg.at<uchar>(y, x)) << std::endl;


    // Do something

}

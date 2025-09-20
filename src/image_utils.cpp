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

void showImage(const std::string &winName, const cv::Mat &img) {
    if (img.empty()) {
        std::cerr << "showImage called with empty Mat\n";
        return;
    }
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    cv::imshow(winName, img);
}

// showing multiple images
void showImages(const std::vector<std::string> &winNames,
                const std::vector<cv::Mat>& images) {
    if (images.size() != winNames.size()) {
        std::cerr << "Number of images mismatch with the number of windows names\n";
        return;
    }

    for (size_t i = 0; i < images.size(); i++) {
        if (images[i].empty()) {
            std::cerr << "Image" << i << " is empty\n";
            return;
        }
        cv::imshow(winNames[i], images[i]);
    }


}


// waiting certain ms or key down until destruction
void waitDestroy (int ms) {
    cv::waitKey(ms);
    cv::destroyAllWindows();
}


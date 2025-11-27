//image_utils.cpp

#include "image_utils.h"
#include <iostream>
#include <filesystem>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// loading a batch of images, returns the vector of structure pairs of original and reference images
ImageBatch loadImageBatch(const std::string& origDir, const std::string& refDir) {
    ImageBatch batch;

    // Verifying given directories
    if (!fs::exists(origDir) || !fs::is_directory(origDir)) {
        std::cerr << "Error: non existing or not a directory : " << origDir << std::endl;
        return batch;
    }
    if (!fs::exists(refDir) || !fs::is_directory(refDir)) {
        std::cerr << "Error: non existing or not a directory : " << refDir << std::endl;
        return batch;
    }

    //  Browse original images directory
    for (const auto& entry : fs::directory_iterator(origDir)) {
        if (entry.is_regular_file()) {
            std::string origFilename = entry.path().filename().string();

            // Reference image file name deduction :
            // orig-imageX.jpg -> manual-detect-imageX.jpg
            std::string refFilename = origFilename;
            // Replace 'orig-' by 'manual-detect-'
            size_t pos = refFilename.find("orig-");
            if (pos != std::string::npos) {
                refFilename.replace(pos, 5, "manual-detect-"); // 5 is the length of "orig-"
            } else {
                // if naming conventions not respected we skip
                std::cerr << "Warning: Name of file original unwanted, skipped : " << origFilename << std::endl;
                continue;
            }

            fs::path originalPath = entry.path();
            fs::path referencePath = fs::path(refDir) / refFilename;

            // Verifying weather reference image file exists
            if (!fs::exists(referencePath)) {
                std::cerr << "Warning: Reference image missing for : " << origFilename << ". Skipped." << std::endl;
                continue;
            }

            // Loading images
            cv::Mat originalImg = cv::imread(originalPath.string(), cv::IMREAD_COLOR);
            cv::Mat referenceImg = cv::imread(referencePath.string(), cv::IMREAD_GRAYSCALE);

            if (originalImg.empty() || referenceImg.empty()) {
                std::cerr << "Error while loading pairs of images : " << origFilename << ". Skipped." << std::endl;
                continue;
            }


            ImagePair pair;
            pair.originalPath = originalPath.string();
            pair.referencePath = referencePath.string();
            pair.originalImage = originalImg;
            pair.referenceImage = referenceImg;

            batch.push_back(pair);
        }
    }


    // Sorting by file name
    std::sort(batch.begin(), batch.end(), [](const ImagePair& a, const ImagePair& b) {
        return a.originalPath < b.originalPath;
    });

    std::cout << "Batch of images loaded successfully, number of pairs : " << batch.size() << std::endl;

    return batch;

}



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


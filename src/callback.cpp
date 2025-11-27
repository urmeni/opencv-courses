//callback.cpp

#include <callback.h>
#include <iostream>
#include "image_utils.h"
#include <processing.h>

void onTrackbarChange(int, void* userData) {
    CallbackData* data = static_cast<CallbackData*>(userData);
    processAndEvaluate(data);


    //std::cout << "Current Detector choice : " << data->detectorChoice << std::endl;
}


void onMouse(int event, int x, int y, int flags, void* userData) {
    CallbackData* data = static_cast<CallbackData*>(userData);

    if (event == cv::EVENT_LBUTTONDOWN) {
        // Left Clic : next image
        data->currentImageIndex = (data->currentImageIndex + 1) % data->batch.size();
        processAndEvaluate(data);
        //std::cout << "Current batch number : " << data->currentImageIndex << std::endl;
    } else if (event == cv::EVENT_RBUTTONDOWN) {
        // Right click : previous image
        data->currentImageIndex = (data->currentImageIndex - 1 + data->batch.size()) % data->batch.size();
        processAndEvaluate(data);
        //std::cout << "Current batch number : " << data->currentImageIndex << std::endl;
    }

}


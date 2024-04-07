#pragma once
#include "global-resources.h"

// #include "opencv.h"
#include <opencv2/opencv.hpp>
// #include <opencv2/highgui/highgui.hpp>

class OpenCV {

public:

    void opencvMainLoop();

private:

    void opencvReadImages();

    bool close_ocv;
    bool update_ocv_images;
    cv::Scalar highlight_color;

};
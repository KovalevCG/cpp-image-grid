#pragma once
#include "global-resources.h"

// #include "opencv.h"
#include <opencv2/opencv.hpp>
// #include <opencv2/highgui/highgui.hpp>

class OpenCV {

public:

    void opencvMainLoop();

private:
    static constexpr int SIZE = 20;

    void opencvReadImages();

    bool close_ocv;
    bool update_ocv_images;
    cv::Scalar highlight_color;

    std::array<std::array<cv::Mat, SIZE>, SIZE> images;
};
#pragma once
#include "global-resources.h"

// #include "opencv.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <array>

class OpenCV {

public:
    OpenCV();
    void startOpencvMainLoop();

private:
    static constexpr int SIZE = 20;

    void opencvReadImages();
    void setTotalSizes();
    cv::Mat createImage(int col, int row, std::string combined = "none", bool resize = true);

    bool close_ocv = false;
    bool update_ocv_images = true;

    cv::Scalar highlight_color = cv::Scalar(220, 220, 0);
    std::array<std::array<cv::Mat, SIZE>, SIZE> images;
    std::array<int, SIZE> cell_widths { };
    std::array<int, SIZE> cell_heights { };
    std::array<std::array<double, SIZE>, SIZE> zoom { };
    std::array<std::array<int, SIZE>, SIZE> tr_x { };
    std::array<std::array<int, SIZE>, SIZE> tr_y { };
    // std::array<bool, 20> merged_cols { };
    // std::array<bool, 20> merged_rows { };

    int num_of_cols = 0;
    int num_of_rows = 0;
    int start_height = 700;
    int start_width = 900;
    int width_total = 0;
    int height_total = 0;
    int initial_cell_width = 0;
    int initial_cell_height = 0;

};
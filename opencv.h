#pragma once
#include "global-resources.h"

// #include "opencv.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <array>

// #include <QThread>

class OpenCV {

public:
    OpenCV();
    void startOpencvMainLoop();

private:
    static constexpr int SIZE = 20;

    void opencvReadImages();
    void setTotalSizes();
    static void onMouse(int event, int x, int y, int flags, void* userdata);
    cv::Mat createImage(int col, int row, std::string combined = "none", bool resize = true);
    void mousePosition(int x, int y);

    bool close_ocv = false;
    bool update_ocv_images = true;
    bool resize = false;
    bool move = false;
    // bool shift_pressed = false;     // Perhaps unnecessary


    cv::Scalar highlight_color = cv::Scalar(220, 220, 0);
    std::array<std::array<cv::Mat, SIZE>, SIZE> images;
    std::array<int, SIZE> cell_widths { };
    std::array<int, SIZE> cell_heights { };
    std::array<std::array<double, SIZE>, SIZE> zoom { };
    std::array<std::array<int, SIZE>, SIZE> tr_x { };
    std::array<std::array<int, SIZE>, SIZE> tr_y { };
    std::array<int, 2> mouse_on_cell { };

    int num_of_cols = 0;
    int num_of_rows = 0;
    int start_height = 700;
    int start_width = 900;
    int width_total = 0;
    int height_total = 0;
    int initial_cell_width = 0;
    int initial_cell_height = 0;
    unsigned int resize_inc_left = 0;
    unsigned int resize_inc_right = 0;

    int start_x = 0;
    int start_y = 0;

    std::string mouse_on_type = "none";
    int mouse_on_num = 0;

    std::chrono::time_point<std::chrono::system_clock> border_time;

};
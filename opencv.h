#pragma once
#include "global-resources.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <array>

class OpenCV {

public:
    OpenCV();
    void startOpencvMainLoop();
    void saveImage(std::string path = "");

private:

    static constexpr size_t SIZE = 2;

    void opencvReadImages();
    void setTotalSizes();
    static void onMouse(int event, int x, int y, int flags, void* userdata);
    cv::Mat createImage(int col, int row, std::string combined = "none", bool resize = true);
    cv::Mat createSaveImage(int col, int row, std::string combined = "none", bool resize = true);
    void mousePosition(int x, int y);
    void runSaveFileDialogFromQt();

    bool close_ocv = false;
    bool update_ocv_images = true;
    bool resize = false;
    bool move = false;

    cv::Scalar HIGHLIGHT_COLOR = cv::Scalar(220, 220, 0);
    cv::Scalar WHITE_COLOR = cv::Scalar(245, 245, 245);
    cv::Scalar BLACK_COLOR = cv::Scalar(10, 10, 10);
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

    // Mouse
    int start_x = 0;
    int start_y = 0;
    std::string mouse_on_type = "none";
    int mouse_on_num = 0;

    // Save Image
    int width_save_total = 0;
    int height_save_total = 0;
    std::array<int, SIZE> cell_save_widths { };
    std::array<int, SIZE> cell_save_heights { };

};
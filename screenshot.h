#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class Screenshot {
public:
    Screenshot();
    bool screenshotRegion(int c, int r);

private:

    std::string temp_dir;
    cv::Mat screen;

    bool show_rectangle = false;
    int screen_x_start = 0, screen_x_end = 0;
    int screen_y_start = 0, screen_y_end = 0;
    bool screen_move = false;
    bool screen_region_done = false;
    bool draw = false;

    int move_start_x = 0;
    int move_start_y = 0;
    int move_end_x = 0;
    int move_end_y = 0;

    static void mouseScreenshot(int event, int x, int y, int flags, void* userdata);
    void updateMousePosition(int event, int x, int y, int flags);
};

#include "screenshot.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <sstream>
#include <iomanip>
#include <string>

//#define NOMINMAX
//#include <windows.h>

using std::cout;
using std::endl;

Screenshot::Screenshot() {}

bool Screenshot::screenshotRegion(int c, int r) {

    show_rectangle = false;
    screen_x_start = 0, screen_x_end = 0;
    screen_y_start = 0, screen_y_end = 0;
    screen_move = false;
    screen_region_done = false;
    draw = false;

    cv::namedWindow("Screenshot Cropping", cv::WND_PROP_FULLSCREEN);
    cv::setWindowProperty("Screenshot Cropping", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    cv::setMouseCallback("Screenshot Cropping", Screenshot::mouseScreenshot, this);

    // CreateDirectory("screenshots", NULL);
    std::ostringstream oss;
    oss << "screenshots/screenshot_"
        << std::setw(2) << std::setfill('0') << c << "_"
        << std::setw(2) << std::setfill('0') << r << ".png";
    std::string path = oss.str();

    screen = cv::imread(path);
    cv::Mat zeros = cv::Mat::zeros(screen.size(), screen.type());
    double alpha = 0.5;
    double beta = 1 - alpha;
    cv::Mat screen_dark;
    cv::addWeighted(screen, alpha, zeros, beta, 0, screen_dark);

    while (true) {
        cv::Mat screen_final = screen_dark.clone();
        if (show_rectangle) {
            int y1 = std::min(screen_y_start, screen_y_end);
            int y2 = std::max(screen_y_start, screen_y_end);
            int x1 = std::min(screen_x_start, screen_x_end);
            int x2 = std::max(screen_x_start, screen_x_end);
            if ((x2 - x1 > 0) && (y2 - y1 > 0)) {
                screen(cv::Rect(x1, y1, x2 - x1, y2 - y1)).copyTo(screen_final(cv::Rect(x1, y1, x2 - x1, y2 - y1)));
            }
        }

        // Define the text color
        cv::Scalar textColor(200, 200, 0);
        // Put text on the image
        cv::putText(screen_final, "Select area of screen to capture", cv::Point(35, 35),
            cv::FONT_HERSHEY_PLAIN, 1, textColor, 1, cv::LINE_AA);
        cv::putText(screen_final, "SHIFT when selecting - Move selection", cv::Point(35, 65),
            cv::FONT_HERSHEY_PLAIN, 1, textColor, 1, cv::LINE_AA);
        cv::putText(screen_final, "Esc - Exit", cv::Point(35, 95),
            cv::FONT_HERSHEY_PLAIN, 1, textColor, 1, cv::LINE_AA);

        cv::imshow("Screenshot Cropping", screen_final);
        int key = cv::waitKey(1);

        if (key == 27) {
            break;
        }
        if (screen_region_done) {
            break;
        }
    }
    cv::destroyWindow("Screenshot Cropping");

    if (screen_region_done) {
        int y1 = std::min(screen_y_start, screen_y_end);
        int y2 = std::max(screen_y_start, screen_y_end);
        int x1 = std::min(screen_x_start, screen_x_end);
        int x2 = std::max(screen_x_start, screen_x_end);
        cv::imwrite(path, screen(cv::Rect(x1, y1, x2 - x1, y2 - y1)));
    }
    return screen_region_done;
}

void Screenshot::mouseScreenshot(int event, int x, int y, int flags, void* userdata) {
    Screenshot* self = static_cast<Screenshot*>(userdata);
    self->updateMousePosition(event, x, y, flags);
}

void Screenshot::updateMousePosition(int event, int x, int y, int flags) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        screen_x_start = x;
        screen_y_start = y;
        screen_x_end = x - 1;  // to make sure we have a region to draw initially
        screen_y_end = y - 1;
        draw = true;
    }
    else if (event == cv::EVENT_LBUTTONUP) {
        screen_x_end = x;
        screen_y_end = y;
        screen_region_done = true;
        draw = false;
    }
    else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_SHIFTKEY)) {
        if (!screen_move) {
            move_start_x = screen_x_start - x;
            move_start_y = screen_y_start - y;
            move_end_x = screen_x_end - x;
            move_end_y = screen_y_end - y;
        }
        screen_move = true;
        screen_x_start = move_start_x + x;
        screen_y_start = move_start_y + y;
        screen_x_end = move_end_x + x;
        screen_y_end = move_end_y + y;

        if (screen_x_start < 0) {
            screen_x_start += abs(screen_x_start);
            screen_x_end += abs(screen_x_start);
        }
        if (screen_y_start < 0) {
            screen_y_start += abs(screen_y_start);
            screen_y_end += abs(screen_y_start);
        }
    }
    else if (event == cv::EVENT_MOUSEMOVE ) {
        screen_move = false;
        if (draw) {
            screen_x_end = x;
            screen_y_end = y;
            if (!show_rectangle) {
                show_rectangle = true;
            }
        }
    }
}
#include "ocv-test.h"
#include <opencv2/opencv.hpp>

OcvTest::OcvTest() {}

void OcvTest::show() {
    // Create an OpenCV window
    cv::namedWindow("OCV Test", cv::WINDOW_AUTOSIZE);
    cv::Mat image = cv::Mat::zeros(400, 400, CV_8UC3);

    // Show the window
    while (true) {
        cv::imshow("OCV Test", image);
        int key = cv::waitKey(1); // Wait for a key press with a short delay to keep CPU usage reasonable

        if (key == 'a' || key == 'A') {
            if (aboutCallback) {
                aboutCallback();  // Call the injected about function
            }
        }
        else if (key == 27) {  // 'ESC' key to close the window
            break;
        }
    }
    cv::destroyWindow("OCV Test");
}

void OcvTest::setAboutCallback(const std::function<void()>& callback) {
    aboutCallback = callback;
}
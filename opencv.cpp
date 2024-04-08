#include "opencv.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

bool close_ocv = false;
bool update_ocv_images = true;
cv::Scalar highlight_color = cv::Scalar(220, 220, 0);


void OpenCV::opencvMainLoop() {

    cv::namedWindow("Edit", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Edit", 50, 50);
    // cv::setMouseCallback("Edit", onMouse, nullptr);

    while (!this->close_ocv) {

        // update opencv images if needed
        if (this->update_ocv_images) {
            this->opencvReadImages();
            this->update_ocv_images = false;
        }

        // Stack Images
        // If we don't have combined columns
        if (!GlobalResources::anyMergedCols()) {

        }




        //if (cv::waitKey(100) >= 0)
        //{
        //    std::cout << "Break" << std::endl;
        //    break;
        //}
        if (cv::waitKey(30) >= 0) {
            close_ocv = true;
        }
    }
}

void OpenCV::opencvReadImages() {
    // Load images based on paths in GlobalResources
    for (size_t row = 0; row < GlobalResources::num_of_rows; ++row) {
        for (size_t col = 0; col < GlobalResources::num_of_cols; ++col) {
            images[row][col] = cv::imread(GlobalResources::getImagePath(col, row), cv::IMREAD_COLOR);
        }
    }
}
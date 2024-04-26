#include "ocv-test.h"

#include <opencv2/opencv.hpp>
#include <iostream>
using std::cout;
using std::endl;


//void OcvTest::alphaBlend(cv::Mat& foreground, cv::Mat& background, cv::Mat& alpha, cv::Mat& outImage) {
//    // Assume all images are the same size and type (CV_32FC1 for single-channel float)
//    int numberOfPixels = foreground.rows * foreground.cols * foreground.channels();
//
//    float* fptr = reinterpret_cast<float*>(foreground.data);
//    float* bptr = reinterpret_cast<float*>(background.data);
//    float* aptr = reinterpret_cast<float*>(alpha.data);
//    float* outImagePtr = reinterpret_cast<float*>(outImage.data);
//
//    for (int i = 0; i < numberOfPixels; i++, outImagePtr++, fptr++, aptr++, bptr++) {
//        *outImagePtr = (*fptr) * (*aptr) + (*bptr) * (1 - *aptr);
//    }
//}

void OcvTest::alphaBlend(const cv::Mat& foreground, const cv::Mat& background, const cv::Mat& alpha, cv::Mat& outImage) {
    // Ensure the output image has the same size and type as the background
    outImage = background.clone();

    // Use the alpha mask to blend the foreground onto the background
    foreground.copyTo(outImage, alpha);
}

void OcvTest::runTest() {

    cv::Scalar highlight_color = cv::Scalar(220, 220, 0);
    cv::Scalar white_color = cv::Scalar(250, 250, 250);

    // Load the foreground and background images and alpha masks
    cv::Mat background = cv::imread("images/test/background.png", cv::IMREAD_COLOR);
    cv::Mat mask_thin = cv::imread("images/test/mask_thin.png", cv::IMREAD_GRAYSCALE);
    cv::Mat mask_thick = cv::imread("images/test/mask_thick.png", cv::IMREAD_GRAYSCALE);
    cv::Mat foreground_highlighted = cv::Mat::zeros(40, 40, CV_8UC3);
    foreground_highlighted.setTo(highlight_color);
    cv::Mat foreground_white = cv::Mat::zeros(40, 40, CV_8UC3);
    foreground_white.setTo(white_color);
    // Creating ROI
    cv::Rect roi_save(background.cols - mask_thick.cols, 0, mask_thick.cols, mask_thick.rows);
 
    // -------------------------------------------

    cv::Mat roi = background(roi_save);

    // Apply masks and combine
    foreground_white.copyTo(roi, mask_thick);
    foreground_highlighted.copyTo(roi, mask_thin);


    // Show the result
    cv::imshow("Alpha Blended Image", background);
    cv::waitKey(0);

}

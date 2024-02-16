#include "opencv.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

void OpenCV::displayOpenCVWindow() {






    cv::Mat img1 = cv::imread(globalResources.getImagePath(0, 0));

    cv::Mat img2 = cv::imread(globalResources.getImagePath(1, 0));

    // Ensure images are loaded
    //if (img1.empty() || img2.empty()) {
    //    std::cerr << "Error reading one of the images." << std::endl;
    //    return -1;
    //}

    // Step 2: Crop the images to 500x500px
    // Assuming we crop starting from the top-left corner (0, 0)
    //cv::Rect roi(0, 0, 500, 500); // x, y, width, height
    //cv::Mat img1Cropped = img1(roi);
    //cv::Mat img2Cropped = img2(roi);

    // Step 3: Combine the images horizontally
    cv::Mat combinedImg;
    cv::hconcat(img1, img2, combinedImg);

    cv::namedWindow("Edit", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Edit", 50, 50);
    cv::imshow("Edit", combinedImg);

    // cv::waitKey(0); 


    //cv2.namedWindow('Edit', cv2.WINDOW_AUTOSIZE)
    //cv2.moveWindow("Edit", 50, 50)









    //cv::Mat image = cv::imread("images/bg_image_opencv.png");

    //// cv::Mat image = cv::Mat::zeros(480, 640, CV_8UC3);
    //cv::putText(image, "Hello from OpenCV", cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255), 2);
    //cv::imshow("OpenCV Window", image);
    //cv::waitKey(0);
}
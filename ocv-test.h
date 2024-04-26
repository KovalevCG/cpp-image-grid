#pragma once

#include <opencv2/core.hpp>

class OcvTest
{
public:
	void runTest();
	
private:
	void alphaBlend(const cv::Mat& foreground, const cv::Mat& background, const cv::Mat& alpha, cv::Mat& outImage);

};


#pragma once

//standard includes
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

//opencv includes
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//dlib includes


class stopsign_detector{

public:
	stopsign_detector();
	bool detected();

private:
	cv::VideoCapture cap;
};



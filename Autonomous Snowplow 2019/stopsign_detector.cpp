#include "stopsign_detector.h"

using namespace std;

stopsign_detector::stopsign_detector() {
	cout << "Initializing stopsign detector" << endl;
	if (!cap.open(0))
	{
		cout << "Failed to initialize camera" << endl;
		exit(0);
	}
}

bool stopsign_detector::detected() {
	cv::Mat frame;
	cap >> frame;
	cv::imshow("Image", frame);
	cv::waitKey(5);
	return false;
}
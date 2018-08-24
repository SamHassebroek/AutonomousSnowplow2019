#include "stopsign_detector.h"

using namespace std;

stopsign_detector::stopsign_detector(bool vis, atomic<bool> * stopsign_detected) {
	cout << "Initializing stopsign detector" << endl;
	stopsign_detected_ref = stopsign_detected;
	*stopsign_detected_ref = false;
	visualize = vis;
	if (!cap.open(0))
	{
		cout << "Failed to initialize camera" << endl;
	}
	ifstream fin("C:/ISU Robotics/AutonomousSnowplow2019/x64/Release/object_detector.svm", ios::binary);
	if (!fin)
	{
		cout << "Couldn't find object_detector.svm" << endl;
	}
	deserialize(detector, fin);
	detected = false;
}

bool stopsign_detector::detect() {
	cv::Mat frame;
	bool ret = false;
	/*-------------------
	Capture Image, Detect Stop Sign
	--------------------*/
	cap >> frame;
	array2d<bgr_pixel> dlibImage;
	assign_image(dlibImage, dlib::cv_image<bgr_pixel>(frame));
	const std::vector<rectangle> rects = detector(dlibImage);
	//cout << "Number of detections: " << rects.size() << endl;
	if (rects.size() > 0) {
		ret = true;
		//cout << rects.at(0) << endl;
	}
	if (visualize)
	{
		if (rects.size() > 0) {
			cv::Rect rectToDraw(rects.at(0).left(), rects.at(0).top(), rects.at(0).width(), rects.at(0).height());
			cv::rectangle(frame, rectToDraw, cv::Scalar(0, 255, 0));
		}
		cv::imshow("Image", frame);
	}
	cv::waitKey(1);
	return ret;
}

void stopsign_detector::run()
{
	while (true)
	{
		*stopsign_detected_ref = detect();
	}
}
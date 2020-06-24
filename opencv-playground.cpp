#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void getContoursOutline(Mat src, int thickness = 1, int width = 0, int height = 0) {
	Mat gray;
	if (src.channels() == 1) {
		gray = src;
	}
	else {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}

	imshow("orig", gray);
	Canny(gray, gray, 30, 200, 5);
	// gray = ~gray;
	imshow("canny", gray);


	threshold(gray, gray, 200, 255, THRESH_BINARY);
	Mat cont = gray;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(cont, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat res = Mat(gray.rows, gray.cols, CV_8UC1, Scalar(255));
	drawContours(res, contours, -1, Scalar(0), thickness, LINE_AA, hierarchy);

	if (width > 0 || height > 0) {
		double ratio = res.cols / res.rows;
		double w = width > 0 ? width : height * ratio;
		double h = height > 0 ? height : width / ratio;
		resize(res, res, Size(w, h));
	}

	imshow("res", res);
	//waitKey(0);
}

void main()
{
	// const char* fname = "c:\\temp\\textview.png";
	const char* fname = "c:\\temp\\textblurry.png";
	
	bool video = false;

	Mat image;
	image = imread(fname, IMREAD_COLOR); // Read the file
	if (image.empty() && !video) // Check for invalid input
	{
		Mat image(500, 500, CV_8UC3, Scalar(0, 0, 0));
		putText(image, "input image not found", Point(20, 100), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, LINE_AA);
		imshow("err", image);
		waitKey(0);
		return;
	}

	getContoursOutline(image, 2 , image.cols / 2);
	resize(image, image, Size(image.cols / 2, image.cols / 2));
	imshow("orig", image);
	waitKey(0);
}

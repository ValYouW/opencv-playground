#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "utils.h"

using namespace std;
using namespace cv;

void scaleContours(vector<vector<Point>>& cnts, float scaleX, float scaleY)
{
	for (int i = 0; i < cnts.size(); i++) {
		vector<Point>& cnt = cnts[i];
		for (int j = 0; j < cnt.size(); j++) {
			cnt[j] = Point(cnt[j].x * scaleX, cnt[j].y * scaleY);
		}
	}
}

Mat getContoursOutline(Mat src, int thickness, int width, int height, bool resultTransparent) {
	Mat gray;
	if (src.channels() == 1) {
		gray = src.clone();
	}
	else {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}

	// We should not work on large images
	gray = Utils::resizeAspect(gray, 500);

	// prep for contour detection
	Canny(gray, gray, 20, 50, 3);
	threshold(gray, gray, 100, 255, THRESH_BINARY);

	// find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// resize to original size or requested size
	double ratio = (double)src.cols / src.rows;
	double w = width > 0 ? width : (height > 0 ? height * ratio : src.cols);
	double h = height > 0 ? height : (width > 0 ? width / ratio : src.rows);

	scaleContours(contours, w / gray.cols, h / gray.rows);
	int type = resultTransparent ? CV_8UC4 : CV_8UC1;
	Scalar bg = resultTransparent ? Scalar(0) : Scalar(255);
	Scalar fg = resultTransparent ? Scalar(0, 0, 0, 255) : Scalar(0);
	Mat res = Mat(h, w, type, bg);
	drawContours(res, contours, -1, fg, thickness, LINE_AA, hierarchy);

	return res;
}

void encodeDecodePNG() {
	const char* fname = "tmp\\orlando.png";

	ifstream is;
	is.open(fname, ios::binary);

	// get length of file:
	is.seekg(0, ios::end);
	int length = is.tellg();

	// allocate memory:
	vector<char> buffer(length);

	// read data as a block:
	is.seekg(0, ios::beg);
	is.read(buffer.data(), length);
	is.close();

	// decode png
	Mat decoded = imdecode(buffer, IMREAD_COLOR);
	imshow("decoded", decoded);

	// encode & save as png
	int total = decoded.total() * decoded.elemSize();
	vector<uint8_t> localBuffer;
	imencode(".png", decoded, localBuffer);
	ofstream file("tmp\\encoded_png.png", ios::binary);
	file.write((char*)localBuffer.data(), localBuffer.size());
	file.close();

	// show encoded
	Mat encoded = imread("tmp\\encoded_png.png", IMREAD_COLOR);
	imshow("encoded", encoded);

	waitKey(0);
}

void main()
{
	encodeDecodePNG();
	return;
	const char* fname = "tmp\\orlando.png";
	
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

	//imshow("res", res);
	//waitKey(0);
}

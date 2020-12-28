#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat resizeAspect(Mat img, int longSideSize) {
	if (img.empty()) {
		return img;
	}

	Size2f s;
	if (img.cols > img.rows) 
	{
		s = Size2f(longSideSize, ((double)img.rows / img.cols) * longSideSize);
	}
	else 
	{
		s = Size2f(((double)img.cols / img.rows) * longSideSize, longSideSize);
	}

	Mat dst;
	resize(img, dst, s);

	return dst;
}

void writeLinesOnImage(Mat& image, vector<Vec4i> lines, int thickness, Scalar color)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		line(image, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), color, thickness, 8);
	}
}


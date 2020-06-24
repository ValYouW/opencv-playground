#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void main()
{
	const char* fname = "c:\\temp\\textview.png";
	
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

	resize(image, image, Size(), 0.5, 0.5);
	imshow("image", image);
	waitKey(0);
}

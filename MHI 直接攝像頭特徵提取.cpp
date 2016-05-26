#include <time.h>
#include <stdio.h>
#include<iostream>
#include <ctype.h>
#include <opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\core\core.hpp>
#include<opencv2\video\background_segm.hpp>

using namespace cv;
using namespace std;

const double MHI_DURATION = 2;

// ring image buffer
vector<Mat> buf;
int last = 0;

// temporary images
Mat mhi, mask, zplane;

// parameters:
//  img - input video frame
//  dst - resultant motion picture
static void  update_mhi(const Mat& img, Mat& dst, int diff_threshold)
{
	double timestamp = (double)clock() / CLOCKS_PER_SEC; // get current time in seconds
	Size size = img.size();
	int  idx1 = last;

	// allocate images at the beginning or
	// reallocate them if the frame size is changed
	if (mhi.size() != size)
	{
		mhi = Mat::zeros(size, CV_32F);
		zplane = Mat::zeros(size, CV_8U);

		buf[0] = Mat::zeros(size, CV_8U);
		buf[1] = Mat::zeros(size, CV_8U);
	}

	cvtColor(img, buf[last], COLOR_BGR2GRAY); // convert frame to grayscale

	int idx2 = (last + 1) % 2; // index of (last - (N-1))th frame
	last = idx2;

	Mat silh = buf[idx2];
	absdiff(buf[idx1], buf[idx2], silh); // get difference between frames
	threshold(silh, silh, diff_threshold, 1, THRESH_BINARY); // and threshold it
	//threshold(silh , silh , diff_threshold , 1 , THRESH_BINARY_INV );
	updateMotionHistory(silh, mhi, timestamp, MHI_DURATION); // update MHI

	// convert MHI to blue 8u image
	mhi.convertTo(mask, CV_8U, 255. / MHI_DURATION, (MHI_DURATION - timestamp)*255. / MHI_DURATION);

	Mat planes[] = { mask, zplane, zplane };
	merge(planes, 3, dst);

	
}


int main(int argc, char** argv)
{
	VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened())
	{
		printf("Could not initialize video capture\n");
		return 0;
	}
	buf.resize(2);
	Mat image, motion;
	for (;;)
	{
		cap >> image;
		if (image.empty())
			break;

		update_mhi(image, motion, 30);
		imshow("原始影像", image);
		imshow("Motion History Image(運動歷史影像)", motion);
		if (waitKey(10) >= 0)
			break;
	}

	return 0;
}
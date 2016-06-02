#include <time.h>   //  for  clock()   and   CLOCKS_PER_SEC
#include <stdio.h>
#include<iostream>
#include <ctype.h>
#include <opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\core\core.hpp>

using namespace cv;
using namespace std;

/*MHI 簡短版本------>*/
int main(int argc, char** argv)
{
	Mat frame, frame2, dst;

	double timestamp = (double)clock() / CLOCKS_PER_SEC; // I found this in the Opencv/samples  motempl.cpp
	double duration = 30; //same as above , found this value in the opencv/samples/motempl.cpp
	//double timestamp = clock();
	VideoCapture cap(0);
	while (1)
	{
		cap.read(frame);
		cvtColor(frame, frame, CV_BGR2GRAY);
		cap.read(dst);
		cvtColor(dst, dst, CV_BGR2GRAY);
		absdiff(frame, dst, frame2); // 直接相減
		imshow("absolute frame difference", frame2);
		threshold(frame2, frame2, 30, 255, THRESH_BINARY);
		//Mat tst(frameHeight, frameWidth CV_32FC1);
		//Mat.at（int y， int x）
		
		Mat tst(frame2.rows, frame2.cols, CV_32FC1);

		imshow("threshold_MHI", frame2);
		updateMotionHistory(frame2, tst, timestamp, duration);
		/*
		void updateMotionHistory(InputArray silhouette,
		InputOutputArray mhi,    //single-channel, 32-bit floating-point
		double timestamp,
		double duration)
		*/

		waitKey(30);
	}
	return 0;
}
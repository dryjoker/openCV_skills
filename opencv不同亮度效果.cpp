#include <time.h>
#include <stdio.h>
#include<iostream>
#include <ctype.h>
#include <opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\core\core.hpp>

using namespace cv;
using namespace std;

/*��ӮĪG------>*/
int main(int argc, char** argv)
{
	VideoCapture cap;
	cap.open(0);
	Mat image;
	
	if (!cap.isOpened())
	{
		printf("Could not initialize video capture\n");
		return 0;
	}
	Mat img(300, 200, CV_8UC1);
	for (;;)
	{
		cap >> image;
		if (image.empty())
			break;
		namedWindow("Now", 0);
		imshow("Now", image);

		////�ഫ��8��  �վ�G�� 
		image.convertTo(img, CV_8UC1, 255.0/255.0); // �G�פ���
		namedWindow("�G��255.0/255.0",0);
		imshow("�G��255.0/255.0", img);

		image.convertTo(img, CV_8UC1, 255.0 / 500.0); // �ܷt
		namedWindow("�G��255.0 / 500.0", 0);
		imshow("�G��255.0 / 500.0", img);
		
		image.convertTo(img, CV_8UC1, 255.0 / 750.0); // �ܷt
		namedWindow("�G��255.0 / 750.0", 0);
		imshow("�G��255.0 / 750.0", img);

		image.convertTo(img, CV_8UC1, 255.0 / 1000.0); // �ܷt
		namedWindow("�G��255.0 / 1000.0", 0);
		imshow("�G��255.0 / 1000.0", img);


		image.convertTo(img, CV_8UC1, 255.0 / 2000.0); // �ܷt
		namedWindow("�G��255.0 / 2000.0", 0);
		imshow("�G��255.0 / 2000.0", img);

		image.convertTo(img, CV_8UC1, 255.0 / 3000.0); // �ܷt
		namedWindow("�G��255.0 / 3000.0", 0);
		imshow("�G��255.0 / 3000.0", img);


		int key = waitKey(30);

		if (key == 27)
			break;

	}
	return 0;

}
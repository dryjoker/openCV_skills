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
	cap.read(image);//��Ū�@�i�v��
	Mat photo ;
	image.copyTo(photo);//  �@�}�l�N�s�A���e��(��v���}�Ҫ��Ĥ@�iframe)
	
	if (!cap.isOpened())
	{
		printf("Could not initialize video capture\n");
		return 0;
	}
	
	for (;;)
	{
		cap >> image;
		if (image.empty())
			break;
		imshow("Now", image);
		imshow("photo", photo);
		int key = waitKey(30);
		if (key == 27)
			break;
		if (key == 'b')
			image.copyTo(photo);
	}
	return 0;

}

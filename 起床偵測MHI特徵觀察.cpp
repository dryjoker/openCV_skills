//Standard Library
#include <iostream>
#include <stdio.h>
//OpenCV Header
#include <opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\video\background_segm.hpp>
using namespace cv;
using namespace std;
// Kinect for Windows SDK Header
#include <Kinect.h>
//for  clock() �B CLOCKS_PER_SEC
#include <time.h>
int main(int argc, char** argv)
{
	//Mat motion; //  for MHI
	double timestamp = (double)clock() / CLOCKS_PER_SEC; // get current time in seconds �C�L�d�����@��]1�@��^
	IKinectSensor * mySensor = nullptr;
	GetDefaultKinectSensor(&mySensor);   //����P���� 
	mySensor->Open();            //���}�P����


	
	IDepthFrameSource * mySource = nullptr;    //���o�`�׼ƾ�
	mySensor->get_DepthFrameSource(&mySource);

	int height = 0, width = 0;
	IFrameDescription * myDescription = nullptr;   //���o�`�׼ƾڪ�����v
	mySource->get_FrameDescription(&myDescription);
	myDescription->get_Height(&height);
	myDescription->get_Width(&width);
	myDescription->Release();

	IDepthFrameReader * myReader = nullptr;
	mySource->OpenReader(&myReader);     //���}�`�׼ƾڪ�Reader

	IDepthFrame * myFrame = nullptr;
	Mat temp(height, width, CV_16UC1);     //�إ߹Ϲ��x�}
	Mat img(height, width, CV_8UC1);	
	Mat depInfo(height, width, CV_8UC1);

	Size size = img.size();
	Mat frame1,  frameDst;
	Mat frame2 = Mat::zeros(size, CV_8U);

	Mat imgDep(height, width, CV_16SC1);
	char filenameD[999]; // �v���ǦC �s���ɦW 
	unsigned int uFrameCount = 0;  //   �C��run��frame��!

	while (uFrameCount < 900)
	{
		if (myReader->AcquireLatestFrame(&myFrame) == S_OK) //�q�LReader��������̷s���@�V�`�׼ƾڡA��J�`�״V��,�çP�_�O�_���\���
		{
			//�w�� �w�İϸ��������B�z			
			UINT uBufferSize = 0;
			UINT16 * pBuffer = NULL;
			//pBuffer�����qxef���C�ӵe�����h������
			myFrame->AccessUnderlyingBuffer(&uBufferSize, &pBuffer);
			//(*pBuffer) % 256; // 
			imgDep.data = (uchar*)pBuffer;
			imgDep.convertTo(depInfo  , CV_8UC1 , 255.0/ 4500);
			

			myFrame->CopyFrameDataToArray(height * width, (UINT16 *)temp.data); //����ƾڦs�J16�쪺�Ϲ��x�}�� 
			temp.convertTo(img, CV_8UC1, 255.0 / 4500);    //�A��16���ഫ��8�� 
			

			//�Ĥ@�i���ذѦҼv��
			img.copyTo(frame1);
			absdiff(frame2, frame1, frameDst);
			frame1.copyTo(frame2);
			imshow("�۴�᪺���G",frameDst);
			////�G�ȤƼg�k1 ..... ���e�ȳ]128������
			///threshold(frameDst ,frameDst ,128.0 ,255.0 , THRESH_BINARY);
			////�G�ȤƼg�k2  �O�o����cvtColor��gray��q���I���Y�B���g�k
			//frameDst = frameDst > 128;
			threshold(frameDst, frameDst, 5, 255.0, THRESH_BINARY);
			imshow("�۴��A�G�Ȥƪ����G", frameDst);

			//Mat motion(frameDst.rows , frameDst.cols , CV_8SC1 ); //  for MHI
			Mat motion(frameDst.rows, frameDst.cols, CV_32FC1);//  for MHI
			updateMotionHistory(frameDst , motion , timestamp , 50);
			
			imshow("�۴��G�ȤƥN�JMHI�����G", motion);

			//sprintf(filenameD, "depth_%04d.png", uFrameCount);
			//imwrite(filenameD, frameDst);
			
			//absdiff( frame2 , frame1 , );

			imshow("Test" , img); // show 8-bit			
			//sprintf(filenameD, "depth_%04d.png", uFrameCount);
			//imwrite(filenameD , img);
			
			//img = img > 128; // 0 ~ 255
			//threshold(img,img , 128 , 255 , 0);
			
			
			//imshow("�G�ȤƥX�Ӫ����G" , img);
			//updateMotionHistory( img , motion , timestamp, 8);

			//update_mhi(temp, motion, 30);
			//imshow("Motion History Image(�B�ʾ��v�v��)", motion);

			uFrameCount++;
			myFrame->Release();
		}
		

		if (waitKey(30) == VK_ESCAPE)
			break;
	}
	myReader->Release();         //���񤣥Ϊ��ܶq�åB�����P����
	mySource->Release();
	mySensor->Close();
	mySensor->Release();

	return 0;
}
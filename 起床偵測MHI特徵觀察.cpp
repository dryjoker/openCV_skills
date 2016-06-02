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
//for  clock() 、 CLOCKS_PER_SEC
#include <time.h>
int main(int argc, char** argv)
{
	//Mat motion; //  for MHI
	double timestamp = (double)clock() / CLOCKS_PER_SEC; // get current time in seconds 每過千分之一秒（1毫秒）
	IKinectSensor * mySensor = nullptr;
	GetDefaultKinectSensor(&mySensor);   //獲取感應器 
	mySensor->Open();            //打開感應器


	
	IDepthFrameSource * mySource = nullptr;    //取得深度數據
	mySensor->get_DepthFrameSource(&mySource);

	int height = 0, width = 0;
	IFrameDescription * myDescription = nullptr;   //取得深度數據的分辨率
	mySource->get_FrameDescription(&myDescription);
	myDescription->get_Height(&height);
	myDescription->get_Width(&width);
	myDescription->Release();

	IDepthFrameReader * myReader = nullptr;
	mySource->OpenReader(&myReader);     //打開深度數據的Reader

	IDepthFrame * myFrame = nullptr;
	Mat temp(height, width, CV_16UC1);     //建立圖像矩陣
	Mat img(height, width, CV_8UC1);	
	Mat depInfo(height, width, CV_8UC1);

	Size size = img.size();
	Mat frame1,  frameDst;
	Mat frame2 = Mat::zeros(size, CV_8U);

	Mat imgDep(height, width, CV_16SC1);
	char filenameD[999]; // 影像序列 存檔檔名 
	unsigned int uFrameCount = 0;  //   每次run的frame數!

	while (uFrameCount < 900)
	{
		if (myReader->AcquireLatestFrame(&myFrame) == S_OK) //通過Reader嘗試獲取最新的一幀深度數據，放入深度幀中,並判斷是否成功獲取
		{
			//針對 緩衝區資料獲取的處理			
			UINT uBufferSize = 0;
			UINT16 * pBuffer = NULL;
			//pBuffer必須從xef中每個畫面都去抓取資料
			myFrame->AccessUnderlyingBuffer(&uBufferSize, &pBuffer);
			//(*pBuffer) % 256; // 
			imgDep.data = (uchar*)pBuffer;
			imgDep.convertTo(depInfo  , CV_8UC1 , 255.0/ 4500);
			

			myFrame->CopyFrameDataToArray(height * width, (UINT16 *)temp.data); //先把數據存入16位的圖像矩陣中 
			temp.convertTo(img, CV_8UC1, 255.0 / 4500);    //再把16位轉換為8位 
			

			//第一張先建參考影格
			img.copyTo(frame1);
			absdiff(frame2, frame1, frameDst);
			frame1.copyTo(frame2);
			imshow("相減後的結果",frameDst);
			////二值化寫法1 ..... 門檻值設128不明顯
			///threshold(frameDst ,frameDst ,128.0 ,255.0 , THRESH_BINARY);
			////二值化寫法2  記得先用cvtColor轉gray單通有點偷吃步的寫法
			//frameDst = frameDst > 128;
			threshold(frameDst, frameDst, 5, 255.0, THRESH_BINARY);
			imshow("相減後再二值化的結果", frameDst);

			//Mat motion(frameDst.rows , frameDst.cols , CV_8SC1 ); //  for MHI
			Mat motion(frameDst.rows, frameDst.cols, CV_32FC1);//  for MHI
			updateMotionHistory(frameDst , motion , timestamp , 50);
			
			imshow("相減後二值化代入MHI的結果", motion);

			//sprintf(filenameD, "depth_%04d.png", uFrameCount);
			//imwrite(filenameD, frameDst);
			
			//absdiff( frame2 , frame1 , );

			imshow("Test" , img); // show 8-bit			
			//sprintf(filenameD, "depth_%04d.png", uFrameCount);
			//imwrite(filenameD , img);
			
			//img = img > 128; // 0 ~ 255
			//threshold(img,img , 128 , 255 , 0);
			
			
			//imshow("二值化出來的結果" , img);
			//updateMotionHistory( img , motion , timestamp, 8);

			//update_mhi(temp, motion, 30);
			//imshow("Motion History Image(運動歷史影像)", motion);

			uFrameCount++;
			myFrame->Release();
		}
		

		if (waitKey(30) == VK_ESCAPE)
			break;
	}
	myReader->Release();         //釋放不用的變量並且關閉感應器
	mySource->Release();
	mySensor->Close();
	mySensor->Release();

	return 0;
}
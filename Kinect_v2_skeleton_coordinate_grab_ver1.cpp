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

const   string  get_name(int n);    //判斷關節的名字
int main(void)
{
	IKinectSensor   * mySensor = nullptr;
	GetDefaultKinectSensor(&mySensor);
	mySensor->Open();


	int myBodyCount = 0;
	IBodyFrameSource    * myBodySource = nullptr;
	IBodyFrameReader    * myBodyReader = nullptr;
	mySensor->get_BodyFrameSource(&myBodySource);
	myBodySource->OpenReader(&myBodyReader);
	myBodySource->get_BodyCount(&myBodyCount);


	IDepthFrameSource   * myDepthSource = nullptr;
	IDepthFrameReader   * myDepthReader = nullptr;
	mySensor->get_DepthFrameSource(&myDepthSource);
	myDepthSource->OpenReader(&myDepthReader);


	int height = 0, width = 0;
	IFrameDescription   * myDescription = nullptr;;
	myDepthSource->get_FrameDescription(&myDescription);
	myDescription->get_Height(&height);
	myDescription->get_Width(&width);   //以上為準備好深度數據和骨骼數據的Reader

	IBodyFrame  * myBodyFrame = nullptr;
	IDepthFrame * myDepthFrame = nullptr;
	Mat img16(height, width, CV_16UC1); //為顯示深度影像做準備
	Mat img8(height, width, CV_8UC1);

	while (1)
	{
		while (myDepthReader->AcquireLatestFrame(&myDepthFrame) != S_OK);
		myDepthFrame->CopyFrameDataToArray(width * height, (UINT16 *)img16.data);
		img16.convertTo(img8, CV_8UC1, 255.0 / 4500);
		imshow("Depth Img", img8);  //深度圖像的轉換及顯示

		while (myBodyReader->AcquireLatestFrame(&myBodyFrame) != S_OK);

		int myBodyCount = 0;
		IBody   ** bodyArr = nullptr;
		myBodySource->get_BodyCount(&myBodyCount);//取的攝影機捕捉到的人體總數
		bodyArr = new IBody *[myBodyCount];
		for (int i = 0; i < myBodyCount; i++)   //bodyArr的初始化
			bodyArr[i] = nullptr;

		myBodyFrame->GetAndRefreshBodyData(myBodyCount, bodyArr);

		for (int i = 0; i < myBodyCount; i++)   //遍歷6個人
		{
			BOOLEAN     result = false;
			if (bodyArr[i]->get_IsTracked(&result) == S_OK && result)   //判断此人是否被侦测到
			{
				cout << "Body " << i << " tracked!" << endl;

				int count = 0;
				Joint   jointArr[JointType_Count];
				bodyArr[i]->GetJoints(JointType_Count, jointArr);    //獲取此人的關節數據
				for (int j = 0; j < JointType_Count; j++)
				{
					if (jointArr[j].TrackingState != TrackingState_Tracked) //將確定偵測到的關節顯示出來
						continue;
					string  rt = get_name(jointArr[j].JointType);   //獲取關節的名字
					if (rt != "NULL")   //輸出關節信息
					{
						count++;
						cout << "   " << rt << " tracked" << endl;
						if (rt == "脖子")
							cout << "       脖子座標: " << jointArr[j].Position.X << "," << jointArr[j].Position.Y << "," << jointArr[j].Position.Z << endl;
						if (rt == "肩膀中央")
							cout << "       肩膀中央座標: " << jointArr[j].Position.X << "," << jointArr[j].Position.Y << "," << jointArr[j].Position.Z << endl;
						if (rt == "左肩膀")
							cout << "       左肩膀座標: " << jointArr[j].Position.X << "," << jointArr[j].Position.Y << "," << jointArr[j].Position.Z << endl;
						if (rt == "右肩膀")
							cout << "       右肩膀座標: " << jointArr[j].Position.X << "," << jointArr[j].Position.Y << "," << jointArr[j].Position.Z << endl;
						if (rt == "脊椎中央")
							cout << "       脊椎中央座標: " << jointArr[j].Position.X << "," << jointArr[j].Position.Y << "," << jointArr[j].Position.Z << endl;
					}
					
				}
				cout << count << " joints tracked" << endl << endl;
			}
		}
		myDepthFrame->Release();
		myBodyFrame->Release();
		delete[] bodyArr;

		if (waitKey(30) == VK_ESCAPE)
			break;
		//Sleep(1000);    //每秒鐘更新一次, 放慢速度用

	}
	myBodyReader->Release();
	myDepthReader->Release();
	myBodySource->Release();
	myDepthSource->Release();
	mySensor->Close();
	mySensor->Release();

	return  0;
}

const   string  get_name(int n)
{
	switch (n)
	{
		case 2:   
			return "脖子"; 
			break;
		case 20:   
			return  "肩膀中央"; 
			break;
		case 4:   
			return  "左肩膀"; 
			break;
		case 8:   
			return  "右肩膀"; 
			break;
		case 1:
			return  "脊椎中央"; 
			break;

		default:return "NULL";
	}
}
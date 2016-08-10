#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv/cv.h"
#include "opencv/highgui.h"

#include<iostream>
using namespace std;
using namespace cv;

int main()
{
    // Data for visual representation
    int width = 512, height = 512;
    Mat image = Mat::zeros(height, width, CV_8UC3);

    // Set up training data
    float labels[20] = {1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 , 1.0 ,1.0, 1.0, 
						-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
    Mat labelsMat(20, 1, CV_32FC1, labels);
	 
	float trainingData[20][2] = { {501, 10}, {480 , 13} , 
								 {475 , 26} ,{ 488 , 35} , 
								 {500 , 19}, {502 , 48},
								 {495 , 56} , {465 , 29},
								 {275 , 25} , {285 , 36},
								 {255, 10}, {501, 330}, 


								 
								 {10, 501}, {15,189},
								 {25 , 48} , {30 , 100} , 
								 {45 , 119} , {55 , 69},
								 {65 , 90} , {75 , 368}
								   
								};
    Mat trainingDataMat(20, 2, CV_32FC1, trainingData);

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    // Train the SVM
    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);

    Vec3b green(0,255,0), blue (255,0,0);
    // Show the decision regions given by the SVM


	
	
    for (int i = 0; i < image.rows; ++i)
        for (int j = 0; j < image.cols; ++j)
        {
            Mat sampleMat = (Mat_<float>(1,2) << j,i);
            float response = SVM.predict(sampleMat);

            if (response == 1)
                image.at<Vec3b>(i,j)  = green;
            else if (response == -1)
                 image.at<Vec3b>(i,j)  = blue;
        }
	

    // Show the training data
    int thickness = -1;
    int lineType = 8;
	circle(	image, Point( 501, 10), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 480, 13), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 475, 26), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 488, 35), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 500, 19), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 502, 48), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 495, 56), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 465, 29), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 275, 25), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point( 285, 36), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point(255,  10), 5, Scalar(255, 255, 255), thickness, lineType);
	circle(	image, Point(501,330), 5, Scalar(255, 255, 255), thickness, lineType);
	//circle(	image, Point(255,  10), 5, Scalar(0,   0,   0), thickness, lineType);
	//circle(	image, Point(501,330), 5, Scalar(0,   0,   0), thickness, lineType);
	
	circle(	image, Point( 10, 501), 5, Scalar(0,   0,   0), thickness, lineType);
    circle(	image, Point(15,  189), 5, Scalar(  0,   0,   0), thickness, lineType);
    circle(	image, Point(25,  48), 5, Scalar(  0,   0,   0), thickness, lineType);
	circle(	image, Point(30,  100), 5, Scalar(  0,   0,   0), thickness, lineType);
	circle(	image, Point(45,  119), 5, Scalar(  0,   0,   0), thickness, lineType);
	circle(	image, Point(55,  69), 5, Scalar(  0,   0,   0), thickness, lineType);
	circle(	image, Point(65,  90), 5, Scalar(  0,   0,   0), thickness, lineType);
    circle(	image, Point(75,  368), 5, Scalar(  0,   0,   0), thickness, lineType);
    


    // Show support vectors
    thickness = 2;
    lineType  = 8;
    int c     = SVM.get_support_vector_count();

    for (int i = 0; i < c; ++i)
    {
        const float* v = SVM.get_support_vector(i);
        circle(	image,  Point( (int) v[0], (int) v[1]),   6,  Scalar(128, 128, 128), thickness, lineType);
    }

    imwrite("result.png", image);        // save the image

    imshow("SVM Simple Example", image); // show it to the user

	/*
	float test[7] = { 18,22,64,255,188,125,97};
	CvMat test_Matrix = cvMat(4,3,CV_8UC3,test);


	SVM.predict(test_Matrix,);
	*/
	//CvMat* svm_Cvmat_test = cvCreateMat(4, 2, CV_32FC1 , );  

	//cvSetData(&svm_Cvmat_test,test,CV_AUTOSTEP);

	//Mat svm_test(4,2,CV_32FC1,test);

	//第一次測試失敗
	//CvMat * svm_result;

	//第二次測試失敗
	//CvMat * svm_result = cvCreateMat(4,1,CV_32FC1);	
	//const CvMat* svm_Cvmat_test = (const CvMat*)svm_test;

	//SVM.predict(svm_Cvmat_test, svm_result);
	//cout << svm_result << endl ;
    waitKey(0);

}
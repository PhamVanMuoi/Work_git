#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/calib3d/calib3d.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <tuple>
#include <time.h>
#include <math.h>
#include <iostream>
#include <thread>
#include <pthread.h>
#define max 100

using namespace std;
using namespace cv;


void mouseEvent(int evt, int x, int y, int flags, void* param) 
{                    
    Mat* rgb = (Mat*) param;
    if (evt == CV_EVENT_LBUTTONDBLCLK) 
    { 
        printf("%d %d: %d, %d, %d\n", 
        x, y, 
        (int)(*rgb).at<Vec3b>(y, x)[0], 
        (int)(*rgb).at<Vec3b>(y, x)[1], 
        (int)(*rgb).at<Vec3b>(y, x)[2]); 
    }         
}

void matrix (float A[max][max], float B[max][max], float C[max][max], int m, int n, int p)
{
  for ( int i =0; i < m; i++)
    for (int k=0; k < p; k++)
    {
      C[i][k]=0;
      for (int j=0; j< n; j++)
        C[i][k]= C[i][k]+ A[i][j]*B[j][k];
    }

  cout<< C<<endl;
}


int main(){

  namedWindow("Left",WINDOW_NORMAL);
  //namedWindow("Right",WINDOW_NORMAL);
  //resizeWindow("XXX",1920,1080);
  // Mat show1;


  clock_t start = clock();
  Mat img1_L = imread("/media/muoi/Muoi/Work/3DGeometry/xxx/Left.jpg", CV_LOAD_IMAGE_COLOR);
  Mat img1_R = imread("/media/muoi/Muoi/Work/3DGeometry/xxx/Right.jpg", CV_LOAD_IMAGE_COLOR);
  Mat R1, R2, P1, P2, Q;
  Mat K1, K2, R;
  Mat Left_3D, show1;
  Mat lmapx, lmapy, rmapx, rmapy;
  Mat imgU1, imgU1_pose, imgU2, imgLeft, imgRight;

  Vec3d T;
  Mat D1, D2;
  // Read image

  Mat img1 = img1_L;
  Mat img2 = img1_R;

  Size img_size = img1.size();

  cv::FileStorage fs1("/media/muoi/Muoi/Work/3DGeometry/xxx/calibStereo3D_RGB_50cm0.12.yml", cv::FileStorage::READ);
  fs1["K1"] >> K1;
  fs1["K2"] >> K2;
  fs1["D1"] >> D1;
  fs1["D2"] >> D2;
  fs1["R"] >> R;
  fs1["T"] >> T;
  

  stereoRectify(K1, D1, K2, D2, img1.size(), R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 0, img1.size());
 
  cv::initUndistortRectifyMap(K1, D1, R1, P1, img1.size(), CV_32F, lmapx, lmapy);
  cv::initUndistortRectifyMap(K2, D2, R2, P2, img1.size(), CV_32F, rmapx, rmapy);

  cv::remap(img1, imgU1, lmapx, lmapy, cv::INTER_LINEAR);
  cv::remap(img2, imgU2, rmapx, rmapy, cv::INTER_LINEAR);




  imwrite("L_rec.png", imgU1);
  imwrite("R_rec.png", imgU2);

  cvtColor(imgU1,imgLeft,CV_RGB2GRAY);
  cvtColor(imgU2,imgRight,CV_RGB2GRAY);
  Mat imgDisparity16S = Mat( imgLeft.rows, imgLeft.cols, CV_16S );
  Mat imgDisparity8U = Mat( imgLeft.rows, imgLeft.cols, CV_8UC1 );

  Ptr<StereoSGBM> sgbm = StereoSGBM::create();
  sgbm->setMinDisparity(1);
  sgbm->setNumDisparities(16*10);
  sgbm->setBlockSize(15);
  // sgbm->setP1(0); // 8
  sgbm->setP2(512); // 32

  sgbm->compute( imgLeft, imgRight, imgDisparity16S );

  //-- Check its extreme values
  double minVal; double maxVal;

  minMaxLoc( imgDisparity16S, &minVal, &maxVal );

 
  Mat pointcloud;
  

  
  imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));
  reprojectImageTo3D(imgDisparity16S, pointcloud, Q, true);
  float depth_value = imgDisparity16S.at<uint16_t>(232, 492)/ 4000.0f;
  cout << "xxx"<<depth_value<< endl;

  setMouseCallback("Left", mouseEvent, &img1_L );
  
  imshow("Left", img1_L);

  // setMouseCallback("Right", mouseEvent, &imgRight );
  
  // imshow("Right", imgRight);

  imwrite( "Disparity_rec.png", imgDisparity8U );
  ofstream saveFile ("imgDisparity16S.txt");

  saveFile << imgDisparity16S;

  saveFile.close();   
  printf("Time process: %.2fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);

  int co1[] = {1197, 100};
  Mat point1(2, 1, CV_32FC1, co1);
  int co2[] = {1111,510};
  Mat point2(2, 1, CV_32FC1, co2);
  Mat points4D;

 // // cv::triangulatePoints(P1, P2, point1, point2, points4D) ;
 //  cv::triangulatePoints(P1, point1, points4D) ;
 //  cout << "3D"<< points4D <<endl;

  waitKey(0);


  return 0;
}


// int main()
// {
//   clock_t start1 = clock();

//   thread t(worker);
//   //thread t1(worker);
//   cout << "main thread" << endl;
//   t.join();
//   //t1.join();

//   //worker();
//   worker();
//   printf("Sum time process: %.2fs\n", (double)(clock() - start1)/CLOCKS_PER_SEC);
//     return 0;
// }





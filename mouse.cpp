
//#include "opencv2/highgui/highgui.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
 
using namespace std;
using namespace cv;
 
void mouseEvent(int evt, int x, int y, int flags, void* param) 
{                    
    Mat* rgb = (Mat*) param;
    if (evt == CV_EVENT_LBUTTONDOWN) 
    { 
        printf("%d %d: %d, %d, %d\n", 
        x, y, 
        (int)(*rgb).at<Vec3b>(y, x)[0], 
        (int)(*rgb).at<Vec3b>(y, x)[1], 
        (int)(*rgb).at<Vec3b>(y, x)[2]); 
    }         
}
 
int main(int argc, char** argv)
{
    // Read image from file
    Mat img = imread(argv[1]);
 
    //if fail to read the image
    if ( img.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }
 
    //Create a window
    namedWindow("XXX", WINDOW_NORMAL);
    resizeWindow("XXX",1280,720);

 
    //set the callback function for any mouse event
    setMouseCallback("XXX", mouseEvent, &img);
 
    //show the image
    imshow("XXX", img);
 
    // Wait until user press some key
    waitKey(0);
 
    return 0;
}
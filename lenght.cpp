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
#include <math.h>
#include <pthread.h>
#define max 100

using namespace std;
using namespace cv;




int main(int argc, char **argv){

	// z1= 0.5481;
	// z2= 0.5205;
	float z1, z2;
	z1= 0.52;
	z2=0.45;

	float X1, X2, Y1, Y2, Z1, Z2;
	int x1, y1, x2, y2;
	x1 =atoi(argv[1]) ;
	y1 = atoi(argv[2]) ;

	x2 = atoi(argv[3]) ;
	y2 = atoi(argv[4]) ;

	float c_x, c_y, f_x, f_y,c;
	
	c_x = 655.896650;
	c_y = 352.157631;
	f_x = 969.514395;
	f_y = 968.154965;


  	X1 = (x1 - c_x)*z1/f_x;
	Y1= (y1- c_y)*z1/f_y;
	Z1=z1;

	X2 = (x2 - c_x)*z2/f_x;
	Y2= (y2- c_y)*z2/f_y;
	Z2=z2;
	float A= (X1-X2);
 	float B = (Y1- Y2);
 	float C= (Z1-Z2);

 	float lenght= sqrt(A*A + B*B +C*C);
 	cout <<"lenght:\t" << lenght << "\t(met)" <<endl;



  	waitKey(0);


  	return 0;

}
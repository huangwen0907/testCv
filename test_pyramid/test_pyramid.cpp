#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argv, char* argc) 
{

    int levels =1;
    Mat img = imread("data/img.jpg");
    namedWindow("original");

    imshow("original",img);


    Mat currImg = img;

    Mat lap = currImg;
    
    Mat down_py, up_py;
    for(int l=0;l < levels; l++) {
        Mat down, up;
        pyrDown(currImg,down);
        down_py = down; // store the down jpg
        pyrUp(down,up,currImg.size());
        up_py = up; // store the up img
        lap = currImg - up;
        currImg = down;
   
    }

    namedWindow("Laplace pyramid");
    imshow("Laplace pyramid",lap);
    imshow("up_py",up_py);
    imshow("down_py",down_py);

    waitKey(0);
    return 0;
}
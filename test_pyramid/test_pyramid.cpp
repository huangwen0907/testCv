#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

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
    
    // detect the feature and descriptor Through ORB
    Ptr<ORB> orb = ORB::create ( 900, 1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE,31,20 );
    Mat descriptor;
    std::vector<KeyPoint> keypoints;



    //check the keypoints from the img
    orb->detect(currImg,keypoints);
    // compute the descriptor from the img of current
    orb->compute(currImg,keypoints,descriptor);

        // plot the keypoints and descriptor
    Mat outstanding;
    drawKeypoints(currImg,keypoints,outstanding,Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    // drawKeypoints(currImg,keypoints,outstanding);
    imshow("keypoints",outstanding);

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
    // imshow("up_py",up_py);
    // imshow("down_py",down_py);

    waitKey(0);
    return 0;
}
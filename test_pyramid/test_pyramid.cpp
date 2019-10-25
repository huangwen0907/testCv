#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

int main(int argv, char** argc)
{

    int levels =1;
    Mat img = imread("data/img.jpg");
    // labelImg.ptr<int>(i-1);
    int row = 1;
    int col = 1;
    //1. This is used for read the grayscale image
    cout << "fuck img: " << (int)(*(img.data + img.step[0] * row + img.step[1] * col)) << endl;
    //2. This is used for read the RGB image
    cout << "fuck rgb" << img.at<Vec3b>(row,col) << endl;

    // 3.This is used for read the BGR image through the channel
    // [row, col]像素的第 1 通道地址被 * 解析(blue通道)
    // *(img.data + img.step[0] * row + img.step[1] * col) = 255;
    // //[row, col]像素的第 2 通道地址被 * 解析(green通道), 关于elemSize1函数的更多描述请见 Fn1 里所列的博文链接
    // *(img.data + img.step[0] * row + img.step[1] * col + img.elemSize1()) = 255;
    // //[row, col]像素的第 3 通道地址被 * 解析(red通道)
    // *(img.data + img.step[0] * row + img.step[1] * col + img.elemSize1() * 2) = 255;

    // 4.
    uchar* data = img.ptr<uchar>(row);
    cout << "4 fuck img bgr: " << (int)data[col*3] << " : " << (int)data[col * 3 + 1] << " : " << (int)data[col *3 + 2] << endl;

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
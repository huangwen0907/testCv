#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

/**
 * @函数 CannyThreshold
 * @简介： trackbar 交互回调 - Canny阈值输入比例1:3
 */
void CannyThreshold(int, void*)
{
    /// 使用 3x3内核降噪
    blur( src_gray, detected_edges, Size(3,3) );
    imshow("detected edges",detected_edges);

    /// 运行Canny算子
    //! applies Canny edge detector and produces the edge map.
    // CV_EXPORTS_W void Canny( InputArray image, OutputArray edges,
    //                         double threshold1, double threshold2,
    //                         int apertureSize=3, bool L2gradient=false );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

    /// 使用 Canny算子输出边缘作为掩码显示原图像
    dst = Scalar::all(0);

    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
 }


/** @函数 main */
int main( int argc, char** argv )
{
    /// 装载图像
    src = imread("data/img4.jpg");

    if( !src.data )
    { return -1; }

    /// 创建与src同类型和大小的矩阵(dst)
    dst.create( src.size(), src.type() );

    /// 原图像转换为灰度图像
    cvtColor( src, src_gray, CV_BGR2GRAY );

    /// 创建显示窗口
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    imshow("raw image",src);
    cout << "Src size:" << src.rows << "cols: " << src.cols << endl;

    /// 创建trackbar
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

    /// 显示图像
    CannyThreshold(0, 0);

    /// 等待用户反应
    waitKey(0);

    return 0;
  }

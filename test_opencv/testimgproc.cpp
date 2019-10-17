#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;



int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;
Mat src;

// Threshold Operation
char * windName = "Threshold";
int maxType = 4;
double maxValue = 255;
int thresholdType = 2;
int thresholdValue = 122;

/** Function Headers */
void Erosion( int, void* );
void Dilation( int, void* );
void ThresholdDemo(int, void*);

int main(int argc, char* argv)
{
        /// Load 图像
    src = imread("data/img3.jpg");
    imread("data/img.jpg");

    if( !src.data ){
        cout << "fuck , no valid image" << endl;
        return -1;
    }

    /// 创建显示窗口
    namedWindow( "Erosion Demo", CV_WINDOW_AUTOSIZE );
    namedWindow( "Dilation Demo", CV_WINDOW_AUTOSIZE );
    cvMoveWindow( "Dilation Demo", src.cols, 0 );
    cvMoveWindow( "Dilation Demo", src.rows, 0 );

    /// 创建腐蚀 Trackbar
    // int createTrackbar(const string& trackbarname, const string& winname,
    //                           int* value, int count,
    //                           TrackbarCallback onChange = 0,
    //                           void* userdata = 0);
    createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
                    &erosion_elem, max_elem,
                    Erosion );

    createTrackbar( "Kernel size:\n 2n +1", "Erosion Demo",
                    &erosion_size, max_kernel_size,
                    Erosion );

    /// 创建膨胀 Trackbar
    createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
                    &dilation_elem, max_elem,
                    Dilation );

    createTrackbar( "Kernel size:\n 2n +1", "Dilation Demo",
                    &dilation_size, max_kernel_size,
                    Dilation );

    // 阈值操作

    // int createTrackbar(const string& trackbarname, const string& winname,
    //                             int* value, int count,
    //                             TrackbarCallback onChange = 0,
    //                             void* userdata = 0);
    namedWindow(windName,CV_WINDOW_AUTOSIZE);
    createTrackbar("Type:\n 0:binary \n 1:BInar Inverted \n 2:Truncate \n 3:To zero \n 4:To Zero Inverted",
                    windName,&thresholdType,maxType,ThresholdDemo);
    createTrackbar("Track Value",
                windName,&thresholdValue,maxValue,ThresholdDemo);

    /// Default start
    Erosion( 0, 0 );
    Dilation( 0, 0 );

    waitKey(0);
    return 0;
}

void ThresholdDemo(int, void*)
{
    Mat dstImg;
    threshold(src,dstImg,thresholdValue,255,thresholdType);
    imshow(windName,dstImg);
}

/**  @function Erosion  */
void Erosion( int, void* )
{
    int erosion_type;
    // 矩形: MORPH_RECT
    // 交叉形: MORPH_CROSS
    // 椭圆形: MORPH_ELLIPSE
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    //Mat getStructuringElement(int shape, Size ksize, Point anchor=Point(-1,-1));
    Mat element = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ));

    /// 腐蚀操作
    //  erodes the image (applies the local minimum operator)
    // void erode( InputArray src, OutputArray dst, InputArray kernel,
    //                      Point anchor=Point(-1,-1), int iterations=1,
    //                      int borderType=BORDER_CONSTANT,
    //                      const Scalar& borderValue=morphologyDefaultBorderValue() );
    Mat erosion_dst;
    erode( src, erosion_dst, element );
    imshow( "Erosion Demo", erosion_dst );
}

/** @function Dilation */
void Dilation( int, void* )
{
    int dilation_type;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement( dilation_type,
                                        Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                        Point( dilation_size, dilation_size ) );
    ///膨胀操作
    Mat dilation_dst;
    dilate( src, dilation_dst, element );
    imshow( "Dilation Demo", dilation_dst );
}

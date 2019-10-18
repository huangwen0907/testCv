#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void cornerHarrisDemo(int,void*);
void shiTomasiDemo(int,void*);

// Harris
char *sourceWindow = "feature detected";
int threshValue = 200;
int maxThreshValue = 255;

// Shi-Tomasi
RNG rng(12345);
int ShiConers = 23;
int maxShiTrackBAR = 100;

Mat src, srcGray;
int main(int argc,char** argv)
{

    src = imread("data/img4.jpg");
    if (src.empty()) {
        cout << "fuck, not a valid image" << endl;
        return -1;
    }

    cvtColor(src,srcGray,CV_BGR2GRAY);
    namedWindow(sourceWindow,CV_WINDOW_AUTOSIZE);
    cvMoveWindow( sourceWindow, src.rows, 0 );
    cvMoveWindow( sourceWindow, src.cols, 0 );
    createTrackbar("Harris Threshold:",sourceWindow,&threshValue,maxThreshValue,cornerHarrisDemo);
    createTrackbar("Shi-Tomas Threshold:",sourceWindow,&ShiConers,maxShiTrackBAR,shiTomasiDemo);

    imshow(sourceWindow,src);
    cornerHarrisDemo(0,0);
    shiTomasiDemo(0,0);
    waitKey(0);
    return 0;
}

void cornerHarrisDemo(int,void*)
{
    Mat dst, dst_norm,dst_norm_scaled;
    dst = Mat::zeros(src.size(),CV_32FC1);

    int blockSize = 2;
    int appertureSize = 3;
    double k = 0.04;

    //! computes Harris cornerness criteria at each image pixel
    // void cornerHarris( InputArray src, OutputArray dst, int blockSize,
    //                                 int ksize, double k,
    //                                 int borderType=BORDER_DEFAULT );
    cornerHarris(srcGray,dst,blockSize,appertureSize,k,BORDER_DEFAULT);

    // Normalizing
    normalize(dst,dst_norm,0,255,NORM_MINMAX,CV_32FC1,Mat());
    // ! scales array elements, computes absolute values and converts the
    // results to 8-bit unsigned integers: dst(i)=saturate_cast<uchar>abs(src(i)*alpha+beta)
    convertScaleAbs(dst_norm,dst_norm_scaled);

    // drawing a circle around circle
    for(int j = 0;j<dst_norm_scaled.rows;j++) {
        for(int i =0; i < dst_norm_scaled.cols;i++) {
            if ((int)dst_norm.at<float>(j,i) > threshValue) {
                // void circle(CV_IN_OUT Mat& img, Point center, int radius,
                //        const Scalar& color, int thickness=1,
                //        int lineType=8, int shift=0);
                circle(dst_norm_scaled,Point(i,j),5,Scalar(0),2,8,0);
            }
        }
    }

    namedWindow("corners",CV_WINDOW_AUTOSIZE);
    imshow("corners",dst_norm_scaled);

}


void shiTomasiDemo(int,void*)
{
    if (ShiConers < 1) {
        ShiConers = 1;
    }

    /// Parameters for Shi-Tomasi algorithm
    vector<Point2f> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3;
    bool userHarrisDetector = false;
    double k = 0.04;

    Mat copy;
    copy = src.clone();

    //! finds the strong enough corners where the cornerMinEigenVal() or cornerHarris() report the local maxima
    // void goodFeaturesToTrack( InputArray image, OutputArray corners,
    //                                      int maxCorners, double qualityLevel, double minDistance,
    //                                      InputArray mask=noArray(), int blockSize=3,
    //                                      bool useHarrisDetector=false, double k=0.04 );
    goodFeaturesToTrack(srcGray,corners,ShiConers,qualityLevel,minDistance,Mat(),blockSize,userHarrisDetector,k);

    cout<<"** Number of corners detected: "<<corners.size()<<endl;

    for (int i = 0; i < corners.size();i++) {
        circle(copy,corners[i],4,Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)),2,8,0);
    }

    namedWindow("fuck ShiT-Tomas",CV_WINDOW_AUTOSIZE);
    imshow("Coners Shi-Tomas",copy);
}

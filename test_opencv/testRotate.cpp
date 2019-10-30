#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "iostream"

using namespace cv;
using namespace std;

void rotateImage(Mat &src,Mat &dst,float angle);

int main(int argc, char** argv)
{
    // rotateImage();
    Mat img1 = imread("data/img.jpg");
    Mat img1_rotate;
    float angle = 30;
    rotateImage(img1,img1_rotate,angle);
    waitKey();
    return 0;
}

void rotateImage(Mat &src,Mat &dst,float angle)
{
    float radian = (float)(angle/180 * CV_PI);


    /*fill the image*/

    //! copies 2D array to a larger destination array with extrapolation of the outer part of src using the specified border mode
    // CV_EXPORTS_W void copyMakeBorder( InputArray src, OutputArray dst,
    //                                 int top, int bottom, int left, int right,
    //                                 int borderType, const Scalar& value=Scalar() );
    int maxborder = (int)(max(src.cols,src.rows)*1.414);
    int dx = (maxborder - src.cols) * 0.5;
    int dy = (maxborder - src.rows) * 0.5;

    copyMakeBorder(src,dst,dy,dy,dx,dx,BORDER_CONSTANT);

    /*rotate the image**/

    Point2f center((float)dst.cols*0.5,(float)dst.rows*0.5);
    //means 1;
    Mat affine = getRotationMatrix2D(center,angle,1);

    float sinVal = abs(sin(radian));
    float cosVal = abs(cos(radian));

    //means 2;
    // float a[2][3] = {cosVal,sinVal,dst.cols*0.5,sinVal,-cosVal,dst.rows*0.5};
    // Mat affine(2,3,CV_32F,a);

    // means 3;
    // Mat affine = (Mat_<double>(2,3) << cosVal,sinVal,dst.cols*0.5,sinVal,-cosVal,dst.rows*0.5);

    cout << "fuck affine: " << endl << affine << endl;
    //! warps the image using affine transformation
    warpAffine(dst,dst,affine,dst.size());

    imshow("fuck 1 dst",dst);

    // resize the image, cut down the border

    Size targetSize((int)(src.cols*cosVal+src.rows*sinVal),(int)(src.cols*sinVal+src.rows*cosVal));

    int x = (dst.cols - targetSize.width)*0.5;
    int y = (dst.rows -targetSize.height)*0.5;
    Rect rect(x,y,targetSize.width,targetSize.height);
    dst = Mat(dst,rect);

    imshow("fuck 2 dst",dst);
}

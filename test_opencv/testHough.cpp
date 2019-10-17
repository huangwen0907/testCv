#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv)
{

    Mat src = imread("data/img4.jpg", 0);
    if(src.empty())
    {
        cout << "Fuck: Not a valid image" << endl;
        return -1;
    }

    Mat dst, cdst;
    Canny(src, dst, 50, 200, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);

    #if 1
        vector<Vec2f> lines;

        //! finds lines in the black-n-white image using the standard or pyramid Hough transform
        // CV_EXPORTS_W void HoughLines( InputArray image, OutputArray lines,
        //                               double rho, double theta, int threshold,
        //                               double srn=0, double stn=0 );
        HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }
    #else
        vector<Vec4i> lines;
        //! finds line segments in the black-n-white image using probabilistic Hough transform
        // CV_EXPORTS_W void HoughLinesP( InputArray image, OutputArray lines,
        //                                double rho, double theta, int threshold,
        //                                double minLineLength=0, double maxLineGap=0 );
        HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            Vec4i l = lines[i];
            line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        }
    #endif
        imshow("source", src);
        imshow("detected lines", cdst);

    waitKey();

    return 0;
}

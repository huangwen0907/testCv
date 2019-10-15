#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

/*
    mix the image g(x) = (1-a)f1(x) + f2 (x)

*/
int main(int argc,char* argv)
{
    double alpha = 0.5;
    double beta; double input;

      /// Ask the user enter alpha
    std::cout<<" Simple Linear Blender "<<std::endl;
    std::cout<<"-----------------------"<<std::endl;
    std::cout<<"* Enter alpha [0-1]: ";
    std::cin>>input;

    if (input < 0 || input >1) {
        cout <<"fuck you, please give valid variable between 0 and 1" << endl;
        // return -1;
    } else
    {
         alpha = input;
    }

    // judge the valid img
    Mat img1, img2,mixImg;
    img1 = imread("data/img.jpg");
    img2 = imread("data/img2.jpg");
    // change the img2's size to match the img1
    //void resize(InputArray src, OutputArray dst, Size dsize, double fx=0, double fy=0, int interpolation=INTER_LINEAR );
    resize(img2,img2,img1.size());

    if (!img1.data || !img2.data) {
        cout << "fuck , please put the correct image" << endl;
        return -1;
    }

    namedWindow("image");
    beta = 1 -alpha;
    addWeighted(img1,alpha,img2,beta,0.0,mixImg);

    imshow("mixImg",mixImg);
    waitKey(0);
    return 0;
}

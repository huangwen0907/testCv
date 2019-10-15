#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argv, char* argc)
{
    {
    //Mat() 构造函数,在 C\C++ 中通过构造函数进行初始化
    Mat M(2,2,CV_8UC3,Scalar(0,0,255));
    cout << "M:" << endl << M <<endl;
    }

    {
    //在 C\C++ 中通过构造函数进行初始化
    //如何创建一个超过两维的矩阵：指定维数，然后传递一个指向一个数组的指针，这个数组包含每个维度的尺寸；其余的相同
    int sz[3] = {2,2,2};
    Mat L(3,sz,CV_8UC(1),Scalar::all(0));
    }


    {
        // read the image
        Mat img = imread("data/img.jpg");
        // modify the image

        if (!img.data) {
            cout << "No valid image, please get the correct address" << endl;
            return -1;
        }
        imshow("raw image",img);

        Mat grayImg;
        cvtColor(img,grayImg,CV_BGR2GRAY);
        // write a new image
        imwrite("data/imggray.jpg",grayImg);
        imshow("gray img",grayImg);
    }

    {
        //  zeros(), ones(), :eyes()
        Mat EYE = Mat::eye(4,4,CV_64F);
        Mat ZERO = Mat::zeros(2,2,CV_32F);
        Mat ONE = Mat::ones(3,3,CV_8UC1);
        cout << "EYE:" <<endl << EYE <<endl;
        cout << "ZERO:" <<endl << ZERO <<endl;
        cout << "ONE:" <<endl << ONE <<endl;
    }

    {
        // clone & copy
        Mat data = (Mat_<double>(3,3) <<1,2,3,4,5,6,7,8,9);
        cout << "data:" <<endl << data <<endl;

        Mat dataClone = data.clone();
        Mat dataCloneRow = data.col(1).clone();
        cout << "dataClone:" <<endl << dataClone <<endl;
        cout << "dataCloneRow:" <<endl << dataCloneRow <<endl;

    }

    waitKey(0);
    return  0;
}

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
        namedWindow("fuck img");
        imshow("raw image",img);

        Mat grayImg;
        cvtColor(img,grayImg,CV_BGR2GRAY);
        // write a new image
        imwrite("data/imggray.jpg",grayImg);
        imshow("gray img",grayImg);

        // copy a rectangle picture
        Mat imgRect;
        imgRect.create(img.rows/2,img.cols/2,CV_8UC3);
        // img.copyTo(imgRect);
        // pyrDown the image (pyramid process)
        pyrDown(img,imgRect);
        imshow("Rectangle",imgRect);


        // mat mask operation
        // reference website:
        // http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/mat-mask-operations/mat-mask-operations.html#maskoperationsfilter
        Mat sharPen;
        const int nChannels = img.channels();
        cout <<"Img channels:" << nChannels << endl;

        //使用掩码矩阵的时候，我们先把矩阵中心的元素（上面的例子中是(0,0)位置的元素，也就是5
        //对齐到要计算的目标像素上，再把邻域像素值和相应的矩阵元素值的乘积加起来。
        // I(i,j) = 5*I(i,j) - {I(i-1,j)+I(i+1,j)+I(i,j-1)+I(i,j+1)} // sum the nearest 5 points value and then get the mean value
        sharPen.create(img.size(),img.type());
        for (int j =1;j< img.rows-1;++j) {
            const uchar* pre = img.ptr<uchar>(j-1);
            const uchar* cur = img.ptr<uchar>(j);
            const uchar* next = img.ptr<uchar>(j+1);

            uchar* output = sharPen.ptr<uchar>(j);

            for (int i = nChannels;i<nChannels*(img.rows-1);++i) {
                *output++ = saturate_cast<uchar>(5*cur[i]-cur[i-nChannels]-cur[i+nChannels]-pre[i]-next[i]);
            }
        }

        //在图像的边界上，上面给出的公式会访问不存在的像素位置（比如(0,-1)）。
        //因此我们的公式对边界点来说是未定义的。一种简单的解决方法，是不对这些边界点使用掩码，而直接把它们设为0：
        sharPen.row(0).setTo(Scalar(0));
        sharPen.row(sharPen.rows-1).setTo(Scalar(0));
        sharPen.col(0).setTo(Scalar(0));
        sharPen.col(sharPen.cols-1).setTo(Scalar(0));

        imshow("sharpen",sharPen);

        Mat kern = (Mat_<char>(3,3) <<  0, -1, 0,
                                   -1,  5, -1,
                                    0, -1,  0);
        Mat sharpenK;
        //滤波器在图像处理中的应用
        filter2D(img,sharpenK,img.depth(),kern);

        imshow("sharpenK",sharpenK);

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

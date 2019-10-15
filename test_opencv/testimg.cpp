#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

/*
    1: mix the image g(x) = (1-a)f1(x) + f2 (x)
    2: change the brightness about the image: g(x) = a*f(x) + b
        a & b is used as the gain and bias paramters
*/
int main(int argc,char* argv)
{
    double caseValue;
    /// Ask the user case value
    std::cout<<" Simple case value "<<std::endl;
    std::cout<<"-----------------------"<<std::endl;
    std::cout<<"* Enter 1:mix image, 2:modify the brightness "<< endl;
    std::cin >> caseValue;

    if (caseValue == 1) {
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
    } else if (caseValue == 2) {
        Mat img;
        img = imread("data/img.jpg");
        double alpha, beta;
        cout << "fuck input the gain alpha[0,3]:";
        cin >> alpha;
        cout << "fuck input the beta bias[0,100]:";
        cin >> beta;
        if (!img.data) {
            cout << "fuck unvalid image" << endl;
            return -1;
        }
        // if not initialize the newImg's size, it will crash when the code is runing
        Mat newImg = Mat::zeros(img.size(),img.type());

        cout << "fuck alpha & beta:" << alpha <<"  :  "<< beta << endl;
        for(int y=0;y < img.rows-1;y++) {
            for(int x=0;x<img.cols-1;x++) {
                for(int k=0;k<3;k++) {
                    //g=af+b运算结果可能超出像素取值范围，还可能是非整数（如果 \alpha 是浮点数的话），所以我们要用 saturate_cast 对结果进行转换，以确保它为有效值。
                    newImg.at<Vec3b>(y,x)[k] = saturate_cast<uchar>(alpha * (img.at<Vec3b>(y,x)[k]) + beta);
                }
            }
        }
        namedWindow("brightness");
        imshow("newImg",newImg);

    } else {
        cout << "fuck why you can not input a correct case value" << endl;
        return -1;
    }

    waitKey(0);
    return 0;
}

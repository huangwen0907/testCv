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
    3: modify the image by DFT(discrete fourier transform)
*/
int main(int argc,char* argv)
{
    double caseValue;
    /// Ask the user case value
    std::cout<<" Simple case value "<<std::endl;
    std::cout<<"-----------------------"<<std::endl;
    std::cout<<"* Enter 1:mix image, 2:modify the brightness 3:modify the image by DFT 4: Filter the Image"<< endl;
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

    } else if (caseValue == 3) {
        // do the discrete fourier transform
        // must read the gray image,if not, it will crash in below line:
        // Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
        Mat img = imread("data/img.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        if(img.empty()) {
            cout << "fuck , why not get a valid image" << endl;
            return -1;
        }

        Mat padded; // expand the input image to optimal size
        int m = getOptimalDFTSize(img.rows);
        int n = getOptimalDFTSize(img.cols);
        // on the border add zero values
        //copies 2D array to a larger destination array with extrapolation of the outer part of src using the specified border mode
        //oid copyMakeBorder( InputArray src, OutputArray dst,
                        // int top, int bottom, int left, int right,
                        // int borderType, const Scalar& value=Scalar() );
        copyMakeBorder(img,padded,0,m-img.rows,0,n-img.cols,BORDER_CONSTANT,Scalar::all(0));

        Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};

        Mat complexImg;

        // Add to the expanded another plane with zeros
        // OutputIterator merge(const SinglePassRange1& rng1,const SinglePassRange2& rng2, OutputIterator          out)
        merge(planes,2,complexImg);

        // This way the result may fit in the source matrix
        // performs forward or inverse 1D or 2D Discrete Fourier Transformation
        // CV_EXPORTS_W void dft(InputArray src, OutputArray dst, int flags=0, int nonzeroRows=0);
        dft(complexImg,complexImg);

        // compute the magnitude and switch to logarithmic scale
        // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
        // void split(const Mat& src, vector<Mat_<_Tp> >& mv)
        split(complexImg,planes); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

        //! computes magnitude (magnitude(i)) of each (x(i), y(i)) vector
        // CV_EXPORTS_W void magnitude(InputArray x, InputArray y, OutputArray magnitude);
        magnitude(planes[0],planes[1],planes[0]); //planes[0] = magnitude

        Mat magPlanes = planes[0];
        magPlanes += Scalar::all(1); // switch to logarithmic scale

        log(magPlanes,magPlanes);

        // crop the spectrum, if it has an odd number of rows or columns
        magPlanes = magPlanes(Rect(0,0,magPlanes.cols&-2, magPlanes.rows& -2));

        // rearrange the quadrants of Fourier image so that the origin is at the image center
        int cx = magPlanes.cols/2;
        int cy = magPlanes.rows/2;

        Mat q0(magPlanes,Rect(0,0,cx,cy)); // top-left
        Mat q1(magPlanes,Rect(cx,0,cx,cy)); // top-right
        Mat q2(magPlanes,Rect(0,cy,cx,cy)); // bottom-left
        Mat q3(magPlanes,Rect(cx,cy,cx,cy)); // bottom-right

        // switch the q0<--> q3
        Mat tmp;
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);


        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);

        normalize(magPlanes,magPlanes,0,1,CV_MINMAX);

        namedWindow("DFT");

        imshow("input Image",img);
        imshow("spectrum magnitude",magPlanes);
    } else if (caseValue == 4) {
        Mat src, dst;
        Mat kernel;
        Point anchor;
        double delta;
        int ddepth;
        int kernelSize;
        char* windNmae = "Filter2D Demo";

        int c;
        src = imread("data/img3.jpg",CV_LOAD_IMAGE_UNCHANGED);
        if(src.empty()) {
            cout << "fuck, get a valid input image" << endl;
        }

        namedWindow(windNmae,CV_WINDOW_AUTOSIZE);

        // Point(-1, -1): 指定锚点位置(被平滑点)， 如果是负值，取核的中心为锚点。
        anchor = Point(-1,-1);
        delta = 0;
        ddepth = -1;

        int ind = 0;

        while (true)
        {
            c = waitKey(1000);
            if((char)c == 'c') {
                break;
            }
            kernelSize = 3+2*(ind%5);
            kernel = Mat::ones(kernelSize,kernelSize,CV_32F)/(float)(kernelSize * kernelSize);

            filter2D(src,dst,ddepth,kernel,anchor,delta,BORDER_DEFAULT);
            imshow(windNmae,dst);
            ind ++;
        }

    } else {
        cout << "fuck why you can not input a correct case value" << endl;
        return -1;
    }

    waitKey(0);
    return 0;
}

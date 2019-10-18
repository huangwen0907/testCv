#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat img1,img2;
    img1= imread("data/img5.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    img2= imread("data/img6.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    resize(img1,img2,img1.size());

    // detect the keypoint
    // cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("");
    cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    std::vector<KeyPoint> keypoint_1,keypoint_2;

    detector->detect(img1,keypoint_1);
    detector->detect(img2,keypoint_2);

    // calculate the descriptors
    Mat descriptor_1,descriptor_2;
    cv::Ptr<cv::DescriptorExtractor> descriptor = cv::ORB::create();
    descriptor->compute(img1,keypoint_1,descriptor_1);
    descriptor->compute(img2,keypoint_2,descriptor_2);

    // matching descriptor
    // NOTE; 'BruteForce-Hamming' can not be changed to other name
    vector <DMatch> matches;
    cv::Ptr<DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptor_1,descriptor_2,matches);

    Mat matchMap;
    drawMatches(img1,keypoint_1,img2,keypoint_2,matches,matchMap,Scalar::all(-1));

    pyrDown(matchMap,matchMap);
    imshow("match map",matchMap);

    cout << "fuck matches: " << matches.size() << endl;

    waitKey(0);
    return 0;
}

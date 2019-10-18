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

    cout << "fuck matches: " << matches.size() << endl;

    // Quick calculation of max and min distances between keypoints
    Mat optimizeMap;
    // optimizeMap = matchMap.clone();
    double max_dist = 0, min_dist = 100;
    for (int i = 0; i < descriptor_1.rows;i++) {
        double dist = matches[i].distance;
        cout << "fuck dist: " << matches[i].distance << endl;
        if (dist > max_dist) max_dist = dist;
        if (dist < min_dist) min_dist = dist;
    }
    cout << "fuck max dist: " << max_dist << "  min dist: " << min_dist << endl;

    // store the good matches
    std::vector<DMatch> goodMatches;
    for (int i = 0;i<descriptor_1.rows;i++) {
        if (matches[i].distance < 2 * min_dist) {
            goodMatches.push_back(matches[i]);
        }
    }


    Mat matchMap;
    drawMatches(img1,keypoint_1,img2,keypoint_2,matches,matchMap,Scalar::all(-1));

    // pyrDown(matchMap,matchMap);
    imshow("match map",matchMap);

    drawMatches(img1,keypoint_1,img2,keypoint_2,goodMatches,optimizeMap,Scalar::all(-1));
    imshow("goodMatches map",optimizeMap);

    waitKey(0);
    return 0;
}

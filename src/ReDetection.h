#ifndef REDETECTION_H
#define REDETECTION_H

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
class ReDetection
{
public:
    ReDetection();
    double myCompareHist(InputArray img1, InputArray img2, int method=1);
    double myCompareHist(InputArray img1,const cv::Mat& mask1,InputArray img2,const cv::Mat& mask2,int method=1);
};

#endif // REDETECTION_H

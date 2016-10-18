#ifndef REDETECTION_H
#define REDETECTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

class ReDetection
{
public:
    ReDetection();
    double myCompareHist(InputArray img1, InputArray img2, int method=1);
    double myCompareHist(InputArray img1,const cv::Mat& mask1,InputArray img2,const cv::Mat& mask2,int method=1);
    double myCompareLocalFeature(InputArray img1, InputArray img2, int minHessian=400, double maxDist=0.2);
    double myCompareLocalFeature(InputArray img1,const cv::Mat& mask1,InputArray img2,const cv::Mat& mask2,int minHessian=400, double maxDist=0.2);
    //http://study.marearts.com/2015/02/opencv-hog-descriptor-computation-and.html
    //http://study.marearts.com/2014/04/example-source-code-of-extract-hog.html
    double myCompareHog(InputArray img1,InputArray img2);
    double myCompareColor(InputArray img1,InputArray img2);
};

#endif // REDETECTION_H

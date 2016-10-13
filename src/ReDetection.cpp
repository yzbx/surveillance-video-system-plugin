#include "ReDetection.h"

ReDetection::ReDetection()
{

}

double ReDetection::myCompareHist(InputArray img1, InputArray img2, int method)
{
    cv::Mat src,hsv;
    MatND hist1,hist2;

    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };

    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    src=img1.getMat();
    cvtColor(src, hsv, CV_BGR2HSV);
    calcHist( &hsv, 1, channels, Mat(), // do not use mask
              hist1, 2, histSize, ranges,
              true, // the histogram is uniform
              false );

    src=img2.getMat();
    cvtColor(src, hsv, CV_BGR2HSV);
    calcHist( &hsv, 1, channels, Mat(), // do not use mask
              hist2, 2, histSize, ranges,
              true, // the histogram is uniform
              false );

    normalize( hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat() );
    normalize( hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat() );
    return compareHist(hist1,hist2,method);
}

double ReDetection::myCompareHist(InputArray img1, const Mat &mask1, InputArray img2, const Mat &mask2, int method)
{
    cv::Mat src,hsv;
    MatND hist1,hist2;

    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };

    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    src=img1.getMat();
    cvtColor(src, hsv, CV_BGR2HSV);
    calcHist( &hsv, 1, channels, mask1, // do not use mask
              hist1, 2, histSize, ranges,
              true, // the histogram is uniform
              false );

    src=img2.getMat();
    cvtColor(src, hsv, CV_BGR2HSV);
    calcHist( &hsv, 1, channels, mask2, // do not use mask
              hist2, 2, histSize, ranges,
              true, // the histogram is uniform
              false );

    normalize( hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat() );
    normalize( hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat() );
    return compareHist(hist1,hist2,method);
}

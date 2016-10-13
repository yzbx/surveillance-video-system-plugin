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

double ReDetection::myCompareLocalFeature(InputArray img1, InputArray img2, int method)
{
    //    Mat im1 = imread(im1_name, CV_LOAD_IMAGE_GRAYSCALE);
    //    Mat im2 = imread(im2_name, CV_LOAD_IMAGE_GRAYSCALE);
    Mat im1=img1.getMat();
    Mat im2=img2.getMat();
    if(im1.channels()==3) cvtColor(im1,im1,CV_BGR2GRAY);
    if(im2.channels()==3) cvtColor(im2,im2,CV_BGR2GRAY);

    if (im1.empty() || im2.empty())
    {
        cout << "could not open one of the images..." << endl;
        assert(false);
        return -1;
    }

    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );
    SurfDescriptorExtractor extractor;
    FlannBasedMatcher matcher_popcount;

    vector<KeyPoint> kpts_1, kpts_2;
    detector.detect(im1, kpts_1);
    detector.detect(im2, kpts_2);

    Mat desc_1, desc_2;
    extractor.compute(im1, kpts_1, desc_1);
    extractor.compute(im2, kpts_2, desc_2);

    //Do matching using features2d
    vector<DMatch> matches_popcount;
    matcher_popcount.match(desc_1,desc_2,matches_popcount);

    //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< DMatch > good_matches;

    for( int i = 0; i < desc_1.rows; i++ )
    {
        if(matches_popcount[i].distance < 0.1)
        {
            good_matches.push_back( matches_popcount[i]);
        }
    }

    return good_matches.size();
}

double ReDetection::myCompareLocalFeature(InputArray img1, const Mat &mask1, InputArray img2, const Mat &mask2, int method)
{
    //    Mat im1 = imread(im1_name, CV_LOAD_IMAGE_GRAYSCALE);
    //    Mat im2 = imread(im2_name, CV_LOAD_IMAGE_GRAYSCALE);
    Mat im1=img1.getMat();
    Mat im2=img2.getMat();
    if(im1.channels()==3) cvtColor(im1,im1,CV_BGR2GRAY);
    if(im2.channels()==3) cvtColor(im2,im2,CV_BGR2GRAY);

    if (im1.empty() || im2.empty())
    {
        cout << "could not open one of the images..." << endl;
        assert(false);
        return -1;
    }

    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );
    SurfDescriptorExtractor extractor;
    FlannBasedMatcher matcher_popcount;

    vector<KeyPoint> kpts_1, kpts_2;
    detector.detect(im1, kpts_1,mask1);
    detector.detect(im2, kpts_2,mask2);

    Mat desc_1, desc_2;
    extractor.compute(im1, kpts_1, desc_1);
    extractor.compute(im2, kpts_2, desc_2);

    //Do matching using features2d
    vector<DMatch> matches_popcount;
    matcher_popcount.match(desc_1,desc_2,matches_popcount);

    std::vector< DMatch > good_matches;

    for( int i = 0; i < desc_1.rows; i++ )
    {
        if(matches_popcount[i].distance < 0.1)
        {
            good_matches.push_back( matches_popcount[i]);
        }
    }

    return good_matches.size();

    return 0;
}

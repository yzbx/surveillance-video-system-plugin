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

double ReDetection::myCompareLocalFeature(InputArray img1, InputArray img2, int minHessian, double maxDist)
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

    //    int minHessian = 300;
    SurfFeatureDetector detector( minHessian );
    SurfDescriptorExtractor extractor;
    FlannBasedMatcher matcher_popcount;

    vector<KeyPoint> kpts_1, kpts_2;
    detector.detect(im1, kpts_1);
    detector.detect(im2, kpts_2);
    if(kpts_1.size()==0||kpts_2.size()==0) return 0;

    Mat desc_1, desc_2;
    extractor.compute(im1, kpts_1, desc_1);
    extractor.compute(im2, kpts_2, desc_2);
    if(desc_1.empty()||desc_2.empty()) return 0;

    //Do matching using features2d
    vector<DMatch> matches_popcount;
    matcher_popcount.match(desc_1,desc_2,matches_popcount);

    double max_dist = 0; double min_dist = 100;
    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < desc_1.rows; i++ )
    { double dist = matches_popcount[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    printf("-- Max dist : %f with cols=%d\n", max_dist, desc_1.cols);
    printf("-- Min dist : %f with cols=%d\n", min_dist, desc_1.cols);

    //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< DMatch > good_matches;

    for( int i = 0; i < desc_1.rows; i++ )
    {
        if(matches_popcount[i].distance < maxDist)
        {
            good_matches.push_back( matches_popcount[i]);
        }
    }

    return good_matches.size();
}

double ReDetection::myCompareLocalFeature(InputArray img1, const Mat &mask1, InputArray img2, const Mat &mask2, int minHessian, double maxDist)
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

    //    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );
    SurfDescriptorExtractor extractor;
    FlannBasedMatcher matcher_popcount;

    vector<KeyPoint> kpts_1, kpts_2;
    detector.detect(im1, kpts_1,mask1);
    detector.detect(im2, kpts_2,mask2);
    if(kpts_1.size()==0||kpts_2.size()==0) return 0;

    Mat desc_1, desc_2;
    extractor.compute(im1, kpts_1, desc_1);
    extractor.compute(im2, kpts_2, desc_2);

    if(desc_1.empty()||desc_2.empty()) return 0;

    //Do matching using features2d
    vector<DMatch> matches_popcount;
    matcher_popcount.match(desc_1,desc_2,matches_popcount);

    std::vector< DMatch > good_matches;

    for( int i = 0; i < desc_1.rows; i++ )
    {
        if(matches_popcount[i].distance < maxDist)
        {
            good_matches.push_back( matches_popcount[i]);
        }
    }

    return good_matches.size();
}

double ReDetection::myCompareHog(InputArray img1, InputArray img2)
{
    Mat im1=img1.getMat();
    Mat im2=img2.getMat();
    cv::Size imgsize(64,48);
    if(im1.channels()==3) cvtColor(im1,im1,CV_BGR2GRAY);
    if(im2.channels()==3) cvtColor(im2,im2,CV_BGR2GRAY);
    cv::resize(im1,im1,imgsize);
    cv::resize(im2,im2,imgsize);

    HOGDescriptor hog1(Size(32,16), Size(8,8), Size(4,4), Size(4,4), 9);
    HOGDescriptor hog2(Size(32,16), Size(8,8), Size(4,4), Size(4,4), 9);
    vector< float> des1,des2;
    hog1.compute( im1, des1);
    hog2.compute(im2,des2);
    float sum=0;
    assert(des1.size()==des2.size());
    for(int i=0;i<des1.size();i++){
        sum+=fabs(des1[i]-des2[i]);
    }

    return sum;
}

double ReDetection::myCompareColor(InputArray img1, InputArray img2)
{
    Mat im1=img1.getMat();
    Mat im2=img2.getMat();
    cv::Size imgsize(64,48);
    assert(img1.channels()==img2.channels());
    cv::resize(im1,im1,imgsize);
    cv::resize(im2,im2,imgsize);
    Mat dif;
    cv::absdiff(im1,im2,dif);
    double sum=0.0;
    Scalar sums=cv::sum(dif);

    int C=img1.channels();
    for(int i=0;i<C;i++){
        sum+=sums[i];
    }

    return sum/C;
}

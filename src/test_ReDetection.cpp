#include "ReDetection.h"

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    Mat img1=imread("/home/yzbx/Pictures/dindan.png");
//    Mat img2=imread("/home/yzbx/Pictures/GUI.jpg");
//    Mat img1=imread("/home/yzbx/build/build-objectTracking-Desktop_Qt_5_6_0_GCC_64bit-Debug//main/Dataset1_Cam1_5814_1_0.jpg");
    Mat img2=imread("/home/yzbx/build/build-objectTracking-Desktop_Qt_5_6_0_GCC_64bit-Debug/main/Dataset1_Cam1_5829_1_0.jpg");
    imshow("img1",img1);
    imshow("img2",img2);
    waitKey(2000);

    ReDetection rr;
    double v=rr.myCompareHist(img1,img2,3);
    cout<<"v="<<v<<endl;

    v=rr.myCompareLocalFeature(img1,img2);
    cout<<"v="<<v<<endl;
    return 0;
}

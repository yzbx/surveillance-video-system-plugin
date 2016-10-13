#include "ReDetection.h"

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    Mat img1=imread("/home/yzbx/Pictures/dindan.png");
    Mat img2=imread("/home/yzbx/Pictures/GUI.jpg");
    imshow("img1",img1);
    imshow("img2",img2);
    waitKey(0);

    ReDetection rr;
    double v=rr.myCompareHist(img1,img2,3);
    cout<<"v="<<v<<endl;
    return 0;
}

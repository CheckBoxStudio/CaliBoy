#include "chessboard.h"

int countSquares(
    const cv::Mat &image, 
    const cv::Point2f p1, const cv::Point2f p2, 
    const int winSize)
{
    int ns = 0;
    int w = image.cols;
    int h = image.rows;
    int step = image.step;
    const uchar *pData = image.data;
    float x1 = p1.x;
    float y1 = p1.y;
    float x2 = p2.x;
    float y2 = p2.y;

    if (
        (x1-winSize < 0) || (x1+winSize >= w) || (x2-winSize < 0) || (x2+winSize >= w) ||
        (y1-winSize < 0) || (y1+winSize >= h) || (y2-winSize < 0) || (y2+winSize >= h)) {
        ns = -1;
        return ns;
    }
    if ((x1 - x2)*(x1 - x2)+(y1 - y2)*(y1 - y2) < winSize) {
        ns = -1;
        return ns;
    }

    double lambda[3] = {y1 - y2,x2 - x1,x1*y2 - x2*y1};
    double scale = 1/sqrt(lambda[0]*lambda[0]+lambda[1]*lambda[1]);
    lambda[0] *= scale;
    lambda[1] *= scale;
    lambda[2] *= scale;

    double dx = x2-x1;
    double dy = y2-y1;

    int Np = 0;
    double *xs = 0;
    double *ys = 0;
    if (abs(dx) > abs(dy)) {
        Np = floor(abs(dx));
        xs = new double[Np];
        ys = new double[Np];
        int k = 1;
        if (x2 < x1)
            k = -1;
        for (int i=0; i<Np; ++i) {
            xs[i] = x1 + k*i;
            ys[i] = -(lambda[2] + lambda[0]*xs[i])/lambda[1];
        }
    }
    else {
        Np = floor(abs(dy));
        xs = new double[Np];
        ys = new double[Np];
        int k = 1;
        if (y2 < y1)
            k = -1;
        for (int i=0; i<Np; ++i) {
            ys[i] = y1 + k*i;
            xs[i] = -(lambda[2] + lambda[1]*ys[i])/lambda[0];
        }
    }
    cv::Mat imgPatch(winSize*2+1, Np, image.type());
    int stepPatch = imgPatch.step;
    uchar *pPatch = imgPatch.data;
    for (int j=-winSize; j<=winSize; ++j)
    {
        int r = j+winSize;
        double _dx = j*lambda[0];
        double _dy = j*lambda[1];
        for (int i=0; i<Np; ++i) {
            int val = pData[ int(ys[i]+_dy+0.5)*step + int(xs[i]+_dx+0.5) ];
            pPatch[r*stepPatch + i] = val;
        }
    }
    //cv::imshow("Test",imgPatch);
    //cv::waitKey();
    float *k_sum = new float[winSize*2+1];
    k_sum[winSize] = 0;
    for (int i=0; i<winSize; i++) {
        k_sum[i] = 1;
        k_sum[i+winSize+1] = -1;
    }
    cv::Mat kernel_sum(winSize*2+1,1,CV_32FC1,k_sum);
    float k_mean[3] = {0.25, 0.5, 0.25};
    cv::Mat kernel_mean(1,3,CV_32FC1,k_mean);

    filter2D(imgPatch,imgPatch,-1,kernel_sum);
    filter2D(imgPatch,imgPatch,-1,kernel_mean);

    pPatch = (uchar *)(imgPatch.data + (winSize+1)*stepPatch);
    for (int i=winSize+1; i<Np-winSize; ++i)
    {
        if ( (pPatch[i-1]==0 && pPatch[i]>0 ) ||
             (pPatch[i-1]>0 && pPatch[i]==0 ))
            ns++;
    }

    delete []xs;
    delete []ys;
    delete []k_sum;

    return ns;
}

void findChessboardCornersHomography(
    const std::vector<cv::Point2f> &boundCorners,
    std::vector<cv::Point2f> &gridCorners,
    const int nX, const int nY)
{
    std::vector<cv::Point2f> planePoints;
    planePoints.push_back(cv::Point2f(0,0));
    planePoints.push_back(cv::Point2f(0,1));
    planePoints.push_back(cv::Point2f(1,1));
    planePoints.push_back(cv::Point2f(1,0));

    cv::Mat HMat = findHomography(planePoints,boundCorners);
    planePoints.clear();
    //transpose(HMat,HMat);

    gridCorners.clear();
    for (int i=0; i<nY; ++i) {
        for (int j=0; j<nX; ++j) {
            double sX = j*1.0/(nX-1.0);
            double sY = i*1.0/(nY-1.0);
            double x = sX * HMat.at<double>(0,0) + sY * HMat.at<double>(0,1) + HMat.at<double>(0,2);
            double y = sX * HMat.at<double>(1,0) + sY * HMat.at<double>(1,1) + HMat.at<double>(1,2);
            double k = sX * HMat.at<double>(2,0) + sY * HMat.at<double>(2,1) + HMat.at<double>(2,2);

            gridCorners.push_back(cv::Point2f(x/k,y/k));
        }
    }
}

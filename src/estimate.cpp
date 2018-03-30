#include "estimate.h"
#include "..\eigen_3.3.1\Eigen\Eigen"

using namespace Eigen;

double estFocal_Rect(
    const std::vector<cv::Point2f> &corners,
    const double nX, const double nY,
    const double init_f)
{
    assert(corners.size() == 4);

    double f_est = init_f;

    Vector3f point00, point01, point11, point10;
    Vector3f Lines1, Lines2;
    Vector3f DVanishingPoint1(3,1);
    Vector3f DVanishingPoint2(3,1);
    Vector3f hVanishingPoint(3,1);
    Vector3f vVanishingPoint(3,1);
    Vector3f vanishingLine(3,1);

    // convert into format of eigen
    point00 << corners.at(0).x/f_est, corners.at(0).y/f_est, 1.0;
    point10 << corners.at(1).x/f_est, corners.at(1).y/f_est, 1.0;
    point11 << corners.at(2).x/f_est, corners.at(2).y/f_est, 1.0;
    point01 << corners.at(3).x/f_est, corners.at(3).y/f_est, 1.0;

    // find the intersection by cross-product, include the anishing points
    Lines1 =  point00.cross(point10);
    Lines2 =  point01.cross(point11);
    hVanishingPoint = Lines1.cross(Lines2);
    Lines1 =  point00.cross(point01);
    Lines2 =  point10.cross(point11);
    vVanishingPoint = Lines1.cross(Lines2);

    // find the vanishing line
    vanishingLine = hVanishingPoint.cross(vVanishingPoint); 
    if (vanishingLine(2)<0) vanishingLine *= -1.0;
    vanishingLine /= vanishingLine.norm();

    // length unit for projective space
    double r = vanishingLine.dot(point00);
    Vector3f hUnitDir = point10*r/vanishingLine.dot(point10)-point00;
    Vector3f vUnitDir = point01*r/vanishingLine.dot(point01)-point00;

    // vertex of squre
    double k = sqrtf(nX/nY);
    Vector3f pointH = point00 + hUnitDir/k; pointH /= pointH(2);
    Vector3f pointV = point00 + vUnitDir*k; pointV /= pointV(2);
    Vector3f pointD = point00 + hUnitDir/k + vUnitDir*k; pointD /= pointD(2);

    // vanishing points
    Lines1 =  point00.cross(pointD);
    Lines2 =  pointV.cross(pointH);
    DVanishingPoint1 = vanishingLine.cross(Lines1);
    DVanishingPoint2 = vanishingLine.cross(Lines2); 

    hVanishingPoint /= hVanishingPoint.norm();
    vVanishingPoint /= vVanishingPoint.norm();
    DVanishingPoint1 /= DVanishingPoint1.norm();
    DVanishingPoint2 /= DVanishingPoint2.norm();

    double a1 = hVanishingPoint(0);
    double b1 = hVanishingPoint(1);
    double c1 = hVanishingPoint(2);
   
    double a2 = vVanishingPoint(0);
    double b2 = vVanishingPoint(1);
    double c2 = vVanishingPoint(2);
   
    double a3 = DVanishingPoint1(0);
    double b3 = DVanishingPoint1(1);
    double c3 = DVanishingPoint1(2);
   
    double a4 = DVanishingPoint2(0);
    double b4 = DVanishingPoint2(1);
    double c4 = DVanishingPoint2(2);

    f_est *= sqrt(abs(- (c1*c2*(a1*a2 + b1*b2) + c3*c4*(a3*a4 + b3*b4)) / (c1*c1*c2*c2 + c3*c3*c4*c4)));

    return f_est;
}

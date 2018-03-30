#include "undistortion.h"
#include <math.h>

/* [Derivation]
 * -------------------------------------
 * |  x'   = x * ( 1 + c * r^2 )
 * |  y'   = y * ( 1 + c * r^2 )
 * |  r^2  = x^2 + y^2
 * |------------------------------------
 * |  Given(x',y'), c, estimate (x,y)
 * |------------------------------------
 * |  @ For radial distortion, ph is invariant
 * |  >> x' / cos(ph)   =   x / cos(ph) * ( 1 + c * r^2 )
 * |  @ x' / cos(ph) = r'
 * |  @ x / cos(ph) = r
 * |  >> r'             =   r * ( 1 + c * r^2 )
 * |  >> r^3 + (1/c) * r - (1/c) * r' = 0
 * |------------------------------------
 * |  Solve a cubic equation with one unknown, whos quadratic term is 0
 * |------------------------------------
 * |  x^3 + px + q = 0
 * |  x == r
 * |  p = 1/c
 * |  q = -r'/c
 * |------------------------------------
 * |  Cardin formula
 * |------------------------------------
 * |  x1 = (__ -q/2 + (_ (q/2)^2 + (p/3)^3 _)^(1/2) __)^(1/3) + 
 * |       (__ -q/2 + (_ (q/2)^2 - (p/3)^3 _)^(1/2) __)^(1/3)
 * |  x2 = ...
 * |  x3 = ...
 * |  Or
 * |  x1    = 2 * r^(1/3) * cos(theta)
 * |  x2    = ...
 * |  x3    = ...
 * |  r     = ( -(p/3)^3 ) ^ (1/2)
 * |  theta = (1/3) * arccos (-q/(2*r))
 * |------------------------------------
 */
void removeDistortion2(std::vector<cv::Point2f> &points, const double c)
{
    if (abs(c) <= 1e-6)
        return;
    for (int i=0; i<points.size(); ++i) {
        double x = points.at(i).x;
        double y = points.at(i).y;
        double r = 0;
        double ph = atan2(y,x);
        double Q = -1/(3*c);// -p/3
        double R = -x/(2*c*cos(ph));// q/2
        double Q3 = pow(Q,3.0);
        double R2 = pow(R,2.0);
        if (c<0) {
            // this works in all practical situations (it starts failing for very large
            // values of c)
            double theta = acos(R/sqrt(Q3));
            r = -2*sqrt(Q)*cos((theta-2*3.141592653)/3.0);
        } else {
            // note: this always works, even for ridiculous values of c
            double A = pow(sqrt(R2-Q3)-R, 1.0/3.0); 
            double B = Q * (1.0/A);
            r = (A+B);
        }
        points.at(i).x = r*cos(ph);
        points.at(i).y = r*sin(ph);
    }
}
void addDistortion2(std::vector<cv::Point2f> &points, const double c)
{
    if (abs(c) <= 1e-6)
        return;
    for (int i=0; i<points.size(); ++i) {
        double x = points.at(i).x;
        double y = points.at(i).y;
        double k = 1.0 + c * (x*x + y*y);
        points.at(i).x *= k;
        points.at(i).y *= k;
    }
}

/*
 * Iteration
 */
void removeDistortionFast(std::vector<cv::Point2f> &points, const double k1)
{
    if (k1 == 0)
        return;

    for (int i=0; i<points.size(); ++i) {
        cv::Point2f p = points.at(i);
        double r2 = p.x*p.x + p.y*p.y;
        double s = 1 + k1 * r2/(1+k1*r2);
        points.at(i).x = p.x/s;
        points.at(i).y = p.y/s;
    }
}
void removeDistortionFast(
    std::vector<cv::Point2f> &points,
    const double k1, const double k2,
    const double p1, const double p2,
    const double k3)
{
    if (k1 == 0 && k2 == 0 && k3 == 0 &&
        p1 == 0 && p2 == 0)
        return;

    for (int i=0; i<points.size(); ++i) {

        cv::Point2f p = points.at(i);
        for (int ii=0; ii<20; ++ii) {
            double r2 = p.x*p.x + p.y*p.y;
            double k = (((k3 * r2) + k2)* r2 + k1)*r2 +1;
            double px = 2*p1*p.x*p.y + p2*(r2 + 2*p.x*p.x);
            double py = 2*p2*p.x*p.y + p1*(r2 + 2*p.y*p.y);
            p.x = (points.at(i).x - px)/k;
            p.y = (points.at(i).y - py)/k;
        }
        points.at(i).x = p.x;
        points.at(i).y = p.y;

    }
}
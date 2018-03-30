#ifndef UNDISTORTION_H
#define UNDISTORTION_H

#include <vector>
#include <cv.h>

/*
 * Removal of distortion.
 * -------------------------------------
 * |  x'   = x * ( 1 + c * r^2 )
 * |  y'   = y * ( 1 + c * r^2 )
 * |  r^2  = x^2 + y^2
 * |  Given (x',y'), c, estimate (x,y).
 * -------------------------------------
 * @param points: coordinates with distortion (X = (X-uX)/f);
 * @param c: distortion factor.
 */
void removeDistortion2(std::vector<cv::Point2f> &points, const double c);

/*
 * Add distortion.
 * -------------------------------------
 * |  x'   = x * ( 1 + c * r^2 )
 * |  y'   = y * ( 1 + c * r^2 )
 * |  r^2  = x^2 + y^2
 * |  Given (x,y), c, estimate (x',y').
 * -------------------------------------
 * @param points: coordinates without distortion (X = (X-uX)/f);
 * @param c: distortion factor.
 */
void addDistortion2(std::vector<cv::Point2f> &points, const double c);

/*
 * Removal of distortion by iteration.
 * -------------------------------------
 * Two steps.
 * -------------------------------------
 * @param points: coordinates with distortion (X = (X-uX)/f);
 * @param c: distortion factor.
 */
void removeDistortionFast(std::vector<cv::Point2f> &points, const double k1);

/*
 * Removal of distortion by iteration.
 * -------------------------------------
 * IterationNum == 20
 * -------------------------------------
 * @param points: coordinates with distortion (X = (X-uX)/f);
 * @param k1/k2/k3: radial distortion factor;
 * @param p1/p2: tangential distortion factor.
 */
void removeDistortionFast(
    std::vector<cv::Point2f> &points,
    const double k1, const double k2,
    const double p1, const double p2,
    const double k3 = 0);
#endif// !UNDISTORTION_H
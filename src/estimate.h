#ifndef ESTIMATE_H
#define ESTIMATE_H

#include <cv.h>

/*
 * Initial estimation of focal length.
 * @param corners: corners of projection of a square;
 * @param nX/nY: ratio of width to height of the original square;
 * @param init_f: initial suggestion value of focal length;
 * @return : the estimation of focal length.
 */
double estFocal_Rect(
    const std::vector<cv::Point2f> &corners,
    const double nX, const double nY,
    const double init_f = 100.0);

#endif // !ESTIMATE_H
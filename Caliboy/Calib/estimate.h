#ifndef ESTIMATE_H
#define ESTIMATE_H

#include <cv.h>

/*
 * 初步估计像素焦距.
 * @param corners: 输入图像矩形角点(中心移至主点);
 * @param nX/nY: 输入矩形水平/竖直方向长度(相对长度即可);
 * @param init_f: 像素焦距初始估计值;
 * @return : 初步估计像素焦距.
 */
double estFocal_Rect(const std::vector<cv::Point2f> &corners,
					 const double nX, const double nY,
					 const double init_f = 100.0);

#endif // !ESTIMATE_H
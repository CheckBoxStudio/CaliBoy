#ifndef UNDISTORTION_H
#define UNDISTORTION_H

#include <vector>
#include <cv.h>

/*
 * 去除畸变.
 * -------------------------------------
 * |  x'   = x * ( 1 + c * r^2 )
 * |  y'   = y * ( 1 + c * r^2 )
 * |  r^2  = x^2 + y^2
 * |  已知(x',y')、c，求(x,y)
 * -------------------------------------
 * @param points: 输入有畸变点坐标,相机坐标系下坐标(X = (X-uX)/f);
 * @param c: 畸变因子.
 */
void removeDistortion2(std::vector<cv::Point2f> &points, const double c);

/*
 * 添加畸变.
 * -------------------------------------
 * |  x'   = x * ( 1 + c * r^2 )
 * |  y'   = y * ( 1 + c * r^2 )
 * |  r^2  = x^2 + y^2
 * |  已知(x,y)、c，计算(x',y')
 * -------------------------------------
 * @param points: 输入有畸变点坐标,相机坐标系下坐标(X = (X-uX)/f);
 * @param c: 畸变因子;
 */
void addDistortion2(std::vector<cv::Point2f> &points, const double c);

/*
 * 去除畸变.
 * -------------------------------------
 * 两步迭代
 * -------------------------------------
 * @param points: 输入有畸变点坐标,相机坐标系下坐标(X = (X-uX)/f);
 * @param c: 畸变因子.
 */
void removeDistortionFast(std::vector<cv::Point2f> &points, const double k1);

/*
 * 去除畸变.
 * -------------------------------------
 * 迭代去畸变(iterationNum == 20)
 * -------------------------------------
 * @param points: 输入有畸变点坐标,相机坐标系下坐标(X = (X-uX)/f);
 * @param k1/k2/k3: 径向畸变因子;
 * @param p1/p2: 切向畸变因子.
 */
void removeDistortionFast(std::vector<cv::Point2f> &points,
						  const double k1, const double k2,
						  const double p1, const double p2,
						  const double k3 = 0);
#endif // !UNDISTORTION_H
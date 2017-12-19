#ifndef UNDISTORTION_H
#define UNDISTORTION_H

#include <vector>
#include <cv.h>

/*
 * ȥ������.
 * -------------------------------------
 * |  x'   = x * ( 1 + c * r^2 )
 * |  y'   = y * ( 1 + c * r^2 )
 * |  r^2  = x^2 + y^2
 * |  ��֪(x',y')��c����(x,y)
 * -------------------------------------
 * @param points: �����л��������,�������ϵ������(X = (X-uX)/f);
 * @param c: ��������.
 */
void removeDistortion2(std::vector<cv::Point2f> &points, const double c);

/*
 * ��ӻ���.
 * -------------------------------------
 * |  x'   = x * ( 1 + c * r^2 )
 * |  y'   = y * ( 1 + c * r^2 )
 * |  r^2  = x^2 + y^2
 * |  ��֪(x,y)��c������(x',y')
 * -------------------------------------
 * @param points: �����л��������,�������ϵ������(X = (X-uX)/f);
 * @param c: ��������;
 */
void addDistortion2(std::vector<cv::Point2f> &points, const double c);

/*
 * ȥ������.
 * -------------------------------------
 * ��������
 * -------------------------------------
 * @param points: �����л��������,�������ϵ������(X = (X-uX)/f);
 * @param c: ��������.
 */
void removeDistortionFast(std::vector<cv::Point2f> &points, const double k1);

/*
 * ȥ������.
 * -------------------------------------
 * ����ȥ����(iterationNum == 20)
 * -------------------------------------
 * @param points: �����л��������,�������ϵ������(X = (X-uX)/f);
 * @param k1/k2/k3: �����������;
 * @param p1/p2: �����������.
 */
void removeDistortionFast(std::vector<cv::Point2f> &points,
						  const double k1, const double k2,
						  const double p1, const double p2,
						  const double k3 = 0);
#endif // !UNDISTORTION_H
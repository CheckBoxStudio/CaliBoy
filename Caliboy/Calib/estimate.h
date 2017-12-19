#ifndef ESTIMATE_H
#define ESTIMATE_H

#include <cv.h>

/*
 * �����������ؽ���.
 * @param corners: ����ͼ����νǵ�(������������);
 * @param nX/nY: �������ˮƽ/��ֱ���򳤶�(��Գ��ȼ���);
 * @param init_f: ���ؽ����ʼ����ֵ;
 * @return : �����������ؽ���.
 */
double estFocal_Rect(const std::vector<cv::Point2f> &corners,
					 const double nX, const double nY,
					 const double init_f = 100.0);

#endif // !ESTIMATE_H
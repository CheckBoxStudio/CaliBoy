#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <vector>
#include <cv.h>

/*
 * ����������������̸�ǵ���(�������).
 * @param image: ����ͼ��;
 * @param p1/p2: �����λ��;
 * @param winSize: �ǵ�������Χ.
 */
int countSquares(const cv::Mat &image, const cv::Point2f p1, const cv::Point2f p2, 
				 const int winSize);
/*
 * ����ǵ����������̸�ǵ�.
 * @param boundCorners: ���α߽��;
 * @param gridCorners: [output]���̸�ǵ�;
 * @param nX/nY: [output]���̸�ǵ�����/����.
 */
void findChessboardCornersHomography(const std::vector<cv::Point2f> &boundCorners,
									 std::vector<cv::Point2f> &gridCorners,
									 const int nX, const int nY);	

#endif // !CHESS_BOARD_H
#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <vector>
#include <cv.h>

/*
 * 计算两点间连线棋盘格角点数(含输入点).
 * @param image: 输入图像;
 * @param p1/p2: 输入点位置;
 * @param winSize: 角点搜索范围.
 */
int countSquares(const cv::Mat &image, const cv::Point2f p1, const cv::Point2f p2, 
				 const int winSize);
/*
 * 计算角点区域内棋盘格角点.
 * @param boundCorners: 矩形边界点;
 * @param gridCorners: [output]棋盘格角点;
 * @param nX/nY: [output]棋盘格角点列数/行数.
 */
void findChessboardCornersHomography(const std::vector<cv::Point2f> &boundCorners,
									 std::vector<cv::Point2f> &gridCorners,
									 const int nX, const int nY);	

#endif // !CHESS_BOARD_H
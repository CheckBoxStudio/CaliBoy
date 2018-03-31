#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <vector>
#include <cv.h>

/*
 * Find all grid points along the line (the endpoints are included too).
 * @param image: input image;
 * @param p1/p2: endpoints of the line;
 * @param winSize: neighborhood size for searching.
 */
int countSquares(
    const cv::Mat &image, 
    const cv::Point2f p1, const cv::Point2f p2, 
    const int winSize);
/*
 * Find all grid points in a quadrilateral area.
 * @param boundCorners: the vertexes of the quadrilateral area;
 * @param gridCorners: [output] all the grid points;
 * @param nX/nY: [output] the column and row number of the grid points.
 */
void findChessboardCornersHomography(
    const std::vector<cv::Point2f> &boundCorners,
    std::vector<cv::Point2f> &gridCorners,
    const int nX, const int nY);

#endif // !CHESS_BOARD_H
#ifndef CAMERA_H
#define CAMERA_H

#include <QString>
#include <QImage>
#include <cv.h>
#include <vector>

using cv::Mat;
using std::vector;
using cv::Point2f;

/*
 * 相机类[数据类]
 * 主要包括相机内部参数.
 */
class camera{
public:
	camera();
	~camera();
	void clean();
	camera& operator=(camera &other);
public:
	double fx;		// 相机等效焦距X
	double fy;		// 相机等效焦距Y
	double cx;		// 相机像素主点X
	double cy;		// 相机像素主点Y
	double s;		// 倾斜因子
	double k[5];	// 畸变系数k1,k2,p1,p2,k3
};


/*
* 视图类[数据类]
* 主要包括视图图像和网格点及标定位置.
*/
class cameraView{
public:
	cameraView();
	cameraView(QString name);
	~cameraView();
	void clean();
	cameraView& operator=(cameraView &other);


	void loadImage();										/* 重新导入图像 */
	bool isGridCornerPicked();								/* 是否选定好角点:判断有无 */
	void setImage(QString name);							/* 设置图像路径 */
	void setGridCorners(const int nx, const int ny,			/* 设置角点 */
						const vector<Point2f> &corners);

	QString getFileName() {	return m_name; }				/* 获取图像名 */
	QImage* getQImage() { return m_QImage; }				/* 获取显示图像 */
	Mat getMatImage() {	return m_matImage; }				/* 获取处理图像 */
	vector<Point2f>& getGridCorners(int &nx, int &ny);		/* 获取角点 */
	void getGridSize(int &nx, int &ny) {					/* 获取网格角点尺寸 */
		nx = m_nX;
		ny = m_nY;
	}
	int getGridSizeX() {return m_nX;}						/* 获取网格角点X方向个数 */
	int getGridSizeY() {return m_nY;}						/* 获取网格角点Y方向个数 */
	int width() {return m_matImage.cols; }					/* 获取图像宽 */
	int height() {return m_matImage.rows; }					/* 获取图像高 */
public:
	Mat r;		/* 旋转向量 */
	Mat R;		/* 旋转矩阵 */	
	Mat t;		/* 平移向量 */
private:
	QString m_name;						/* 图像名 */
	QImage *m_QImage;					/* 图像[用于显示] */
	Mat m_matImage;						/* 图像[用于处理] */
	int m_nX;							/* 网格角点X方向个数 */
	int m_nY;							/* 网格角点Y方向个数 */
	vector<Point2f> m_gridCorners;		/* 网格角点 */
};


/*
* 场景类[数据类]
* 主要包括相机和视图及网格尺寸.
*/
class cameraScene {
public:
	cameraScene();
	~cameraScene();
	void clean();

	int contains(const QString viewName);			/* 判断是否包含视图[无:-1] */
	void addView(cameraView *newView);				/* 添加视图 */
	void removeView(const QString removedView);		/* 根据图像路径移除视图 */
	void removeView(cameraView *removedView);		/* 移除视图 */
	int viewCount() {return m_views.size();}		/* 视图个数 */

	void getQuadSize(double &x, double &y) {
		x = m_sizeX;
		y = m_sizeY;
	}
	void setQuadSize(const double x, const double y) {
		m_sizeX = x;
		m_sizeY = y;
	}
public:
	double m_sizeX;					/* 网格X方向实际尺寸[mm] */
	double m_sizeY;					/* 网格Y方向实际尺寸[mm] */
	camera m_camera;				/* 相机 */
	vector<cameraView *> m_views;	/* 视图 */
};
#endif // !CAMERA_H
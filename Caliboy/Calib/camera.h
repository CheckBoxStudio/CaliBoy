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
 * �����[������]
 * ��Ҫ��������ڲ�����.
 */
class camera{
public:
	camera();
	~camera();
	void clean();
	camera& operator=(camera &other);
public:
	double fx;		// �����Ч����X
	double fy;		// �����Ч����Y
	double cx;		// �����������X
	double cy;		// �����������Y
	double s;		// ��б����
	double k[5];	// ����ϵ��k1,k2,p1,p2,k3
};


/*
* ��ͼ��[������]
* ��Ҫ������ͼͼ�������㼰�궨λ��.
*/
class cameraView{
public:
	cameraView();
	cameraView(QString name);
	~cameraView();
	void clean();
	cameraView& operator=(cameraView &other);


	void loadImage();										/* ���µ���ͼ�� */
	bool isGridCornerPicked();								/* �Ƿ�ѡ���ýǵ�:�ж����� */
	void setImage(QString name);							/* ����ͼ��·�� */
	void setGridCorners(const int nx, const int ny,			/* ���ýǵ� */
						const vector<Point2f> &corners);

	QString getFileName() {	return m_name; }				/* ��ȡͼ���� */
	QImage* getQImage() { return m_QImage; }				/* ��ȡ��ʾͼ�� */
	Mat getMatImage() {	return m_matImage; }				/* ��ȡ����ͼ�� */
	vector<Point2f>& getGridCorners(int &nx, int &ny);		/* ��ȡ�ǵ� */
	void getGridSize(int &nx, int &ny) {					/* ��ȡ����ǵ�ߴ� */
		nx = m_nX;
		ny = m_nY;
	}
	int getGridSizeX() {return m_nX;}						/* ��ȡ����ǵ�X������� */
	int getGridSizeY() {return m_nY;}						/* ��ȡ����ǵ�Y������� */
	int width() {return m_matImage.cols; }					/* ��ȡͼ��� */
	int height() {return m_matImage.rows; }					/* ��ȡͼ��� */
public:
	Mat r;		/* ��ת���� */
	Mat R;		/* ��ת���� */	
	Mat t;		/* ƽ������ */
private:
	QString m_name;						/* ͼ���� */
	QImage *m_QImage;					/* ͼ��[������ʾ] */
	Mat m_matImage;						/* ͼ��[���ڴ���] */
	int m_nX;							/* ����ǵ�X������� */
	int m_nY;							/* ����ǵ�Y������� */
	vector<Point2f> m_gridCorners;		/* ����ǵ� */
};


/*
* ������[������]
* ��Ҫ�����������ͼ������ߴ�.
*/
class cameraScene {
public:
	cameraScene();
	~cameraScene();
	void clean();

	int contains(const QString viewName);			/* �ж��Ƿ������ͼ[��:-1] */
	void addView(cameraView *newView);				/* �����ͼ */
	void removeView(const QString removedView);		/* ����ͼ��·���Ƴ���ͼ */
	void removeView(cameraView *removedView);		/* �Ƴ���ͼ */
	int viewCount() {return m_views.size();}		/* ��ͼ���� */

	void getQuadSize(double &x, double &y) {
		x = m_sizeX;
		y = m_sizeY;
	}
	void setQuadSize(const double x, const double y) {
		m_sizeX = x;
		m_sizeY = y;
	}
public:
	double m_sizeX;					/* ����X����ʵ�ʳߴ�[mm] */
	double m_sizeY;					/* ����Y����ʵ�ʳߴ�[mm] */
	camera m_camera;				/* ��� */
	vector<cameraView *> m_views;	/* ��ͼ */
};
#endif // !CAMERA_H
#include "camera.h"
#include "highgui.h"
#include "qtChinese.h"
#include <QFileInfo>

camera::camera()
{
	clean();
}
camera::~camera()
{
}
void camera::clean()
{
	fx = 1.0;
	fy = 1.0;
	cx = 0.0;
	cy = 0.0;
	s = 0.0;
	for (int i=0; i<5; ++i)
		k[i] =0;
}
camera& camera::operator=(camera &other)
{
	if ( &other != this) {
		fx = other.fx;
		fy = other.fy;
		cx = other.cx;
		cy = other.cy;
		s = other.s;
		for (int i=0; i<5; ++i)
			k[i] = other.k[i];
	}
	return *this;
}


cameraView::cameraView()
	: m_name("")
	, m_QImage(0)
{
	R = Mat::eye(3,3,CV_64F);
	r = Mat::zeros(3,1,CV_64F);
	t = Mat::zeros(3,1,CV_64F);
	m_nX = 0;
	m_nY = 0;
}
cameraView::cameraView(QString name)
	: m_name(name)
	, m_QImage(0)
{
	R = Mat::eye(3,3,CV_64F);
	r = Mat::zeros(3,1,CV_64F);
	t = Mat::zeros(3,1,CV_64F);
	m_nX = 0;
	m_nY = 0;
	loadImage();
}
cameraView::~cameraView()
{
	if (0 != m_QImage)
		delete m_QImage;
	m_gridCorners.clear();
}
void cameraView::clean()
{
	R = Mat::eye(3,3,CV_64F);
	r = Mat::zeros(3,1,CV_64F);
	t = Mat::zeros(3,1,CV_64F);
	m_name = "";
	m_matImage.release();
	if (0 != m_QImage) {
		delete m_QImage;
		m_QImage = 0;
	}
	m_nX = 0;
	m_nY = 0;
	m_gridCorners.clear();
}
bool cameraView::isGridCornerPicked()
{
	bool ret = false;
	if (m_nX != 0 && m_nY != 0 &&
		m_gridCorners.size() == m_nY*m_nX)
		ret = true;

	return ret;
}
void cameraView::loadImage()
{
	if (0 != m_QImage) {
		delete m_QImage;
		m_QImage = 0;
	}
	m_QImage = new QImage(m_name);
	m_matImage = cv::imread(ToCHN(m_name),0);


	if (m_QImage->isNull()) {
		delete m_QImage;
		m_QImage = 0;
		m_matImage.release();
	}
}
void cameraView::setImage(QString name)
{
	clean();
	m_name = name;
	loadImage();
}
cameraView& cameraView::operator=(cameraView &other)
{
	if ( &other != this) {
		clean();
		m_name = other.m_name;
		loadImage();
		R = other.R.clone();
		t = other.t.clone();
	}
	return *this;
}
vector<Point2f>& cameraView::getGridCorners(int &nx, int &ny)
{
	nx = m_nX;
	ny = m_nY;
	return m_gridCorners;
}
void cameraView::setGridCorners(const int nx, const int ny, const vector<Point2f> &corners)
{
	m_nX = nx;
	m_nY = ny;
	m_gridCorners.clear();
	for (int i=0; i<m_nY*m_nX; ++i)
		m_gridCorners.push_back(corners.at(i));
}

cameraScene::cameraScene()
	: m_sizeX(30.0)
	, m_sizeY(30.0)
{
	
}
cameraScene::~cameraScene()
{
	clean();
}
void cameraScene::clean()
{
	for (int i = 0; i < m_views.size(); ++i) {
		if (0 != m_views.at(i))
			delete m_views.at(i);
	}
	m_views.clear();
	m_camera.clean();
}
int cameraScene::contains(const QString viewName)
{
	for (int i = 0; i<m_views.size(); ++i) {
		if (QFileInfo(m_views.at(i)->getFileName()).absoluteFilePath() 
			== QFileInfo(viewName).absoluteFilePath()) {
			return i;
		}
	}
	return -1;
}
void cameraScene::addView(cameraView * newView)
{
	m_views.push_back(newView);
}
void cameraScene::removeView(const QString removedView)
{
	for (auto iter = m_views.begin(); iter != m_views.end(); ++iter) {
		if (QFileInfo((*iter)->getFileName()).absoluteFilePath() 
			== QFileInfo(removedView).absoluteFilePath()) {
			m_views.erase(iter);
			break;
		}
	}
}
void cameraScene::removeView(cameraView * removedView)
{
	for (auto iter = m_views.begin(); iter != m_views.end(); ++iter) {
		if (*iter == removedView) {
			m_views.erase(iter);
			break;
		}
	}
}

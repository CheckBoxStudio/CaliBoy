#include "camera.h"
#include "highgui.h"
#include <QFileInfo>

camera::camera()
{
    clean();
}
camera::~camera()
{
    clean();
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
    : m_QImage(0)
{
    clean();
}
cameraView::cameraView(QString name)
    : m_QImage(0)
{
    clean();
    loadImage(name);
}
cameraView::~cameraView()
{
    clean();
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
cameraView& cameraView::operator=(cameraView &other)
{
    if (&other != this) {
        clean();
        loadImage(other.m_name);
        r = other.r.clone();
        R = other.R.clone();
        t = other.t.clone();
    }
    return *this;
}
bool cameraView::isGridCornerPicked()
{
    return (m_nX != 0 && m_nY != 0 && m_gridCorners.size() == m_nY*m_nX);
}
void cameraView::loadImage(QString file)
{
    clean();
    
    m_QImage = new QImage(file);
    if (m_QImage->isNull()) {
        delete m_QImage;
        m_QImage = 0;
    }
    else {
        m_matImage = cv::imread(file.toLocal8Bit().data(), 0);
        m_name = file;
    }
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
    m_sizeX = 0;
    m_sizeY = 0;
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
int cameraScene::contains(const cameraView * newView)
{
    return contains(newView->getFileName());
}

void cameraScene::addView(cameraView * newView)
{
    if (contains(newView->getFileName()) == -1)
        m_views.push_back(newView);
}
void cameraScene::addView(const QString newFile)
{
    if (contains(newFile) == -1)
        m_views.push_back(new cameraView(newFile));
}

void cameraScene::removeView(const QString removedView)
{
    int n = contains(removedView);
    if (n != -1)
        m_views.erase(m_views.begin() + n);
}
void cameraScene::removeView(cameraView * removedView)
{
    int n = contains(removedView);
    if (n != -1)
        m_views.erase(m_views.begin() + n);
}

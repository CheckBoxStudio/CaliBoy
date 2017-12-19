#include "calibresultviewer.h"
#include <QSplitter>
#include <QLayout>
#include "qtChinese.h"

#include "QCustomPlot\qcustomplot.h"
#include "scenerender.h"

#include "Calib\camera.h"
#include <cv.h>

CalibResultViewer::CalibResultViewer(cameraScene *calibScene, QWidget *parent)
	: QDockWidget(parent)
	, m_scene(0)
	, m_viewIdx(-1)
	, m_showAll(true)
	, m_fixCamera(true)
{
	setWindowTitle(FromCHN("标定结果"));
	setFeatures(QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	setupme();
	setScene(calibScene);
}
CalibResultViewer::~CalibResultViewer()
{	
	m_errPlot->clearGraphs();
}

void CalibResultViewer::setupme()
{
	m_sceneRender = new SceneRender(0,this);

	m_errPlot = new QCustomPlot(this);
	m_errPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    m_errPlot->xAxis->setLabel("dx");
    m_errPlot->yAxis->setLabel("dy");
    m_errPlot->axisRect()->setupFullAxesBox();

	butShowAllOrOne = new QPushButton(this);
	butFixCameraOrPlane = new QPushButton(this);
	setButText();

	QSplitter *splitter = new QSplitter(Qt::Vertical,this);
	QVBoxLayout *vLay = new QVBoxLayout();
	QHBoxLayout *hLay = new QHBoxLayout();

	splitter->addWidget(m_sceneRender);
	splitter->addWidget(m_errPlot);
	hLay->addWidget(butFixCameraOrPlane);
	hLay->addStretch();
	hLay->addWidget(butShowAllOrOne);
	vLay->addWidget(splitter);
	vLay->addLayout(hLay);

	setLayout(vLay);
	QWidget *_centralWidget = new QWidget(this);
	_centralWidget->setLayout(vLay);
	setWidget(_centralWidget);

	connect(butFixCameraOrPlane,SIGNAL(clicked()),this,SLOT(slotChangFixState()));
	connect(butShowAllOrOne,SIGNAL(clicked()),this,SLOT(slotChangShowState()));
}
void CalibResultViewer::setButText()
{
	if (m_fixCamera)
		butFixCameraOrPlane->setText(FromCHN("固定棋盘"));
	else
		butFixCameraOrPlane->setText(FromCHN("固定相机"));
	if (m_showAll)
		butShowAllOrOne->setText(FromCHN("显示当前视图"));
	else
		butShowAllOrOne->setText(FromCHN("显示所有视图"));
}

void CalibResultViewer::setScene(cameraScene *calibScene)
{
	m_scene = calibScene;
	updateScene();
	
	m_sceneRender->setScene(calibScene);
}
void CalibResultViewer::setView(const int viewIdx)
{
	const int N = m_errPlot->graphCount();
	if (viewIdx != m_viewIdx && m_viewIdx>=0 && m_viewIdx< N) {
		m_errPlot->graph(m_viewIdx)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus,QColor("#0000ff"),6.0));
		m_errPlot->graph(m_viewIdx)->setVisible(m_showAll);
	}
	if (viewIdx>=0 && viewIdx< N) {
		m_errPlot->graph(viewIdx)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus,QColor("#ff0000"),6.0));
		m_errPlot->graph(viewIdx)->setVisible(true);
	}
	m_viewIdx = viewIdx;
	m_errPlot->replot();

	m_sceneRender->setView(viewIdx);
}
void CalibResultViewer::updateScene()
{
	m_errPlot->clearGraphs();
	if (m_scene != 0 && m_scene->viewCount() != 0) {
		cv::Mat K = cv::Mat::eye(3,3,CV_64F);
		cv::Mat distK = cv::Mat(5,1,CV_64F,m_scene->m_camera.k);
		K.at<double>(0) = m_scene->m_camera.fx;
		K.at<double>(4) = m_scene->m_camera.fy;
		K.at<double>(2) = m_scene->m_camera.cx;
		K.at<double>(5) = m_scene->m_camera.cy;
		double sizeX, sizeY;
		m_scene->getQuadSize(sizeX,sizeY);
		for (int i=0; i<m_scene->viewCount(); ++i) {
			m_errPlot->addGraph();
			cameraView *view = m_scene->m_views.at(i);
			if (view->isGridCornerPicked() && cv::norm(view->t)>10.0) {
				int nx, ny;
				std::vector<cv::Point2f> imgPts = view->getGridCorners(nx,ny);
				std::vector<cv::Point3f> worldPts;
				std::vector<cv::Point2f> projPts;
				QVector<double> dx;
				QVector<double> dy;
				for (int j=0; j<ny; ++j) {
					for (int i=0; i<nx; ++i) {
						cv::Point3f p(i*sizeX,j*sizeY,0);
						worldPts.push_back(p);
					}
				}
				cv::projectPoints(worldPts,
								  view->r,view->t,K,distK,
								  projPts);		
				for (int i=0; i<nx*ny; ++i) {
					dx.push_back(imgPts.at(i).x - projPts.at(i).x);
					dy.push_back(imgPts.at(i).y - projPts.at(i).y);
				}
				m_errPlot->graph(i)->setData(dx,dy);
				m_errPlot->graph(i)->setLineStyle(QCPGraph::lsNone);
				m_errPlot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus));
				m_errPlot->graph(i)->setVisible(m_showAll);
				imgPts.clear();
				worldPts.clear();
				projPts.clear();
				dx.clear();
				dy.clear();
			}
		}
		setView(-1);
	}
	m_errPlot->rescaleAxes();
	m_errPlot->replot();
	
	m_sceneRender->updateScene();
}

void CalibResultViewer::slotChangFixState()
{
	m_fixCamera = !m_fixCamera;
	m_sceneRender->setFixCamera(m_fixCamera);
	setButText();
}
void CalibResultViewer::slotChangShowState()
{
	m_showAll = !m_showAll;

	for (int i=0; i<m_errPlot->graphCount(); ++i)
		m_errPlot->graph(i)->setVisible(m_showAll);
	if (m_viewIdx >=0 && m_viewIdx<m_errPlot->graphCount())
		m_errPlot->graph(m_viewIdx)->setVisible(true);
	m_errPlot->replot();

	m_sceneRender->setShowAll(m_showAll);

	setButText();
}
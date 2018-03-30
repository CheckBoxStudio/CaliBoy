#include "panelresult.h"
#include <QSplitter>
#include <QLayout>

#include "../QCustomPlot/qcustomplot.h"
#include "scenerender.h"

#include "camera.h"
#include <cv.h>

PanelResult::PanelResult(cameraScene *calibScene, QWidget *parent)
    : QDockWidget(parent)
    , m_scene(0)
    , m_viewIdx(-1)
    , m_showAll(true)
    , m_fixCamera(true)
{
    setWindowTitle("Calibraion Result");
    setFeatures(QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    setupme();
    setScene(calibScene);
}
PanelResult::~PanelResult()
{
    m_scenePlot->clearGraphs();
}

void PanelResult::setupme()
{
    m_sceneRender = new SceneRender(0,this);

    m_scenePlot = new QCustomPlot(this);
    m_scenePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    m_scenePlot->xAxis->setLabel("dx");
    m_scenePlot->yAxis->setLabel("dy");
    m_scenePlot->axisRect()->setupFullAxesBox();

    butShowAllOrOne = new QPushButton(this);
    butFixCameraOrPlane = new QPushButton(this);
    setButText();

    QSplitter *splitter = new QSplitter(Qt::Vertical,this);
    QVBoxLayout *vLay = new QVBoxLayout();
    QHBoxLayout *hLay = new QHBoxLayout();

    splitter->addWidget(m_sceneRender);
    splitter->addWidget(m_scenePlot);
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
void PanelResult::setButText()
{
    if (m_fixCamera)
        butFixCameraOrPlane->setText("Fix Chessboard");
    else
        butFixCameraOrPlane->setText("Fix Camera");
    if (m_showAll)
        butShowAllOrOne->setText("Current View");
    else
        butShowAllOrOne->setText("All Views");
}

void PanelResult::setScene(cameraScene *calibScene)
{
    m_scene = calibScene;
    updateScene();

    m_sceneRender->setScene(calibScene);
}
void PanelResult::setView(const int viewIdx)
{
    const int N = m_scenePlot->graphCount();
    if (viewIdx != m_viewIdx && m_viewIdx>=0 && m_viewIdx< N) {
        m_scenePlot->graph(m_viewIdx)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus,QColor("#0000ff"),6.0));
        m_scenePlot->graph(m_viewIdx)->setVisible(m_showAll);
    }
    if (viewIdx>=0 && viewIdx< N) {
        m_scenePlot->graph(viewIdx)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus,QColor("#ff0000"),6.0));
        m_scenePlot->graph(viewIdx)->setVisible(true);
    }
    m_viewIdx = viewIdx;
    m_scenePlot->replot();

    m_sceneRender->setView(viewIdx);
}
void PanelResult::updateScene()
{
    m_scenePlot->clearGraphs();
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
            m_scenePlot->addGraph();
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
                m_scenePlot->graph(i)->setData(dx,dy);
                m_scenePlot->graph(i)->setLineStyle(QCPGraph::lsNone);
                m_scenePlot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus));
                m_scenePlot->graph(i)->setVisible(m_showAll);
                imgPts.clear();
                worldPts.clear();
                projPts.clear();
                dx.clear();
                dy.clear();
            }
        }
        setView(-1);
    }
    m_scenePlot->rescaleAxes();
    m_scenePlot->replot();

    m_sceneRender->updateScene();
}

void PanelResult::slotChangFixState()
{
    m_fixCamera = !m_fixCamera;
    m_sceneRender->setFixCamera(m_fixCamera);
    setButText();
}
void PanelResult::slotChangShowState()
{
    m_showAll = !m_showAll;

    for (int i=0; i<m_scenePlot->graphCount(); ++i)
        m_scenePlot->graph(i)->setVisible(m_showAll);
    if (m_viewIdx >=0 && m_viewIdx<m_scenePlot->graphCount())
        m_scenePlot->graph(m_viewIdx)->setVisible(true);
    m_scenePlot->replot();

    m_sceneRender->setShowAll(m_showAll);

    setButText();
}
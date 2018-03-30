#include "QImgViewer.h"
#include "uiconfig.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>

#include "qmsg.h"

#include "camera.h"
#include "chessboard.h"
#include "estimate.h"
#include "undistortion.h"

QImgViewer::QImgViewer(QWidget *parent)
    : QGraphicsView(parent)
    , m_view(0)
    , m_isPicked(0)
    , m_isPicking(false)
    , m_nX(0)
    , m_nY(0)
    , m_estFocal(100.0)
    , m_c(0)
    , m_winSizw(SZ_AUTO_CORNER)
    , m_barSize(SZ_AUTO_CORNER*1.5)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);
    setupme();
}
QImgViewer::~QImgViewer()
{
    cleanDrawed();
}

void QImgViewer::setView(cameraView *view)
{
    if (0 == view)
        cleanView();
    else {
        cleanDrawed();
        m_view = view;
        QImage *image = m_view->getQImage();
        m_GImageItem->setPixmap(QPixmap::fromImage(*image));
        m_GScene->setSceneRect(image->rect());
        if (m_view->isGridCornerPicked())
            copyGridCorners();

        emit signalViewChanged();
    }
}
void QImgViewer::cleanView()
{
    cleanDrawed();
    m_GImageItem->setPixmap(QPixmap());
    m_view = 0;
    emit signalViewChanged();
}

QImage* QImgViewer::getImage() 
{
    if (0 == m_view)
        return 0;
    else
        return m_view->getQImage();
}

void QImgViewer::setupme()
{
    setAlignment(Qt::AlignCenter);

    m_GScene = new QGraphicsScene(this);
    setScene(m_GScene);
    m_GScene->setBackgroundBrush(QBrush(QColor(C_BACKDOT), Qt::Dense7Pattern));
    m_GImageItem = m_GScene->addPixmap(QPixmap());
    m_GImageItem->setZValue(-2);
    m_GCordText[0] = m_GScene->addText("");
    m_GCordText[1] = m_GScene->addText("");
    m_GCordText[2] = m_GScene->addText("");
    m_GCordText[0]->setHtml("<h2><font color=red size=12>O</font></h2>");
    m_GCordText[1]->setHtml("<h2><font color=red size=12>X</font></h2>");
    m_GCordText[2]->setHtml("<h2><font color=red size=12>Y</font></h2>");
    m_GCordText[0]->setZValue(2);
    m_GCordText[1]->setZValue(2);
    m_GCordText[2]->setZValue(2);
    m_GCordText[0]->setVisible(false);
    m_GCordText[1]->setVisible(false);
    m_GCordText[2]->setVisible(false);

    connect(this, SIGNAL(signalCornerPicked(QPointF&)), this, SLOT(slotCornerPicked(QPointF&)));
    connect(this, SIGNAL(signalCornerRemoved()), this, SLOT(slotCornerRemoved()));
    connect(this, SIGNAL(signalDrawGridCorners()), this, SLOT(slotDrawGridCorners()));
}
void QImgViewer::cleanDrawed()
{
    hideCord();
    for (int i = 0; i<m_GBoundLines.size(); i++) {
        m_GScene->removeItem(m_GBoundLines.at(i));
        delete m_GBoundLines.at(i);
    }
    m_GBoundLines.clear();
    removeCorners(m_GScene, m_GGridCorners);
    removeCorners(m_GScene, m_GBoundCorners);

    m_boundCorners.clear();
    m_gridCorners.clear();
    m_isPicked = false;
    m_nX = 0;
    m_nY = 0;
    m_estFocal = 100.0;
    m_c = 0.0;
}
void QImgViewer::copyGridCorners()
{
    vector<cv::Point2f> corners = m_view->getGridCorners(m_nX, m_nY);
    for (int i = 0; i<corners.size(); ++i)
        m_gridCorners.push_back(corners.at(i));
    QPointF point;
    point.setX(m_gridCorners.at(0).x);
    point.setY(m_gridCorners.at(0).y);
    m_boundCorners.push_back(point);
    point.setX(m_gridCorners.at(m_nX*(m_nY - 1)).x);
    point.setY(m_gridCorners.at(m_nX*(m_nY - 1)).y);
    m_boundCorners.push_back(point);
    point.setX(m_gridCorners.at(m_nX*m_nY - 1).x);
    point.setY(m_gridCorners.at(m_nX*m_nY - 1).y);
    m_boundCorners.push_back(point);
    point.setX(m_gridCorners.at(m_nX - 1).x);
    point.setY(m_gridCorners.at(m_nX - 1).y);
    m_boundCorners.push_back(point);

    vector<QPointF> polyPoints;
    for (int i = 0; i<4; ++i) {
        CornerItem cornerItem =
            drawCorner(m_GScene, m_boundCorners[i],
                SZ_CORNER_CROSS_R, SZ_CORNER_SQURE_R,
                C_CORNER_BOUNDCORNER_CROSS, C_CORNER_BOUNDCORNER_SQURE,
                1);
        m_GBoundCorners.push_back(cornerItem);
        polyPoints.push_back(m_boundCorners[i]);
    }
    drawPoly(m_GScene, polyPoints, m_GBoundLines, C_CORNER_BOUNDLINE, -1);
    slotDrawGridCorners();

    m_isPicked = true;
}
void QImgViewer::hideCord()
{
    m_GCordText[0]->setVisible(false);
    m_GCordText[1]->setVisible(false);
    m_GCordText[2]->setVisible(false);
}
void QImgViewer::updateCord()
{
    QPointF dY = m_boundCorners[1]-m_boundCorners[0];
    QPointF dX = m_boundCorners[3]-m_boundCorners[0];
    QPointF pO = m_boundCorners[0] - (dY+dX)/sqrtf( (dY+dX).x()*(dY+dX).x() +(dY+dX).y()*(dY+dX).y() ) * 40;
    QPointF pY = m_boundCorners[0] + dY/2.0 - dX/sqrtf(dX.x()*dX.x() + dX.y()*dX.y())*30;
    QPointF pX = m_boundCorners[0] + dX/2.0 - dY/sqrtf(dY.x()*dY.x() + dY.y()*dY.y())*30;
    m_GCordText[0]->setPos(pO);
    m_GCordText[1]->setPos(pX);
    m_GCordText[2]->setPos(pY);
    m_GCordText[0]->setVisible(true);
    m_GCordText[1]->setVisible(true);
    m_GCordText[2]->setVisible(true);
}

void QImgViewer::mouseMoveEvent(QMouseEvent *e)
{
    QPointF point = mapToScene(e->pos());
    emit signalFocusChanged(point.x(), point.y());
}
void QImgViewer::wheelEvent(QWheelEvent *e)
{
    QGraphicsView::wheelEvent(e);
    QPointF point = mapToScene(e->pos());
    emit signalFocusChanged(point.x(), point.y());
}
void QImgViewer::mousePressEvent(QMouseEvent *e)
{
    if (m_isPicking && 
    Qt::LeftButton == e->button() &&
    m_boundCorners.size()<4) {

        QPointF point = mapToScene(e->pos());
        if (point.x() >= 5 && point.x() < m_GScene->width()-5 &&
            point.y() >= 5 && point.y() < m_GScene->height()-5) {

            cv::Point2f corner = cv::Point2f(point.x(),point.y());
            vector<cv::Point2f> corners;
            corners.push_back(corner);
            cornerSubPix(m_view->getMatImage(), corners,
            cvSize(m_winSizw,m_winSizw),cvSize(-1,-1),
            cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));
            corner = corners.at(0);
            corners.clear();

            point.setX(corner.x);
            point.setY(corner.y);

            _msgOut(QString("Pick corner at [%1,%2]->[%3,%4].")
                .arg(point.x()).arg(point.y())
                .arg(corner.x).arg(corner.y));
            emit signalCornerPicked(point);
        }
    }
}
void QImgViewer::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_isPicking && 
        Qt::RightButton == e->button() && 
        m_boundCorners.size()>0) {
        _msgOut("Quit the last corner.");
        emit signalCornerRemoved();
    }
}

void QImgViewer::slotCornerPicked(QPointF &corner)
{
    m_boundCorners.push_back(corner);
    CornerItem cornerItem 
        = drawCorner(m_GScene,corner,
            SZ_CORNER_CROSS_R,SZ_CORNER_SQURE_R,
            C_CORNER_BOUNDCORNER_CROSS,C_CORNER_BOUNDCORNER_SQURE,
            1);
    m_GBoundCorners.push_back(cornerItem);
    if (m_GBoundCorners.size()>=2) {
        QGraphicsLineItem *line = 
            m_GScene->addLine(
                (m_boundCorners.end()-2)->x(), 
                (m_boundCorners.end()-2)->y(),
                corner.x(),corner.y(),
                QPen(QColor(C_CORNER_BOUNDLINE)));
        line->setZValue(-1);
        m_GBoundLines.push_back(line);
    }


    if (m_GBoundCorners.size() == 4) {
        QGraphicsLineItem *line = 
            m_GScene->addLine(m_boundCorners[0].x(), m_boundCorners[0].y(),
                corner.x(), corner.y(),
                QPen(QColor(C_CORNER_BOUNDLINE)));
        m_GBoundLines.push_back(line);
        line->setZValue(-1);

        findGridCorners();
    }
}
void QImgViewer::slotCornerRemoved()
{
    m_boundCorners.pop_back();

    removeCorner(m_GScene,(*(m_GBoundCorners.end()-1)));
    m_GBoundCorners.pop_back();

    if (m_boundCorners.size() >= 1) {
        m_GScene->removeItem(*(m_GBoundLines.end()-1));
        delete *(m_GBoundLines.end()-1);
        m_GBoundLines.pop_back();
    }

    if (m_boundCorners.size() == 3){
        hideCord();
        removeCorners(m_GScene, m_GGridCorners);
        m_GScene->removeItem(*(m_GBoundLines.end()-1));
        delete *(m_GBoundLines.end()-1);
        m_GBoundLines.pop_back();

        emit signalFinished(false);
    }
}
void QImgViewer::slotDrawGridCorners()
{
    removeCorners(m_GScene, m_GGridCorners);
    for (int i=0; i<m_gridCorners.size(); ++i) {
        QPointF point = QPointF(m_gridCorners.at(i).x,m_gridCorners.at(i).y);
        CornerItem cornerItem 
            = drawCorner(m_GScene,point,
                SZ_CORNER_CROSS_R,SZ_CORNER_SQURE_R,
                C_CORNER_CROSS,C_CORNER_SQURE);
        m_GGridCorners.push_back(cornerItem);
    }
    updateCord();
}

void QImgViewer::accept()
{
    m_isPicking = false;
    setCursor(Qt::ArrowCursor);
    m_view->setGridCorners(m_nX, m_nY, m_gridCorners);
}
void QImgViewer::reject()
{
    cleanDrawed();
    if (m_view->isGridCornerPicked()) {
        copyGridCorners();
        emit signalViewChanged();
    }

    m_isPicking = false;
    setCursor(Qt::ArrowCursor);
}
void QImgViewer::pickCorners()
{
    msgOut("\n>> Pick Corners");
    msgOut(">> Pleace pick 4 inner corners by Left Click (Quit by Right Click) :");
    cleanDrawed();
    m_isPicking = true;
    setCursor(Qt::CrossCursor);
    emit signalViewChanged();
}
void QImgViewer::setCornerWinSize(int s)
{
    if (s>2) {
        m_winSizw = s;
        msgOut(QString(">> Set corner nerborhood windowsize->%1=2x%2+1.").arg(m_winSizw*2+1).arg(m_winSizw));
    }
}
void QImgViewer::setBarWinSize(int s)
{
    if (s>2) {
        m_barSize = s;
        msgOut(QString(">> Cut the image by pixels->%1").arg(m_barSize));
        if (m_GBoundCorners.size() == 4) {
            hideCord();
            removeCorners(m_GScene, m_GGridCorners);
            findGridCorners();
        }
    }
}
void QImgViewer::undistortionCorner(double c)
{
    if (m_boundCorners.size() != 4)
        return;
    if (m_c != c)
        findUdistortionGridCorners(c);
}
void QImgViewer::findGridCorners()
{
    vector<cv::Point2f> imgPoints;
    for (int i=0; i<4; ++i)
        imgPoints.push_back(cv::Point2f(m_boundCorners[i].x(),m_boundCorners[i].y()));
    int n_x1 = countSquares(m_view->getMatImage(),imgPoints[0],imgPoints[3],m_barSize);
    int n_x2 = countSquares(m_view->getMatImage(),imgPoints[1],imgPoints[2],m_barSize);
    int n_y1 = countSquares(m_view->getMatImage(),imgPoints[0],imgPoints[1],m_barSize);
    int n_y2 = countSquares(m_view->getMatImage(),imgPoints[3],imgPoints[2],m_barSize);
    if (n_x1 != n_x2 || n_y1 != n_y2) {
        msgOut(">> The number of grid is not match£¡");
        return;
    }

    m_nX = n_x1 + 2;
    m_nY = n_y1 + 2;
    estFocal();
    findUdistortionGridCorners();

    msgOut(QString(">> Number of corners: %1 * %2.").arg(m_nX).arg(m_nY));
    msgOut(QString(">> The estimated focak f: %1 pixels.").arg(m_estFocal));
    emit signalFinished(true);
}
void QImgViewer::estFocal()
{
    double half_w = (m_view->width()-1) / 2.0 ;
    double half_h = (m_view->height()-1) / 2.0 ;
    vector<cv::Point2f> boundCorners;
    for (int i=0; i<4; ++i) {
        cv::Point2f point;
        point.x = m_boundCorners[i].x() - half_w;
        point.y = m_boundCorners[i].y() - half_h;
        boundCorners.push_back(point);
    }

    m_estFocal = estFocal_Rect(boundCorners,m_nY,m_nX);
    boundCorners.clear();
}
void QImgViewer::findUdistortionGridCorners(const double c)
{
    m_c = c;
    vector<cv::Point2f> imgPoints;
    double half_w = (m_view->width()-1) / 2.0 ;
    double half_h = (m_view->height()-1) / 2.0 ;
    for (int i=0; i<4; ++i) {
        cv::Point2f point;
        point.x = (m_boundCorners[i].x() - half_w) / m_estFocal;
        point.y = (m_boundCorners[i].y() - half_h) / m_estFocal;
        imgPoints.push_back(point);
    }

    removeDistortion2(imgPoints,m_c);
    findChessboardCornersHomography(imgPoints,m_gridCorners,m_nX,m_nY);
    addDistortion2(m_gridCorners,m_c);

    for (int i=0; i<m_gridCorners.size(); ++i) {
        m_gridCorners.at(i).x = m_gridCorners.at(i).x * m_estFocal + half_w;
        m_gridCorners.at(i).y = m_gridCorners.at(i).y * m_estFocal + half_h;
    }

    cornerSubPix(
    m_view->getMatImage(), m_gridCorners, 
    cvSize(SZ_AUTO_CORNER,SZ_AUTO_CORNER), cvSize(-1,-1), 
    cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1) );

    emit signalDrawGridCorners();
}
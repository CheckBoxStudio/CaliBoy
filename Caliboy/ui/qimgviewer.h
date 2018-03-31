#ifndef QIMGVIEWER_H
#define QIMGVIEWER_H

#include <QGraphicsView>
#include "girdcorner.h"
#include <cv.h>

class QGraphicsPixmapItem;
class QGraphicsLineItem;
class QGraphicsRectItem;
class QGraphicsTextItem;
class QMouseEvent;

class cameraView;

class QImgViewer : public QGraphicsView
{
Q_OBJECT

public:
    QImgViewer(QWidget *parent = 0);
    ~QImgViewer();

    void setView(cameraView *view);
    void cleanView();

    void pickCorners();
    void setCornerWinSize(int s);
    void setBarWinSize(int s);
    void undistortionCorner(double c);
    void accept();
    void reject();

    bool isGridCornersFound() {return m_isPicked;}
    bool isPicking() {return m_isPicking;}
    int getPickedNum() {return m_boundCorners.size();}
    vector<QPointF>&
        getBoundCorners() {return m_boundCorners;}
    vector<cv::Point2f>& 
        getGridCorners() {return m_gridCorners;}
    int getNX() {return m_nX;}
    int getNY() {return m_nY;}
    QImage*getImage();
private:
    cameraView *m_view;

    vector<QPointF> m_boundCorners;
    vector<cv::Point2f> m_gridCorners;
    bool m_isPicking;
    bool m_isPicked;
    int m_nX, m_nY;
    double m_estFocal;
    double m_c;
    int m_winSizw;
    int m_barSize;
private:
    void setupme();
    void cleanDrawed();
    void copyGridCorners();
    void hideCord();
    void updateCord();

    void findGridCorners();
    void estFocal();
    void findUdistortionGridCorners(const double c = 0);
private:
    QGraphicsScene *m_GScene;
    QGraphicsPixmapItem *m_GImageItem;
    QGraphicsTextItem *m_GCordText[3];
    vector<CornerItem> m_GBoundCorners;
    vector<QGraphicsLineItem *> m_GBoundLines;
    vector<CornerItem> m_GGridCorners;
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private slots:
    void slotCornerPicked(QPointF &corner);
    void slotCornerRemoved();
    void slotDrawGridCorners();
signals:
    void signalViewChanged();
    void signalFocusChanged(int x, int y);
    void signalCornerPicked(QPointF &corner);
    void signalCornerRemoved();
    void signalDrawGridCorners();

    void signalFinished(bool ok);
};

#endif// !QIMGVIEWER_H

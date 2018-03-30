#ifndef QZOOMVIEW_H
#define QZOOMVIEW_H

#include <QWidget>
#include "girdcorner.h"

class QLabel;
class QLineEdit;

class QGraphicsScene;
class QGraphicsView;
class QGraphicsPixmapItem;
class QGraphicsLineItem;
class QGraphicsRectItem;

class QImgViewer;

class QImgZoomViewer : public QWidget
{
Q_OBJECT

public:
    QImgZoomViewer(QWidget *parent = 0);
    ~QImgZoomViewer();

    void setZoomSourceView(QImgViewer *view);   /* Set view to be zoom. */
private:
    void setupme();                             /* Setup UI. */
    void copyGridCorners();                     /* Copy the grid corners. */
    void cleanDrawed();                         /* Clean all corners. */

    void updatePos();                           /* Update location. */

private:
    const int m_scale;                          /* Scale ratio. */

    QImgViewer *m_sourceView;                   /* View to be zoom. */
    int mx, my;                                 /* Location to check. */
private:
    QGraphicsScene *m_GScene;
    QGraphicsView *m_GView;
    QGraphicsPixmapItem *m_GPixItem;
    QLabel *m_labelX;
    QLabel *m_labelY;
    QLineEdit *m_XLoc;
    QLineEdit *m_YLoc;

    QGraphicsLineItem *m_GCorssItem[2];
    vector<CornerItem> m_GBoundCorners;
    vector<CornerItem> m_GGridCorners;
    vector<QGraphicsLineItem *> m_GBoundLines;
private slots:
    void slotViewChanged();
    void slotFocusChanged(int x, int y);
    void slotCornerPicked(QPointF &corner);
    void slotCornerRemoved();
    void slotDrawGridCorners();
};

#endif// !QZOOMVIEW_H

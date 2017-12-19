#ifndef QIMGVIEWER_H
#define QIMGVIEWER_H

#include <QGraphicsView>
#include "uihelper.h"
#include <cv.h>

class QGraphicsPixmapItem;
class QGraphicsLineItem;
class QGraphicsRectItem;
class QGraphicsTextItem;
class QMouseEvent;

class cameraView;

class qimgviewer : public QGraphicsView
{
	Q_OBJECT

public:
	qimgviewer(QWidget *parent = 0);
	~qimgviewer();

	/* 场景设置调用 */
	void setView(cameraView *view);			/* 设置显示视图 */
	void cleanView();						/* 清除显示视图 */

	/* 功能调用 */
	void pickCorners();						/* 执行角点检测 */
	void setCornerWinSize(int s);			/* 设置亚像素角点窗口 */
	void setBarWinSize(int s);				/* 设置条带窗口 */
	void undistortionCorner(double c);		/* 畸变参数调整 */
	void accept();							/* 结果被接受 */
	void reject();							/* 结果被拒绝 */

	bool		isGridCornersFound() {return m_isPicked;}
	bool		isPicking() {return m_isPicking;}
	int			getPickedNum() {return m_boundCorners.size();}
	QImage*		getImage();
	vector<QPointF>&
				getBoundCorners() {return m_boundCorners;}
	vector<cv::Point2f>& 
				getGridCorners() {return m_gridCorners;}
	int getNX() {return m_nX;}
	int getNY() {return m_nY;}
private:
	cameraView *m_view;					/* 显示场景<动态_数元> */
	
	vector<QPointF> m_boundCorners;		/* 已选择角点<动态_数元> */
	vector<cv::Point2f> m_gridCorners;	/* 所有网格角点<动态_数元> */
	bool m_isPicking;					/* 状态<动态_数元> */
	bool m_isPicked;					/* 结果<动态_数元> */
	int m_nX, m_nY;						/* 网格尺寸<动态_数元> */
	double m_estFocal;					/* 估计像素焦距<动态_数元> */
	double m_c;							/* 畸变参数<动态_数元> */
	int m_winSizw;						/* 窗口大小<动态_数元> */
	int m_barSize;						/* 条带宽度<动态_数元> */
private:
	void setupme();						/* 界面绘制 */
	void cleanDrawed();					/* 清理所有绘制 */
	void copyGridCorners();				/* 从视图复制整套角点 */
	void hideText();					/* 隐藏坐标文字显示 */
	void replaceText();					/* 调整坐标文字位置并显示 */

	void findGridCorners();				/* 检测角点 */
	void estFocal();					/* 根据4个边界点估计像素焦距 */
	void findUdistortionGridCorners(const double c = 0);/* 畸变调整 */
private:
	QGraphicsScene *m_GScene;					/* 画布[常驻_图元] */
	QGraphicsPixmapItem *m_GImageItem;			/* 底图[常驻_图元] */
	QGraphicsTextItem *m_GText[3];				/* 坐标标注[常驻_图元] */
	vector<CornerItem> m_GBoundCorners;			/* 边界点<动态_图元> */
	vector<QGraphicsLineItem *> m_GBoundLines;	/* 边界线<动态_图元> */
	vector<CornerItem> m_GGridCorners;			/* 所有角点<动态_图元> */
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
private slots:
	void slotCornerPicked(QPointF &corner);		/* 新拾取一个点 */
	void slotCornerRemoved();					/* 撤销一个点 */
	void slotDrawGridCorners();					/* 绘制(重绘)网格 */
signals:
	/* 给放大器的信号 */
	void signalViewChanged();
	void signalFocusChanged(int x, int y);
	void signalCornerPicked(QPointF &corner);
	void signalCornerRemoved();
	void signalDrawGridCorners();

	/* 状态信号 */
	void signalFinished(bool ok);
};

#endif // QIMGVIEWER_H

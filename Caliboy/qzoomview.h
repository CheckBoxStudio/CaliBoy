#ifndef QZOOMVIEW_H
#define QZOOMVIEW_H

#include <QWidget>
#include <uihelper.h>

class QLabel;
class QLineEdit;

class QGraphicsScene;
class QGraphicsView;
class QGraphicsPixmapItem;
class QGraphicsLineItem;
class QGraphicsRectItem;

class qimgviewer;

class qzoomview : public QWidget
{
	Q_OBJECT

public:
	qzoomview(QWidget *parent = 0);
	~qzoomview();

	void setZoomSourceView(qimgviewer *view);	/* 设置放大源控件 */
private:
	void setupme();								/* 界面绘制 */
	void copyGridCorners();						/* 从放大源复制整套角点 */
	void cleanDrawed();							/* 清理所有绘制 */

	void updatePos();							/* 刷新位移显示 */

private:
	const int m_scale;							/* 放大尺度[定值_数元] */

	qimgviewer *m_sourceView;					/* 放大源<动态_数元> */	
	int mx, my;									/* 放大区域坐标<动态_数元> */
private:
	QGraphicsScene *m_GScene;					/* 画布[常驻_图元] */
	QGraphicsView *m_GView;						/* 视图[常驻_图元] */
	QGraphicsPixmapItem *m_GPixItem;			/* 底图[常驻_图元] */
	QLabel *m_labelX;							/* X坐标标识[常驻_图元] */
	QLabel *m_labelY;							/* Y坐标标识[常驻_图元] */
	QLineEdit *m_XLoc;							/* X坐标显示[常驻_图元] */
	QLineEdit *m_YLoc;							/* Y坐标显示[常驻_图元] */

	QGraphicsLineItem *m_GCorssItem[2];			/* 定位十字[常驻_图元] */
	vector<CornerItem> m_GBoundCorners;			/* 边界角点<动态_图元> */
	vector<CornerItem> m_GGridCorners;			/* 网格角点<动态_图元> */
	vector<QGraphicsLineItem *> m_GBoundLines;	/* 边界线<动态_图元> */
private slots:
	void slotViewChanged();						/* 放大源图像更改 */
	void slotFocusChanged(int x, int y);		/* 放大焦点更改 */
	void slotCornerPicked(QPointF &corner);		/* 新拾取一个点 */
	void slotCornerRemoved();					/* 撤销一个点 */
	void slotDrawGridCorners();					/* 绘制(重绘)网格 */
};

#endif // QZOOMVIEW_H

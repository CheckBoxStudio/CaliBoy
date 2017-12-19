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

	/* �������õ��� */
	void setView(cameraView *view);			/* ������ʾ��ͼ */
	void cleanView();						/* �����ʾ��ͼ */

	/* ���ܵ��� */
	void pickCorners();						/* ִ�нǵ��� */
	void setCornerWinSize(int s);			/* ���������ؽǵ㴰�� */
	void setBarWinSize(int s);				/* ������������ */
	void undistortionCorner(double c);		/* ����������� */
	void accept();							/* ��������� */
	void reject();							/* ������ܾ� */

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
	cameraView *m_view;					/* ��ʾ����<��̬_��Ԫ> */
	
	vector<QPointF> m_boundCorners;		/* ��ѡ��ǵ�<��̬_��Ԫ> */
	vector<cv::Point2f> m_gridCorners;	/* ��������ǵ�<��̬_��Ԫ> */
	bool m_isPicking;					/* ״̬<��̬_��Ԫ> */
	bool m_isPicked;					/* ���<��̬_��Ԫ> */
	int m_nX, m_nY;						/* ����ߴ�<��̬_��Ԫ> */
	double m_estFocal;					/* �������ؽ���<��̬_��Ԫ> */
	double m_c;							/* �������<��̬_��Ԫ> */
	int m_winSizw;						/* ���ڴ�С<��̬_��Ԫ> */
	int m_barSize;						/* �������<��̬_��Ԫ> */
private:
	void setupme();						/* ������� */
	void cleanDrawed();					/* �������л��� */
	void copyGridCorners();				/* ����ͼ�������׽ǵ� */
	void hideText();					/* ��������������ʾ */
	void replaceText();					/* ������������λ�ò���ʾ */

	void findGridCorners();				/* ���ǵ� */
	void estFocal();					/* ����4���߽��������ؽ��� */
	void findUdistortionGridCorners(const double c = 0);/* ������� */
private:
	QGraphicsScene *m_GScene;					/* ����[��פ_ͼԪ] */
	QGraphicsPixmapItem *m_GImageItem;			/* ��ͼ[��פ_ͼԪ] */
	QGraphicsTextItem *m_GText[3];				/* �����ע[��פ_ͼԪ] */
	vector<CornerItem> m_GBoundCorners;			/* �߽��<��̬_ͼԪ> */
	vector<QGraphicsLineItem *> m_GBoundLines;	/* �߽���<��̬_ͼԪ> */
	vector<CornerItem> m_GGridCorners;			/* ���нǵ�<��̬_ͼԪ> */
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
private slots:
	void slotCornerPicked(QPointF &corner);		/* ��ʰȡһ���� */
	void slotCornerRemoved();					/* ����һ���� */
	void slotDrawGridCorners();					/* ����(�ػ�)���� */
signals:
	/* ���Ŵ������ź� */
	void signalViewChanged();
	void signalFocusChanged(int x, int y);
	void signalCornerPicked(QPointF &corner);
	void signalCornerRemoved();
	void signalDrawGridCorners();

	/* ״̬�ź� */
	void signalFinished(bool ok);
};

#endif // QIMGVIEWER_H

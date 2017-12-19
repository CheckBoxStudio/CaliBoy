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

	void setZoomSourceView(qimgviewer *view);	/* ���÷Ŵ�Դ�ؼ� */
private:
	void setupme();								/* ������� */
	void copyGridCorners();						/* �ӷŴ�Դ�������׽ǵ� */
	void cleanDrawed();							/* �������л��� */

	void updatePos();							/* ˢ��λ����ʾ */

private:
	const int m_scale;							/* �Ŵ�߶�[��ֵ_��Ԫ] */

	qimgviewer *m_sourceView;					/* �Ŵ�Դ<��̬_��Ԫ> */	
	int mx, my;									/* �Ŵ���������<��̬_��Ԫ> */
private:
	QGraphicsScene *m_GScene;					/* ����[��פ_ͼԪ] */
	QGraphicsView *m_GView;						/* ��ͼ[��פ_ͼԪ] */
	QGraphicsPixmapItem *m_GPixItem;			/* ��ͼ[��פ_ͼԪ] */
	QLabel *m_labelX;							/* X�����ʶ[��פ_ͼԪ] */
	QLabel *m_labelY;							/* Y�����ʶ[��פ_ͼԪ] */
	QLineEdit *m_XLoc;							/* X������ʾ[��פ_ͼԪ] */
	QLineEdit *m_YLoc;							/* Y������ʾ[��פ_ͼԪ] */

	QGraphicsLineItem *m_GCorssItem[2];			/* ��λʮ��[��פ_ͼԪ] */
	vector<CornerItem> m_GBoundCorners;			/* �߽�ǵ�<��̬_ͼԪ> */
	vector<CornerItem> m_GGridCorners;			/* ����ǵ�<��̬_ͼԪ> */
	vector<QGraphicsLineItem *> m_GBoundLines;	/* �߽���<��̬_ͼԪ> */
private slots:
	void slotViewChanged();						/* �Ŵ�Դͼ����� */
	void slotFocusChanged(int x, int y);		/* �Ŵ󽹵���� */
	void slotCornerPicked(QPointF &corner);		/* ��ʰȡһ���� */
	void slotCornerRemoved();					/* ����һ���� */
	void slotDrawGridCorners();					/* ����(�ػ�)���� */
};

#endif // QZOOMVIEW_H

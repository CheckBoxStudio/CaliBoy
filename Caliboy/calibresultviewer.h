/* calibresultviewer.h
 * �鿴�궨���
 */
#ifndef CALIB_RESULT_VIEWER_H
#define CALIB_RESULT_VIEWER_H

#include <QDockWidget>


class QPushButton;

class QCustomPlot;
class SceneRender;

class cameraScene;

class CalibResultViewer : public QDockWidget
{
	Q_OBJECT

public:
	CalibResultViewer(cameraScene *calibScene, QWidget *parent = 0);
	~CalibResultViewer();

	/* �ⲿ���ýӿ� */
	void setScene(cameraScene *calibScene);	/* ������ʾ���� */
	void setView(const int viewIdx);		/* ������ʾ��ͼ */
	void updateScene();						/* ������ʾ���� */
private:
	cameraScene *m_scene;					/* ��ʾ���� */
	int m_viewIdx;							/* ��ǰ��ʾ��ͼ���� */
	bool m_showAll;							/* ��ʾ������ͼ */
	bool m_fixCamera;						/* �̶������ʾ */
private:
	void setupme();							/* ������� */
	void setButText();						/* ������ť���� */
private slots:
	void slotChangFixState();
	void slotChangShowState();
private:
	SceneRender *m_sceneRender;				/* ������Ⱦ */
	QCustomPlot *m_errPlot;					/* ��ͶӰ��� */
	QPushButton *butShowAllOrOne;
	QPushButton *butFixCameraOrPlane;
};

#endif // CALIB_RESULT_VIEWER_H

/* calibresultviewer.h
 * 查看标定结果
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

	/* 外部调用接口 */
	void setScene(cameraScene *calibScene);	/* 设置显示场景 */
	void setView(const int viewIdx);		/* 设置显示视图 */
	void updateScene();						/* 更新显示场景 */
private:
	cameraScene *m_scene;					/* 显示场景 */
	int m_viewIdx;							/* 当前显示视图索引 */
	bool m_showAll;							/* 显示所有视图 */
	bool m_fixCamera;						/* 固定相机显示 */
private:
	void setupme();							/* 界面绘制 */
	void setButText();						/* 调整按钮文字 */
private slots:
	void slotChangFixState();
	void slotChangShowState();
private:
	SceneRender *m_sceneRender;				/* 场景渲染 */
	QCustomPlot *m_errPlot;					/* 重投影误差 */
	QPushButton *butShowAllOrOne;
	QPushButton *butFixCameraOrPlane;
};

#endif // CALIB_RESULT_VIEWER_H

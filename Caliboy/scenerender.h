/* scenerender.h
 * 采用OpenGL渲染标定得到的相机与棋盘格间相对位置关系[R_i | t_i].
 */

#ifndef SCENE_RENDER_H
#define SCENE_RENDER_H

#include <QGLWidget>

class cameraScene;

class SceneRender : public QGLWidget
{
	Q_OBJECT

public:
	SceneRender(cameraScene *calibScene, QWidget *parent = 0);
	~SceneRender();

	/* 外部调用接口 */
	void setScene(cameraScene *calibScene);	/* 设置显示场景 */
	void setView(const int viewIdx);		/* 设置当前视图 */
	void updateScene();						/* 更新场景 */
	void setFixCamera(bool bOn);			/* 设置相机固定 */
	void setShowAll(bool bOn);				/* 设置显示所有视图 */
private:
	void initData();						/* 初始化成员变量 */
	void drawScene_CameraFixed();			/* 固定相机作图 */
	void drawScene_PlaneFixed();			/* 固定棋盘格作图 */
	void drawCoordinate(const double l);	/* 绘制坐标轴:RGB颜色对应XYZ轴 */
	void drawCamera(const double f, const double halfx, const double halfy);		/* 绘制相机 */
	void drawPlane(const int nX, const int nY, const double lX, const double lY);	/* 绘制棋盘格 */
private:
	cameraScene *m_scene;		/* 显示场景 */
	int m_viewIdx;				/* 当前显示视图索引 */
	bool m_fixCamera;			/* 固定相机显示 */
	bool m_showAll;				/* 显示所有视图 */
	
	double m_l;					/* 长度参考:自棋盘格尺寸 */
	double m_fov;				/* 绘制视图的视场角 */
	double m_dist;				/* 相机与场景间的距离,控制远近以调节大小 */
	double m_angleX;			/* 查看旋转角X */
	double m_angleY;			/* 查看旋转角Y */
	bool m_bRotating;			/* 是否处于旋转中 */
	double _mx,_my;				/* 交互操作鼠标位置参看 */	

protected:
	void initializeGL();						/* 初始化 */
	void paintGL();								/* 绘图主函数 */
	void resizeGL( int width, int height );		/* 调整大小 */

	/* 交互查看 */
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);	
	void wheelEvent(QWheelEvent *e);
};

#endif // SCENE_RENDER_H

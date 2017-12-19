/* scenerender.h
 * ����OpenGL��Ⱦ�궨�õ�����������̸�����λ�ù�ϵ[R_i | t_i].
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

	/* �ⲿ���ýӿ� */
	void setScene(cameraScene *calibScene);	/* ������ʾ���� */
	void setView(const int viewIdx);		/* ���õ�ǰ��ͼ */
	void updateScene();						/* ���³��� */
	void setFixCamera(bool bOn);			/* ��������̶� */
	void setShowAll(bool bOn);				/* ������ʾ������ͼ */
private:
	void initData();						/* ��ʼ����Ա���� */
	void drawScene_CameraFixed();			/* �̶������ͼ */
	void drawScene_PlaneFixed();			/* �̶����̸���ͼ */
	void drawCoordinate(const double l);	/* ����������:RGB��ɫ��ӦXYZ�� */
	void drawCamera(const double f, const double halfx, const double halfy);		/* ������� */
	void drawPlane(const int nX, const int nY, const double lX, const double lY);	/* �������̸� */
private:
	cameraScene *m_scene;		/* ��ʾ���� */
	int m_viewIdx;				/* ��ǰ��ʾ��ͼ���� */
	bool m_fixCamera;			/* �̶������ʾ */
	bool m_showAll;				/* ��ʾ������ͼ */
	
	double m_l;					/* ���Ȳο�:�����̸�ߴ� */
	double m_fov;				/* ������ͼ���ӳ��� */
	double m_dist;				/* ����볡����ľ���,����Զ���Ե��ڴ�С */
	double m_angleX;			/* �鿴��ת��X */
	double m_angleY;			/* �鿴��ת��Y */
	bool m_bRotating;			/* �Ƿ�����ת�� */
	double _mx,_my;				/* �����������λ�òο� */	

protected:
	void initializeGL();						/* ��ʼ�� */
	void paintGL();								/* ��ͼ������ */
	void resizeGL( int width, int height );		/* ������С */

	/* �����鿴 */
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);	
	void wheelEvent(QWheelEvent *e);
};

#endif // SCENE_RENDER_H

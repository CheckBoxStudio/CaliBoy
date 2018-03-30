/* scenerender.h
 * Render the views in 3D by OpenGL with parameters [R_i | t_i].
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

    void setScene(cameraScene *calibScene);
    void setView(const int viewIdx);
    void updateScene();
    void setFixCamera(bool bOn);
    void setShowAll(bool bOn);
private:
    void initData();
    void drawScene_CameraFixed();
    void drawScene_PlaneFixed();
    void drawCoordinate(const double l);
    void drawCamera(const double f, const double halfx, const double halfy);
    void drawPlane(const int nX, const int nY, const double lX, const double lY);
private:
    cameraScene *m_scene;
    int m_viewIdx;
    bool m_fixCamera;
    bool m_showAll;

    double m_l;
    double m_fov;
    double m_dist;
    double m_angleX;
    double m_angleY;
    bool m_bRotating;
    double _mx,_my;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
};

#endif// !SCENE_RENDER_H

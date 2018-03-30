/* panelresult.h
 * Check Result.
 */
#ifndef CALIB_RESULT_VIEWER_H
#define CALIB_RESULT_VIEWER_H

#include <QDockWidget>


class QPushButton;

class QCustomPlot;
class SceneRender;

class cameraScene;

class PanelResult : public QDockWidget
{
Q_OBJECT

public:
    PanelResult(cameraScene *calibScene, QWidget *parent = 0);
    ~PanelResult();

    void setScene(cameraScene *calibScene); /* Set scene. */
    void setView(const int viewIdx);        /* Set view in scene. */
    void updateScene();                     /* Update scene */
private:
    cameraScene *m_scene;                   /* Scene data. */
    int m_viewIdx;                          /* Index for view in scene. */
    bool m_showAll;                         /* Whether to display all view. */
    bool m_fixCamera;                       /* Whether to fix the camera for displaying. */
private:
    void setupme();                         /* Setup UI. */
    void setButText();                      /* Update text in buttons. */
private slots:
    void slotChangFixState();
    void slotChangShowState();
private:
    SceneRender *m_sceneRender;             /* Render scene. */
    QCustomPlot *m_scenePlot;               /* Plot scene. */
    QPushButton *butShowAllOrOne;
    QPushButton *butFixCameraOrPlane;
};

#endif // CALIB_RESULT_VIEWER_H

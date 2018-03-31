/* panelruntime.h
 * Runtime panel.
 */
#ifndef INFO_PANEL_H
#define INFO_PANEL_H

#include <QDockWidget>

class QTextBrowser;
class QLabel;
class QLineEdit;

class camera;
class cameraView;

class PanelRuntime : public QDockWidget
{
Q_OBJECT

public:
    PanelRuntime(QWidget *parent = 0);
    ~PanelRuntime();

    QTextBrowser *getBrowser() {return m_textBrowser;};
    void updateCamera(camera *cam);
    void updateCameraView(cameraView *view);
private:
    void setupme();
private:
    QWidget *dockWidgetContents;
    QTextBrowser *m_textBrowser;
    /* sm = K[R | t]M
     * From M to m:
     * 1 - Rotate by R first,
     * 2 - and then translate by t.
     */
    QLabel *m_label_K;         // intrinsic parameters.
    QLabel *m_label_k;         // Distortion parameters[k1,k2,p1,p2,k3]
    QLabel *m_label_om;        // 3x1 Rotation vector.
    QLabel *m_label_R;         // 3x3 Rotation matrix.
    QLabel *m_label_t;         // 3x1 Translation vector.

    QTextBrowser *m_edit_K;
    QLineEdit *m_edit_k;
    QLineEdit *m_edit_om;
    QTextBrowser *m_edit_R;
    QLineEdit *m_edit_t;
};

#endif// !INFO_PANEL_H
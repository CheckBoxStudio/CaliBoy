/* infopanel.h
 * 相机参数标定结果数值显示.
 */
#ifndef INFO_PANEL_H
#define INFO_PANEL_H

#include <QDockWidget>

class QTextBrowser;
class QLabel;
class QLineEdit;

class camera;
class cameraView;

class InfoPanel : public QDockWidget
{
	Q_OBJECT

public:
	InfoPanel(QWidget *parent = 0);
	~InfoPanel();

	QTextBrowser *getBrowser() {return m_textBrowser;};
	void updateCamera(camera *cam);
	void updateCameraView(cameraView *view);
private:
	void setupme();
private:
	QWidget *dockWidgetContents;	// 空间容器
	QTextBrowser *m_textBrowser;	// 文本信息
	/* sm = K[R | t]M
	 * M -> m: 先旋转R,再平移t;
	 */
	QLabel *m_label_K;				// 内参矩阵
	QLabel *m_label_k;				// 畸变参数[k1,k2,p1,p2,k3]
	QLabel *m_label_om;				// 3x1旋转向量形式:方向为旋转轴,模为旋转大小
	QLabel *m_label_R;				// 3x3旋转矩阵
	QLabel *m_label_t;				// 3x1平移向量

	QTextBrowser *m_edit_K;			// 内参矩阵
	QLineEdit *m_edit_k;			// 畸变参数[k1,k2,p1,p2,k3]
	QLineEdit *m_edit_om;			// 3x1旋转向量形式:方向为旋转轴,模为旋转大小
	QTextBrowser *m_edit_R;			// 3x3旋转矩阵
	QLineEdit *m_edit_t;			// 3x1平移向量
};

#endif // !INFO_PANEL_H
/* infopanel.h
 * ��������궨�����ֵ��ʾ.
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
	QWidget *dockWidgetContents;	// �ռ�����
	QTextBrowser *m_textBrowser;	// �ı���Ϣ
	/* sm = K[R | t]M
	 * M -> m: ����תR,��ƽ��t;
	 */
	QLabel *m_label_K;				// �ڲξ���
	QLabel *m_label_k;				// �������[k1,k2,p1,p2,k3]
	QLabel *m_label_om;				// 3x1��ת������ʽ:����Ϊ��ת��,ģΪ��ת��С
	QLabel *m_label_R;				// 3x3��ת����
	QLabel *m_label_t;				// 3x1ƽ������

	QTextBrowser *m_edit_K;			// �ڲξ���
	QLineEdit *m_edit_k;			// �������[k1,k2,p1,p2,k3]
	QLineEdit *m_edit_om;			// 3x1��ת������ʽ:����Ϊ��ת��,ģΪ��ת��С
	QTextBrowser *m_edit_R;			// 3x3��ת����
	QLineEdit *m_edit_t;			// 3x1ƽ������
};

#endif // !INFO_PANEL_H
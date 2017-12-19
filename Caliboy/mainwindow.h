#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "Calib/camera.h"

class qimgviewer;
class qzoomview;
class QLabel;
class QPushButton;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;

class IOSettingDialog;
class CalibSettingDialog;
class InfoPanel;
class CalibResultViewer;

class QThreadCalib;

enum OpState{
	Op_Observing,
	Op_Import,
	Op_Export,
	Op_Picking,
	Op_Calibrating
};

class mainwindow : public QMainWindow
{
	Q_OBJECT

public:
	mainwindow(QWidget *parent = 0);
	~mainwindow();
private:
	void createActions();
	void createMenus();
	void createStatusBar();
	void createPanels();
	void createThreads();
	void initializeData();

	void dispVerInfo();

	void readSettings();
	void writeSettings();
private:
	QMenu *m_menuIO;
	QAction *m_actLoad_Files;
	QAction *m_actLoad_Folder;
	QAction *m_actLoad_ListTxt;
	QAction *m_actAdd_Files;
	QAction *m_actAdd_Folder;
	QAction *m_actAdd_ListTxt;

	QAction *m_actImport;
	QAction *m_actExport;

	QMenu *m_menuView;
	QAction *m_actView_calibReseult;
	QAction *m_actView_checkErr;

	QMenu *m_menuSetting;
	QAction *m_actSetting_IO;
	QAction *m_actSetting_Calib;

	QMenu *m_menuHelp;
	QAction *m_actAbout;
	QAction *m_actHowto;

	QLabel *m_labelStatus;
	// -------------------------
	InfoPanel *m_infoPanel;

	qimgviewer *m_view;
	qzoomview *m_zoomView;
	QPushButton *m_butPreviousOne;
	QPushButton *m_butNextOne;
	QLineEdit *m_infoIndex;
	QLineEdit *m_infoCount;
	QLineEdit *m_infoName;

	// -------------------------
	QPushButton *m_butLoad;
	QPushButton *m_butAdd;

	QPushButton *m_butFindCorners;
	QPushButton *m_butCliab;

	QPushButton *m_butImport;
	QPushButton *m_butExport;

	QPushButton *m_butClear;

	QSpinBox *m_spin_cornerSize;
	QSpinBox *m_spin_barSize;
	QDoubleSpinBox *m_spin_distK1;

	QPushButton *m_butOk;
	QPushButton *m_butNo;

	// -------------------------
	IOSettingDialog *m_dlg_setting_IO;
	CalibSettingDialog *m_dlg_setting_calib;
	CalibResultViewer *m_resultViewer;
private:
	void updateBut();									/* ���°�ť״̬ */
	void updateDisp();									/* ����ͼ����ʾ */

	void removeImages(const QStringList &fileList);		/* �Ƴ�ͼ�� */
	void loadImages(const QStringList &fileList);		/* ���ͼ�� */
	void clearScene();									/* ������� */

	void importResults(const QString cameraFile, const QString calibFile,
					   const bool bGridPts, const bool bRt);										/* ����궨��� */
	void exportResults(const QString cameraFile, const QString calibFile,
					   const bool bGridPts, const bool bRt);
private slots:
	void slotSettingIO();								/* ���뵼������ */
	void slotSettingCalib();							/* �궨���� */
	void slotAbout();									/* ���� */
	void slotHowto();									/* ����˵�� */

	void slotPreviousView();							/* ��һ��ͼ */
	void slotNextView();								/* ��һ��ͼ */
	void slotSetNView();								/* ������ͼ */

	void slotUIFinished(bool ok);						/* UI����״̬�л� */
	void slotAccept();									/* ���ܽ�� */
	void slotReject();									/* ȡ����ܾ���� */

	void slotLoadFiles();								/* �����ļ� */
	void slotLoadFolder();								/* �����ļ��� */
	void slotLoadListTxt();								/* ��Txt�е��� */
	void slotImport();									/* ���� */
	void slotExport();									/* ���� */
	void slotClearScene();								/* ������� */

	void slotViewCalibResult();							/* ��ʾ�궨��� */
	void slotErrorCalibResult();						/* �궨ͳ����� */

	void slotFindCorner();								/* ʰȡ�ǵ� */
	void slotSetCornerWinSize(int s);					/* ���������ؽǵ��ⴰ�ڴ�С */
	void slotSetBarWinSize(int s);						/* �����������ڿ�� */
	void slotUndistortionCorner(double c);				/* �������� */
	void slotClib();									/* ����궨 */

	void slotProgressBegin(OpState state);				/* ������ʼ */
	void slotProgressEnd();								/* �������� */
private:
	cameraScene m_calibScene;							/* ���� */
	int m_dispIndex;									/* ��ͼ���� */
	OpState m_state;									/* ����״̬ */

	QThreadCalib *m_calibThread;						/* �궨�����߳� */
	bool m_estPrincipalPoint;							/* �궨����-�̶�����λ��[ͼ������] */
	bool m_estFocalRatio;								/* �궨����-�̶�fx/fy[1] */
	unsigned int m_estDistMode;							/* �궨����-�����������
														 * [0]: ��
														 * [1]: k1
														 * [2]: k1,k2
														 * [3]: k1,k2,k3
														 * [4]: k1,k2,p1,p2
														 * [5]: k1,k2,p2,p2,k3
														 */
	QString m_IO_camerafile;							/* Ĭ������ļ� */
	QString m_IO_calibfile;								/* Ĭ�ϱ궨��� */
	bool m_IO_gridpts;									/* ���뵼��ѡ��:�������� */
	bool m_IO_Rt;										/* ���뵼��ѡ��:���λ�� */
};

#endif // MAINWINDOW_H

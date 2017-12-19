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
	void updateBut();									/* 更新按钮状态 */
	void updateDisp();									/* 更新图像显示 */

	void removeImages(const QStringList &fileList);		/* 移除图像 */
	void loadImages(const QStringList &fileList);		/* 添加图像 */
	void clearScene();									/* 清除场景 */

	void importResults(const QString cameraFile, const QString calibFile,
					   const bool bGridPts, const bool bRt);										/* 导入标定结果 */
	void exportResults(const QString cameraFile, const QString calibFile,
					   const bool bGridPts, const bool bRt);
private slots:
	void slotSettingIO();								/* 导入导出设置 */
	void slotSettingCalib();							/* 标定设置 */
	void slotAbout();									/* 关于 */
	void slotHowto();									/* 操作说明 */

	void slotPreviousView();							/* 上一视图 */
	void slotNextView();								/* 下一视图 */
	void slotSetNView();								/* 设置视图 */

	void slotUIFinished(bool ok);						/* UI交互状态切换 */
	void slotAccept();									/* 接受结果 */
	void slotReject();									/* 取消或拒绝结果 */

	void slotLoadFiles();								/* 导入文件 */
	void slotLoadFolder();								/* 导入文件夹 */
	void slotLoadListTxt();								/* 从Txt中导入 */
	void slotImport();									/* 导入 */
	void slotExport();									/* 导出 */
	void slotClearScene();								/* 清除场景 */

	void slotViewCalibResult();							/* 显示标定结果 */
	void slotErrorCalibResult();						/* 标定统计误差 */

	void slotFindCorner();								/* 拾取角点 */
	void slotSetCornerWinSize(int s);					/* 调整亚像素角点检测窗口大小 */
	void slotSetBarWinSize(int s);						/* 调整条带窗口宽度 */
	void slotUndistortionCorner(double c);				/* 调整畸变 */
	void slotClib();									/* 相机标定 */

	void slotProgressBegin(OpState state);				/* 操作开始 */
	void slotProgressEnd();								/* 操作结束 */
private:
	cameraScene m_calibScene;							/* 场景 */
	int m_dispIndex;									/* 视图索引 */
	OpState m_state;									/* 操作状态 */

	QThreadCalib *m_calibThread;						/* 标定运行线程 */
	bool m_estPrincipalPoint;							/* 标定设置-固定主点位置[图像中心] */
	bool m_estFocalRatio;								/* 标定设置-固定fx/fy[1] */
	unsigned int m_estDistMode;							/* 标定设置-畸变参数设置
														 * [0]: 无
														 * [1]: k1
														 * [2]: k1,k2
														 * [3]: k1,k2,k3
														 * [4]: k1,k2,p1,p2
														 * [5]: k1,k2,p2,p2,k3
														 */
	QString m_IO_camerafile;							/* 默认相机文件 */
	QString m_IO_calibfile;								/* 默认标定结果 */
	bool m_IO_gridpts;									/* 导入导出选项:网格坐标 */
	bool m_IO_Rt;										/* 导入导出选项:相机位置 */
};

#endif // MAINWINDOW_H

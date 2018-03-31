#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera.h"

class QImgViewer;
class QImgZoomViewer;
class PanelRuntime;
class PanelResult;

class DlgSettingIO;
class DlgSettingCalib;

class QLabel;
class QPushButton;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;

class QThreadCalib;

class mainwindow : public QMainWindow
{
Q_OBJECT
private:
    enum OpState {
        Op_Observing,
        Op_Import,
        Op_Export,
        Op_Picking,
        Op_Calibrating
    };
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
private:
    PanelRuntime *m_panelRuntime;
    PanelResult *m_panelResult;
    QImgViewer *m_view;
    QImgZoomViewer *m_zoomView;

    QPushButton *m_butPreviousOne;
    QPushButton *m_butNextOne;
    QLineEdit *m_infoIndex;
    QLineEdit *m_infoCount;
    QLineEdit *m_infoName;

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

    DlgSettingIO *m_dlg_setting_IO;
    DlgSettingCalib *m_dlg_setting_calib;
    
private:
    void updateBut();
    void updateDisp();

    void removeImages(const QStringList &fileList);
    void loadImages(const QStringList &fileList);
    void clearScene();

    void importResults(
        const QString cameraFile, const QString calibFile,
        const bool bGridPts, const bool bRt);
    void exportResults(
        const QString cameraFile, const QString calibFile,
        const bool bGridPts, const bool bRt);
private slots:
    void slotSettingIO();
    void slotSettingCalib();
    void slotAbout();
    void slotHowto();

    void slotPreviousView();
    void slotNextView();
    void slotSetNView();

    void slotUIFinished(bool ok);
    void slotAccept();
    void slotReject();

    void slotLoadFiles();
    void slotLoadFolder();
    void slotLoadListTxt();
    void slotImport();
    void slotExport();
    void slotClearScene();

    void slotViewCalibResult();
    void slotErrorCalibResult();

    void slotFindCorner();
    void slotSetCornerWinSize(int s);
    void slotSetBarWinSize(int s);
    void slotUndistortionCorner(double c);
    void slotClib();

    void slotProgressBegin(OpState state);
    void slotProgressEnd(); 
private:
    cameraScene m_calibScene;        /* Scene data. */
    OpState m_state;                 /* Operation state. */
    int m_dispIndex;                 /* Index of view in the scene. */


    QThreadCalib *m_calibThread;     /* Runtime thread. */
    bool m_estPrincipalPoint;        /* Whether to estimate the principal point. */
    bool m_estFocalRatio;            /* Whether to estimate the focal ratio.
                                        If not, the focal ratio will be set 1. */
    unsigned int m_estDistMode;      /* Distortion model:
                                      * [0]: None
                                      * [1]: Only k1
                                      * [2]: Only k1,k2
                                      * [3]: Only k1,k2,k3
                                      * [4]: Only k1,k2,p1,p2
                                      * [5]: All k1,k2,p2,p2,k3
                                      */
    QString m_IO_camerafile;         /* Default path of camera file. */
    QString m_IO_calibfile;          /* Default path of calibration info file. */
    bool m_IO_gridpts;               /* Whether to export/import the coordinates of grid points. */
    bool m_IO_Rt;                    /* Whether to export/import the camera parameters. */
};

#endif// !MAINWINDOW_H

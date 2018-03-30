#include "mainwindow.h"

#include <QSettings>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLayout>
#include <QWebEngineView>
#include <QMessageBox>
#include <QFileDialog>

#include "ui/qimgviewer.h"
#include "ui/qimgzoomviewer.h"
#include "ui/panelresult.h"
#include "ui/panelruntime.h"
#include "ui/dlgpickview.h"
#include "ui/dlgcheckerror.h"
#include "ui/dlgsettingIO.h"
#include "ui/dlgsettingcalib.h"
#include "ui/qmsg.h"

#include "threadCalib.h"

#include "parseTxt.h"

#define SZ_AUTO_CORNER  5

#define QSS_STYLE       "./Resources/style.qss"
#define PIC_ICON        "./Resources/icon.png"
#define PIC_PRE         "./Resources/pic/previous.png"
#define PIC_NEXT        "./Resources/pic/next.png"
#define PIC_OPEN_FILES  "./Resources/pic/openFiles.png"
#define PIC_OPEN_FOLDER "./Resources/pic/openFolder.png"
#define PIC_OPEN_TXT    "./Resources/pic/openTxt.png"
#define PIC_ADD_FILES   "./Resources/pic/openFiles+.png"
#define PIC_ADD_FOLDER  "./Resources/pic/openFolder+.png"
#define PIC_ADD_TXT     "./Resources/pic/openTxt+.png"
#define PIC_SAVE        "./Resources/pic/save.png"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(PIC_ICON));
    QFile file(QSS_STYLE);
    if (file.open(QFile::ReadOnly))
        setStyleSheet(file.readAll());

    readSettings();

    createActions();
    createMenus();
    createStatusBar();
    createPanels();
    createThreads();
    initializeData();
    updateBut();
    dispVerInfo();
}
mainwindow::~mainwindow()
{
    writeSettings();
}

void mainwindow::createActions()
{
    m_actLoad_Files = new QAction(QIcon(PIC_OPEN_FILES),"Load Image...",this);
    m_actLoad_Folder = new QAction(QIcon(PIC_OPEN_FOLDER),"Load from Folder...",this);
    m_actLoad_ListTxt = new QAction(QIcon(PIC_OPEN_TXT),"Load from List...",this);
    m_actAdd_Files = new QAction(QIcon(PIC_ADD_FILES),"Add Image...",this);
    m_actAdd_Folder = new QAction(QIcon(PIC_ADD_FOLDER),"Add form Folder...",this);
    m_actAdd_ListTxt = new QAction(QIcon(PIC_ADD_TXT),"Add from List...",this);

    m_actImport = new QAction("Import Result...",this);
    m_actExport = new QAction(QIcon(PIC_SAVE),"Export Result...",this);

    m_actView_calibReseult = new QAction("Render Result...",this);
    m_actView_checkErr = new QAction(("Calibration Error..."),this);

    m_actSetting_IO = new QAction(("Import\\Export Setting..."),this);
    m_actSetting_Calib = new QAction(("Calibration Setting..."),this);

    m_actAbout = new QAction(("About Caliboy..."),this);
    m_actHowto = new QAction(("Instruction..."),this);


    connect(m_actLoad_Files,SIGNAL(triggered()), this,SLOT(slotLoadFiles()));
    connect(m_actLoad_Folder,SIGNAL(triggered()), this,SLOT(slotLoadFolder()));
    connect(m_actLoad_ListTxt,SIGNAL(triggered()), this,SLOT(slotLoadListTxt()));
    connect(m_actAdd_Files,SIGNAL(triggered()), this,SLOT(slotLoadFiles()));
    connect(m_actAdd_Folder,SIGNAL(triggered()), this,SLOT(slotLoadFolder()));
    connect(m_actAdd_ListTxt,SIGNAL(triggered()), this,SLOT(slotLoadListTxt()));

    connect(m_actImport,SIGNAL(triggered()), this,SLOT(slotImport()));
    connect(m_actExport,SIGNAL(triggered()), this,SLOT(slotExport()));

    connect(m_actView_calibReseult,SIGNAL(triggered()), this,SLOT(slotViewCalibResult()));
    connect(m_actView_checkErr,SIGNAL(triggered()), this,SLOT(slotErrorCalibResult()));

    connect(m_actSetting_IO,SIGNAL(triggered()), this,SLOT(slotSettingIO()));
    connect(m_actSetting_Calib,SIGNAL(triggered()), this,SLOT(slotSettingCalib()));
    connect(m_actAbout,SIGNAL(triggered()), this,SLOT(slotAbout()));
    connect(m_actHowto,SIGNAL(triggered()), this,SLOT(slotHowto()));
}
void mainwindow::createMenus()
{
    m_menuIO = menuBar()->addMenu("File");
    m_menuIO->addAction(m_actLoad_Files);
    m_menuIO->addAction(m_actLoad_Folder);
    m_menuIO->addAction(m_actLoad_ListTxt);
    m_menuIO->addSeparator();
    m_menuIO->addAction(m_actAdd_Files);
    m_menuIO->addAction(m_actAdd_Folder);
    m_menuIO->addAction(m_actAdd_ListTxt);
    m_menuIO->addSeparator();
    m_menuIO->addAction(m_actImport);
    m_menuIO->addAction(m_actExport);

    m_menuView = menuBar()->addMenu("View");
    m_menuView->addAction(m_actView_calibReseult);
    m_menuView->addAction(m_actView_checkErr);

    m_menuSetting = menuBar()->addMenu("Option");
    m_menuSetting->addAction(m_actSetting_Calib);
    m_menuSetting->addAction(m_actSetting_IO);

    m_menuHelp = menuBar()->addMenu("Help");
    m_menuHelp->addAction(m_actHowto);
    m_menuHelp->addSeparator();
    m_menuHelp->addAction(m_actAbout);
}
void mainwindow::createStatusBar()
{
    m_labelStatus = new QLabel("Ready");
    m_labelStatus->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_labelStatus->setMinimumSize(m_labelStatus->sizeHint());
    statusBar()->addWidget(m_labelStatus);
}
void mainwindow::createPanels()
{
    m_dlg_setting_IO = new DlgSettingIO(this);
    m_dlg_setting_calib = new DlgSettingCalib(this);

    m_panelResult = new PanelResult(&m_calibScene,this);
    addDockWidget(Qt::RightDockWidgetArea,m_panelResult);

    QWidget *_centralWidget = new QWidget(this); 

    m_panelRuntime = new PanelRuntime(this);
    setOutBrowser(m_panelRuntime->getBrowser());
    addDockWidget(Qt::LeftDockWidgetArea,m_panelRuntime);

    m_view = new QImgViewer(this);
    m_zoomView = new QImgZoomViewer(this);
    m_zoomView->setZoomSourceView(m_view);

    QLabel *pLabel = new QLabel(this);
    pLabel->setFixedWidth(12);
    pLabel->setAutoFillBackground(false);
    pLabel->setStyleSheet("background-color: rgb(120, 120, 120);");

    m_butPreviousOne = new QPushButton(this);
    m_butNextOne = new QPushButton(this);
    m_infoIndex = new QLineEdit(this);
    m_infoCount = new QLineEdit(this);
    m_infoName = new QLineEdit(this);
    m_butPreviousOne->setFixedSize(QSize(15, 20));
    m_butPreviousOne->setAutoFillBackground(true);
    m_butPreviousOne->setIcon(QIcon(PIC_PRE));
    m_butPreviousOne->setStyleSheet(
        "background-color:rgb(210,210,210);}"
        "QPushButton::disabled{"
        "background-color:rgb(230,230,230);}"
        "QPushButton::hover{"
        "background-color:rgb(194,213,242);}"
        "QPushButton::pressed{"
        "background-color:rgb(163,189,227);}");
    m_butNextOne->setFixedSize(QSize(15, 20));
    m_butNextOne->setAutoFillBackground(true);
    m_butNextOne->setIcon(QIcon(PIC_NEXT));
    m_butNextOne->setStyleSheet(
        "background-color:rgb(210,210,210);}"
        "QPushButton::disabled{"
        "background-color:rgb(230,230,230);}"
        "QPushButton::hover{"
        "background-color:rgb(194,213,242);}"
        "QPushButton::pressed{"
        "background-color:rgb(163,189,227);}");
    m_infoIndex->setReadOnly(true);
    m_infoIndex->setAlignment(Qt::AlignCenter);
    m_infoIndex->setFixedSize(QSize(40, 20));
    m_infoIndex->setText("0");
    m_infoIndex->setStyleSheet("background-color: rgb(255,255,255);");
    m_infoCount->setReadOnly(true);
    m_infoCount->setAlignment(Qt::AlignCenter);
    m_infoCount->setFixedSize(QSize(40, 20));
    m_infoCount->setText("/0");
    m_infoCount->setStyleSheet("background-color: rgb(199, 199, 199);");
    m_infoName->setReadOnly(true);
    m_infoName->setFixedHeight(20);
    m_infoName->setText("");

    m_butLoad = new QPushButton("Load Image",this);
    m_butAdd = new QPushButton("Add Image",this);
    m_butImport = new QPushButton("Import",this);
    m_butExport = new QPushButton("Export",this);
    m_butFindCorners = new QPushButton("Detect Corners",this);
    m_butCliab = new QPushButton("Calibrate Me",this);
    m_butClear = new QPushButton("Clean View",this);

    m_spin_cornerSize = new QSpinBox(this);
    m_spin_cornerSize->setMinimum(SZ_AUTO_CORNER/2);
    m_spin_cornerSize->setMaximum(SZ_AUTO_CORNER*2);
    m_spin_cornerSize->setSingleStep(1);
    m_spin_cornerSize->setValue(SZ_AUTO_CORNER);
    m_spin_cornerSize->hide();

    m_spin_barSize = new QSpinBox(this);
    m_spin_barSize->setMinimum(SZ_AUTO_CORNER);
    m_spin_barSize->setMaximum(SZ_AUTO_CORNER*2);
    m_spin_barSize->setSingleStep(1);
    m_spin_barSize->setValue(SZ_AUTO_CORNER*1.5);
    m_spin_barSize->hide();

    m_spin_distK1 = new QDoubleSpinBox(this);
    m_spin_distK1->setMinimum(-0.99);
    m_spin_distK1->setMaximum(0.99);
    m_spin_distK1->setDecimals(2);
    m_spin_distK1->setSingleStep(0.05);
    m_spin_distK1->setValue(0.0);
    m_spin_distK1->hide();

    m_butOk = new QPushButton("Confirm",this);
    m_butNo = new QPushButton("Cancel",this);

    QVBoxLayout *vLayout1 = new QVBoxLayout();
    QVBoxLayout *vLayout2 = new QVBoxLayout();
    QHBoxLayout *hLayout1 = new QHBoxLayout();
    QHBoxLayout *hLayout2 = new QHBoxLayout();
    QHBoxLayout *hLayout3 = new QHBoxLayout();

    hLayout1->setSpacing(1);
    hLayout1->addWidget(pLabel,0);
    hLayout1->addWidget(m_infoIndex,0);
    hLayout1->addWidget(m_infoCount,0);
    hLayout1->addWidget(m_butPreviousOne,0);
    hLayout1->addWidget(m_infoName,1);
    hLayout1->addWidget(m_butNextOne,0);

    vLayout1->addWidget(m_view);
    vLayout1->addLayout(hLayout1);

    hLayout2->addWidget(m_butOk);
    hLayout2->addWidget(m_butNo);
    QFrame* line1 = new QFrame(this);
        line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);
    QFrame* line2 = new QFrame(this);
        line2->setFrameShape(QFrame::HLine);
        line2->setFrameShadow(QFrame::Sunken);

    vLayout2->addWidget(m_zoomView,0);
    vLayout2->addSpacing(5);
    vLayout2->addWidget(line1,0);
    vLayout2->addSpacing(5);
    vLayout2->addWidget(m_butLoad);
    vLayout2->addWidget(m_butAdd);
    vLayout2->addSpacing(10);
    vLayout2->addWidget(m_butFindCorners);
    vLayout2->addWidget(m_butCliab);
    vLayout2->addSpacing(10);
    vLayout2->addWidget(m_butImport);
    vLayout2->addWidget(m_butExport);
    vLayout2->addSpacing(10);
    vLayout2->addWidget(m_butClear);
    vLayout2->addStretch();
    vLayout2->addWidget(m_spin_distK1);
    vLayout2->addWidget(line2);
    vLayout2->addWidget(m_spin_cornerSize);
    vLayout2->addWidget(m_spin_barSize);
    vLayout2->addLayout(hLayout2);

    hLayout3->addLayout(vLayout1,1);
    hLayout3->addLayout(vLayout2,0);

    hLayout3->setSpacing(4);
    hLayout3->setContentsMargins(2,0,2,10);

    _centralWidget->setLayout(hLayout3); 
    setCentralWidget(_centralWidget);

    connect(m_butPreviousOne,SIGNAL(clicked()),this,SLOT(slotPreviousView()));
    connect(m_butNextOne,SIGNAL(clicked()),this,SLOT(slotNextView()));
    connect(m_infoIndex,SIGNAL(editingFinished()),this,SLOT(slotSetNView()));
    connect(m_butOk,SIGNAL(clicked()),this,SLOT(slotAccept()));
    connect(m_butNo,SIGNAL(clicked()),this,SLOT(slotReject()));

    connect(m_butLoad,SIGNAL(clicked()),m_actLoad_Files,SIGNAL(triggered()));
    connect(m_butAdd,SIGNAL(clicked()),m_actAdd_Files,SIGNAL(triggered()));
    connect(m_butClear,SIGNAL(clicked()),this,SLOT(slotClearScene()));

    connect(m_butFindCorners,SIGNAL(clicked()),this,SLOT(slotFindCorner()));
    connect(m_butCliab,SIGNAL(clicked()),this,SLOT(slotClib()));

    connect(m_butImport,SIGNAL(clicked()),this,SLOT(slotImport()));
    connect(m_butExport,SIGNAL(clicked()),this,SLOT(slotExport()));

    connect(m_spin_distK1,SIGNAL(valueChanged(double)),this,SLOT(slotUndistortionCorner(double)));
    connect(m_spin_cornerSize,SIGNAL(valueChanged(int)),this,SLOT(slotSetCornerWinSize(int)));
    connect(m_spin_barSize,SIGNAL(valueChanged(int)),this,SLOT(slotSetBarWinSize(int)));


    connect(m_view,SIGNAL(signalFinished(bool)),this,SLOT(slotUIFinished(bool)));
}
void mainwindow::createThreads()
{
    m_calibThread = new QThreadCalib();
    m_calibThread->setCalibParams(m_estPrincipalPoint,m_estFocalRatio,m_estDistMode);
    connect(m_calibThread, SIGNAL(signalThreadEnd()), this, SLOT(slotProgressEnd()));
}
void mainwindow::initializeData()
{
    m_dispIndex = -1;
    m_state = Op_Observing;
}
void mainwindow::dispVerInfo()
{
    msgOut(
        "    Caliboy #version 2.0.0.201803.r  \n"
        ">>-----------------------------------<<"
    );
}
void mainwindow::readSettings()
{
    QSettings settings("setting.ini", QSettings::IniFormat);

    m_estFocalRatio= settings.value("Setting_Calib/bEstFocalRatio").toBool();
    m_estPrincipalPoint = settings.value("Setting_Calib/bEstPrincipalPoint").toBool();
    m_estDistMode= settings.value("Setting_Calib/iEstDistModo").toInt();

    m_IO_camerafile= settings.value("Setting_IO/camera_file").toString();
    m_IO_calibfile= settings.value("Setting_IO/calib_file").toString();
    m_IO_gridpts= settings.value("Setting_IO/bGridPts").toBool();
    m_IO_Rt= settings.value("Setting_IO/bRtPos").toBool();
}
void mainwindow::writeSettings()
{
    QSettings settings("setting.ini", QSettings::IniFormat);

    settings.setValue("Setting_Calib/bEstPrincipalPoint", m_estPrincipalPoint);
    settings.setValue("Setting_Calib/bEstFocalRatio", m_estFocalRatio);
    settings.setValue("Setting_Calib/iEstDistModo", m_estDistMode);


    settings.setValue("Setting_IO/camera_file", m_IO_camerafile);
    settings.setValue("Setting_IO/calib_file", m_IO_calibfile);
    settings.setValue("Setting_IO/bGridPts", m_IO_gridpts);
    settings.setValue("Setting_IO/bRtPos", m_IO_Rt);
}

void mainwindow::updateBut()
{
    bool bHasView = (m_calibScene.viewCount() == 0)? false : true;
    bool bIsObserving = (m_state == Op_Observing) ?  true:false;
    bool bIsPicking = (m_state == Op_Picking) ? true : false;

    m_actLoad_Files->setEnabled(bIsObserving);
    m_actLoad_Folder->setEnabled(bIsObserving);
    m_actLoad_ListTxt->setEnabled(bIsObserving);
    m_actAdd_Files->setEnabled(bIsObserving && bHasView);
    m_actAdd_Folder->setEnabled(bIsObserving && bHasView);
    m_actAdd_ListTxt->setEnabled(bIsObserving && bHasView);
    m_actImport->setEnabled(bIsObserving && bHasView);
    m_actExport->setEnabled(bIsObserving && bHasView);
    m_actView_checkErr->setEnabled(bIsObserving && bHasView);

    m_butPreviousOne->setEnabled(bIsObserving && bHasView);
    m_butNextOne->setEnabled(bIsObserving && bHasView);
    m_infoIndex->setReadOnly(!(bIsObserving && bHasView));

    m_butLoad->setEnabled(bIsObserving);
    m_butAdd->setEnabled(bIsObserving && bHasView);
    m_butFindCorners->setEnabled(bIsObserving && bHasView);
    m_butCliab->setEnabled(bIsObserving && bHasView);
    m_butImport->setEnabled(bIsObserving && bHasView);
    m_butExport->setEnabled(bIsObserving && bHasView);
    m_butClear->setEnabled(bIsObserving && bHasView);

    m_butOk->setVisible(bIsPicking);
    m_butOk->setEnabled(!bIsPicking);
    m_butNo->setVisible(bIsPicking);
    m_spin_cornerSize->setVisible(bIsPicking);
    m_spin_barSize->setVisible(bIsPicking);  
}
void mainwindow::updateDisp()
{
    if (m_calibScene.viewCount() == 0){
        m_view->cleanView();
        m_infoIndex->setText("0");
        m_infoCount->setText("/0");
        m_infoName->setText("");

        m_panelRuntime->updateCameraView(0);
        m_panelRuntime->updateCamera(0);

        m_dispIndex = -1;
    }
    if (m_calibScene.viewCount()>0){
        if (m_dispIndex>=m_calibScene.viewCount() || 
            m_dispIndex<0)
            m_dispIndex = 0;

        m_view->setView(m_calibScene.m_views.at(m_dispIndex));
        m_infoIndex->setText(QString("%1").arg(m_dispIndex + 1));
        m_infoCount->setText(QString("/%1").arg(m_calibScene.viewCount()));
        m_infoName->setText(m_calibScene.m_views.at(m_dispIndex)->getFileName());

        m_panelRuntime->updateCameraView(m_calibScene.m_views.at(m_dispIndex));
        m_panelRuntime->updateCamera(&m_calibScene.m_camera);
    }
    m_panelResult->setView(m_dispIndex);
    m_panelResult->updateScene(); 
}
void mainwindow::removeImages(const QStringList &fileList)
{
    if (! fileList.isEmpty()) {
        msgOut("\n>> Remove Image(s)...");
        int imgCnt = fileList.size();
        for (int i=0; i<imgCnt; i++) {

            QString fileName = fileList.at(i);
            int n = m_calibScene.contains(fileName);
            if (n == -1) {
                _msgOut(QString("%1/%2:%3 has not been loaded!").arg(i+1).arg(imgCnt).arg(fileName));
            } else {
                m_calibScene.removeView(fileName);
                if (m_dispIndex>n)
                    m_dispIndex--;
                    _msgOut(QString("%1/%2:%3 is removed.").arg(i + 1).arg(imgCnt).arg(fileName));
            }
        }
    }
}
void mainwindow::loadImages(const QStringList &fileList)
{
    if (! fileList.isEmpty()) {
        msgOut("\n>> Load Image(s)...");
        int imgCnt = fileList.size();
        for (int i=0; i<imgCnt; i++) {
            QString fileName = fileList.at(i);

            if (m_calibScene.contains(fileName) != -1) {
                _msgOut(QString("%1/%2:%3 has been loaded!").arg(i+1).arg(imgCnt).arg(fileName));
            } else {
                cameraView *newView = new cameraView(fileName);
                if (newView->getQImage() != 0) {
                    m_calibScene.addView(newView);
                    _msgOut(QString("%1/%2:%3 is loaded.").arg(i + 1).arg(imgCnt).arg(fileName));
                }
                else {
                    _msgOut(QString("%1/%2:Failed to load %3!").arg(i+1).arg(imgCnt).arg(fileName));
                    delete newView;
                }
            }
        }
    }
    if (m_calibScene.viewCount() > 0) {
        m_infoIndex->setReadOnly(false);
        m_infoIndex->setValidator(new QIntValidator(1,m_calibScene.viewCount()));
    }
}
void mainwindow::clearScene()
{
    msgOut("\n>> Clean Scene...");
    msgOut("********************************");
    msgOut("================================");
    m_infoIndex->setReadOnly(true);
    m_calibScene.clean();
}

void importView(
    cameraScene *scene, FILE *fin,
    const bool bGridPts, const bool bRt)
{
    if (scene == 0 ||
    fin == NULL ||
    (!bGridPts && !bRt))
    return;

    char fileName[1024];
    while (fgets(fileName,1024,fin)!=NULL) {
        for (int i = 0; i<1024; ++i)
            if (fileName[i] == '\n')
                fileName[i] = '\0';

        int nx, ny;
        fscanf_s(fin,"%d %d\n",&nx, &ny);

        int viewIdx = scene->contains(QString(fileName));
        if (viewIdx == -1) {
            char buff[1024];
            for (int i=0; i<ny+2; ++i)
                fgets(buff,1024,fin);
        }
        else {
            _msgOut(QString("Import view info for iamge %1").arg(fileName));
            if (nx>0 && ny>0 && bGridPts) {
                std::vector<cv::Point2f> pts;
                for (int i=0; i<nx*ny; ++i) {
                    double x, y;
                    fscanf_s(fin,"%lf %lf",&x,&y);
                    pts.push_back(cv::Point2f(x,y));
                }
                scene->m_views.at(viewIdx)->setGridCorners(nx,ny,pts);
                pts.clear();
            }
            if (bRt) {
                double r[3], t[3];
                fscanf_s(fin,
                        "%lf %lf %lf\n"
                        "%lf %lf %lf\n",
                        &r[0],&r[1],&r[2],
                        &t[0],&t[1],&t[2]);
                cv::Mat rvec(3,1,CV_64F,r);
                cv::Mat R(3,3,CV_64F);
                cv::Mat tvec(3,1,CV_64F,t);
                scene->m_views.at(viewIdx)->r = rvec.clone();
                scene->m_views.at(viewIdx)->t = tvec.clone();
                cv::Rodrigues(rvec,R);
                scene->m_views.at(viewIdx)->R = R;
            }
        }
    }
}
void exportView(
    cameraView *view, FILE *fout,
    const bool bGridPts, const bool bRt)
{
    if (fout == NULL)
        return;
    QString filename = QFileInfo(view->getFileName()).absoluteFilePath().toLocal8Bit();
    _msgOut(QString("Export view info for iamge %1").arg(filename));

    fprintf(fout,"%s\n", filename);

    if (bGridPts) {
        int nx, ny;
        std::vector<cv::Point2f> pts = view->getGridCorners(nx,ny);
        fprintf(fout,"%d %d\n",nx,ny);
        int n = 0;
        for (int i=0; i<ny; ++i) {
            for (int j=0; j<nx; j++) {
                fprintf_s(fout,"%f %f\t",pts.at(n).x,pts.at(n).y);
                n++;
            }
            fprintf(fout,"\n");
        }
    } else 
        fprintf(fout,"0 0\n");

    if (bRt)
        fprintf_s(fout,
            "%f %f %f\n"
            "%f %f %f\n",
            view->r.at<double>(0),view->r.at<double>(1),view->r.at<double>(2),
            view->t.at<double>(0),view->t.at<double>(1),view->t.at<double>(2));
    else
        fprintf_s(fout,
            "0.0 0.0 0.0\n"
            "0.0 0.0 0.0\n");
}
void mainwindow::importResults(
    const QString cameraFile, const QString calibFile,
    const bool bGridPts, const bool bRt)
{
    msgOut("\n>> Import Calibration Result...");
    if (!cameraFile.isEmpty()) {
        _msgOut(QString("Import camera info from file %1").arg(cameraFile));
        if (!QFileInfo(cameraFile).exists())
            _msgOut(QString("File %1 not exits!").arg(cameraFile));
        else {
            FILE *fin_c = 0;
            fopen_s(&fin_c,cameraFile.toLocal8Bit(), "r");
            if (fin_c == NULL)
                _msgOut("Failed to open!");
            else {
                fscanf_s(fin_c,
                    "%lf\t%lf\n"
                    "%lf\t%lf\n"
                    "%lf\t%lf\t%lf\t%lf\t%lf",
                    &m_calibScene.m_camera.cx,&m_calibScene.m_camera.cy,
                    &m_calibScene.m_camera.fx,&m_calibScene.m_camera.fy,
                    &m_calibScene.m_camera.k[0],&m_calibScene.m_camera.k[1],
                    &m_calibScene.m_camera.k[2],&m_calibScene.m_camera.k[3],
                    &m_calibScene.m_camera.k[4]);
                fclose(fin_c);
            }
        }
    }
    if (!calibFile.isEmpty() && (bGridPts || bGridPts)) {
        _msgOut(QString("Import view info from file %1").arg(calibFile));
        if (!QFileInfo(calibFile).exists())
            _msgOut(QString("File %1 not exits!").arg(calibFile));
        else {
            FILE *fin_calib = 0;
            fopen_s(&fin_calib,calibFile.toLocal8Bit(), "r");
            if (fin_calib == NULL)
                _msgOut("Failed to opne!");
            else {
                importView(&m_calibScene, fin_calib,bGridPts,bRt);
                fclose(fin_calib);
            }
        }
    }
}
void mainwindow::exportResults(
    const QString cameraFile, const QString calibFile,
    const bool bGridPts, const bool bRt)
{
    msgOut("\n>> Export Calibration Result...");
    if (!cameraFile.isEmpty()) {
        _msgOut(QString("Export camera info to file %1").arg(cameraFile));
        FILE *fout_c = 0;
        fopen_s(&fout_c,cameraFile.toLocal8Bit(), "w");
        if (fout_c == NULL)
            _msgOut("Failed to open!");
        else {
            fprintf(fout_c,
                "%f\t%f\n"
                "%f\t%f\n"
                "%f\t%f\t%f\t%f\t%f",
                m_calibScene.m_camera.cx,m_calibScene.m_camera.cy,
                m_calibScene.m_camera.fx,m_calibScene.m_camera.fy,
                m_calibScene.m_camera.k[0],m_calibScene.m_camera.k[1],
                m_calibScene.m_camera.k[2],m_calibScene.m_camera.k[3],
                m_calibScene.m_camera.k[4]);
            fclose(fout_c);
        }
    }
    if (!calibFile.isEmpty() && (bGridPts || bGridPts)) {
        _msgOut(QString("Expport view info to file %1").arg(calibFile));
        FILE *fout_calib = 0;
        fopen_s(&fout_calib,calibFile.toLocal8Bit(), "w");
        if (fout_calib == NULL)
            _msgOut("Failed to open!");
        else {
            for (int i=0; i<m_calibScene.viewCount(); ++i) {
                cameraView *view = m_calibScene.m_views.at(i);
                exportView(view,fout_calib,bGridPts,bRt);
            }
            fclose(fout_calib);
        }
    }
}

void mainwindow::slotPreviousView()
{
    slotProgressBegin(Op_Observing);
    m_dispIndex--;
    if (m_dispIndex<0)
        m_dispIndex = m_calibScene.viewCount()-1;
    slotProgressEnd();
}
void mainwindow::slotNextView()
{
    slotProgressBegin(Op_Observing);
    m_dispIndex++;
    if (m_dispIndex == m_calibScene.viewCount())
        m_dispIndex = 0;
    slotProgressEnd();
}
void mainwindow::slotSetNView()
{
    slotProgressBegin(Op_Observing);
    m_dispIndex = m_infoIndex->text().toInt()-1;
    slotProgressEnd();
}

void mainwindow::slotLoadFiles()
{
    slotProgressBegin(Op_Import);
    QStringList fileList = QFileDialog::getOpenFileNames( this,
            "Load Images...", "",
            "Image File (*.bmp;*.jpg;*.tif;*.tiff;*.pgm;*.png)");
    if (!fileList.isEmpty()) {
        if (qobject_cast<QAction*>(sender()) == m_actLoad_Files)
            clearScene();
        loadImages(fileList);
    }
    slotProgressEnd();
}
void mainwindow::slotLoadFolder()
{
    slotProgressBegin(Op_Import);
    QString folder = QFileDialog::getExistingDirectory(this, "Load from Folder...");
    if (!folder.isEmpty()) {
        QStringList fileList;
        QStringList filter;
        filter << "*.bmp" << "*.jpg" <<"*.tif" <<"*.tiff" <<"*.pgm" <<"*.png";
        QDir dir(folder);
        QFileInfoList fileInfoList = dir.entryInfoList(filter);
        for (int i=0; i<fileInfoList.size(); ++i)
            fileList << fileInfoList.at(i).absoluteFilePath(); 

        if (!fileList.isEmpty()) {
            if (qobject_cast<QAction*>(sender()) == m_actLoad_Folder)
                clearScene();
            loadImages(fileList);
        }
    }
    slotProgressEnd();
}
void mainwindow::slotLoadListTxt()
{
    slotProgressBegin(Op_Import);
    QString Txtfile = QFileDialog::getOpenFileName( this,
        "Load from List...", "",
        "Txt File (*.txt);;"
        "All File (*.*);;");    
    if (!Txtfile.isEmpty()) {

        QStringList addList;
        QStringList removeList;
        parseTxt(Txtfile,addList,removeList);

        if (qobject_cast<QAction*>(sender()) == m_actLoad_ListTxt)
            clearScene();
        else
            removeImages(removeList);
        loadImages(addList);
    }
    slotProgressEnd();
}
void mainwindow::slotImport()
{
    slotProgressBegin(Op_Import);
    m_dlg_setting_IO->setParams(m_IO_camerafile,m_IO_calibfile,m_IO_gridpts,m_IO_Rt);
    if (QDialog::Accepted == m_dlg_setting_IO->show_Import()) {
        m_dlg_setting_IO->getParams(m_IO_camerafile, m_IO_calibfile, m_IO_gridpts, m_IO_Rt);
        importResults(m_IO_camerafile, m_IO_calibfile, m_IO_gridpts, m_IO_Rt);
    }
    slotProgressEnd();
}
void mainwindow::slotExport()
{
    slotProgressBegin(Op_Export);
    m_dlg_setting_IO->setParams(m_IO_camerafile,m_IO_calibfile,m_IO_gridpts,m_IO_Rt);
    if (QDialog::Accepted == m_dlg_setting_IO->show_Export()) {
        m_dlg_setting_IO->getParams(m_IO_camerafile, m_IO_calibfile, m_IO_gridpts, m_IO_Rt);
        exportResults(m_IO_camerafile, m_IO_calibfile, m_IO_gridpts, m_IO_Rt);
    }
    slotProgressEnd();

}
void mainwindow::slotClearScene()
{
    slotProgressBegin(Op_Observing);
    clearScene();
    slotProgressEnd();
}

void mainwindow::slotViewCalibResult()
{
    m_panelResult->setVisible(!m_panelResult->isVisible());
}
void mainwindow::slotErrorCalibResult()
{
    if (m_calibScene.viewCount() == 0 ||
        m_calibScene.m_camera.cx < 10.0) {
        msgOut("\n>> Please calibrate this scene first!");
        return;
    }
    DlgCheckError *errDlg = new DlgCheckError(&m_calibScene,this);
    errDlg->exec();
    delete errDlg;
}

void mainwindow::slotUIFinished(bool ok)
{
    if (m_state == Op_Picking) {
        m_spin_distK1->setValue(0);
        m_spin_distK1->setVisible(ok);
    }
    m_butOk->setEnabled(ok);
}
void mainwindow::slotAccept()
{
    if (m_state == Op_Picking) {
        m_view->accept();
    }
    slotProgressEnd();
}
void mainwindow::slotReject()
{
    if (m_state == Op_Picking) {
        m_view->reject();
    }
    slotProgressEnd();
}
void mainwindow::slotProgressBegin(OpState state)
{
    m_state = state;
    switch (m_state)
    {
    case Op_Import:
        m_labelStatus->setText("Importing...");
        break;
    case Op_Export:
        m_labelStatus->setText("Exporting...");
        break;
    case Op_Picking:
        m_labelStatus->setText("Buzy in Corner detection...");
        break;
    case Op_Calibrating:
        m_labelStatus->setText("Buzy in Calibration...");
        break;
    case Op_Observing:
    default:
        m_labelStatus->setText("Ready");
        break;
    }
    updateBut();
}
void mainwindow::slotProgressEnd()
{
    if (m_state == Op_Picking) {
        m_spin_distK1->hide();
        m_spin_cornerSize->hide();
        m_spin_barSize->hide();
    }
    if (m_state == Op_Calibrating) {
        m_panelResult->setVisible(true);
    }
    m_state = Op_Observing;
    m_labelStatus->setText("Ready");
    updateBut();
    updateDisp();
}

void mainwindow::slotFindCorner()
{
    slotProgressBegin(Op_Picking);
    m_view->setCornerWinSize(m_spin_cornerSize->value());
    m_view->setBarWinSize(m_spin_barSize->value());
    m_view->pickCorners();
}
void mainwindow::slotSetCornerWinSize(int s)
{
    m_view->setCornerWinSize(s);
}
void mainwindow::slotSetBarWinSize(int s)
{
    m_view->setBarWinSize(s);
}
void mainwindow::slotUndistortionCorner(double c)
{
    m_view->undistortionCorner(c);
}
void mainwindow::slotClib()
{
    DlgPickView *pickDlg = new DlgPickView(&m_calibScene,this);
    if ( QDialog::Accepted == pickDlg->exec()) {
        float sizeX, sizeY;
        vector<int> list = pickDlg->getPickList();
        pickDlg->getSize(sizeX, sizeY);
        m_calibScene.setQuadSize(sizeX,sizeY);
        slotProgressBegin(Op_Calibrating);
        m_calibThread->calib(&m_calibScene, &list);
    }
    delete pickDlg;
}

void mainwindow::slotSettingIO()
{
    m_dlg_setting_IO->setParams(m_IO_camerafile,m_IO_calibfile,m_IO_gridpts,m_IO_Rt);
    if (QDialog::Accepted == m_dlg_setting_IO->show_IOSetting())
        m_dlg_setting_IO->getParams(m_IO_camerafile,m_IO_calibfile,m_IO_gridpts,m_IO_Rt);
}
void mainwindow::slotSettingCalib()
{
    m_dlg_setting_calib->setParams(m_estPrincipalPoint,m_estFocalRatio,m_estDistMode);
    if ( QDialog::Accepted == m_dlg_setting_calib->exec()) {
        m_dlg_setting_calib->getParams(m_estPrincipalPoint,m_estFocalRatio,m_estDistMode);
        m_calibThread->setCalibParams(m_estPrincipalPoint,m_estFocalRatio,m_estDistMode);
    }
}
void mainwindow::slotAbout()
{
    QMessageBox::about(this,
        "About Caliboy",
        "Caliboy                                                       \n"
        "--------------------------------------------------------------\n"
        "Version 2.0.0.201803, by WeiQM(https://weiquanmao.github.io). \n"
        "\n"
        "Caliboy is a calibration tool, which is a re-implementation of "
        "[Camera Calibration Toolbox for Matlab]"
        "(http://www.vision.caltech.edu/bouguetj/calib_doc/) "
        "in C++ with OpenCV and Qt.   \n"
        "\n"
        "@Github: https://github.com/CheckBoxStudio/CaliBoy\n"
        "@Project Page: https://checkboxstudio.github.io/CaliBoy/\n"
    );
}
void mainwindow::slotHowto()
{
    QWebEngineView *webView = new QWebEngineView();
    webView->setWindowIcon(QIcon(PIC_ICON));
    webView->setWindowTitle("Caliboy Help");
    webView->setContextMenuPolicy(Qt::NoContextMenu);
    webView->load(QUrl("https://checkboxstudio.github.io/CaliBoy/"));
    webView->show();
}

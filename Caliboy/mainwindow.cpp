#include "mainwindow.h"
#include "uiconfig.h"
#include "qtChinese.h"
#include "qmsg.h"

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
#include <QWebView>
#include <QMessageBox>
#include <QFileDialog>

#include "dlgpickview.h"
#include "dlgerrcheck.h"
#include "dlgsettingIO.h"
#include "dlgsettingcalib.h"
#include "calibresultviewer.h"
#include "infopanel.h"
#include "qimgviewer.h"
#include "qzoomview.h"

#include "threadCalib.h"
#include "parseTxt.h"


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
	m_actLoad_Files = new QAction(QIcon(PIC_OPEN_FILES),FromCHN("导入图像..."),this);
	m_actLoad_Folder = new QAction(QIcon(PIC_OPEN_FOLDER),FromCHN("导入文件夹..."),this);
	m_actLoad_ListTxt = new QAction(QIcon(PIC_OPEN_TXT),FromCHN("从列表导入..."),this);
	m_actAdd_Files = new QAction(QIcon(PIC_ADD_FILES),FromCHN("添加图像..."),this);
	m_actAdd_Folder = new QAction(QIcon(PIC_ADD_FOLDER),FromCHN("添加文件夹..."),this);
	m_actAdd_ListTxt = new QAction(QIcon(PIC_ADD_TXT),FromCHN("从列表添加..."),this);
	
	m_actImport = new QAction(FromCHN("导入标定结果..."),this);
	m_actExport = new QAction(QIcon(PIC_SAVE),FromCHN("导出标定结果..."),this);

	m_actView_calibReseult = new QAction(FromCHN("标定结果..."),this);
	m_actView_checkErr = new QAction(FromCHN("标定偏差..."),this);

	m_actSetting_IO = new QAction(FromCHN("导入\\导出设置..."),this);
	m_actSetting_Calib = new QAction(FromCHN("标定设置..."),this);

	m_actAbout = new QAction(FromCHN("关于..."),this);
	m_actHowto = new QAction(FromCHN("操作说明..."),this);


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
	m_menuIO = menuBar()->addMenu(FromCHN("导入\\导出"));
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

	m_menuSetting = menuBar()->addMenu(FromCHN("设置"));
	m_menuSetting->addAction(m_actSetting_Calib);
	m_menuSetting->addAction(m_actSetting_IO);

	m_menuView = menuBar()->addMenu(FromCHN("查看"));
	m_menuView->addAction(m_actView_calibReseult);
	m_menuView->addAction(m_actView_checkErr);

	m_menuHelp = menuBar()->addMenu(FromCHN("帮助"));
	m_menuHelp->addAction(m_actHowto);
	m_menuHelp->addSeparator();
	m_menuHelp->addAction(m_actAbout);
}
void mainwindow::createStatusBar()
{
	m_labelStatus = new QLabel("Ready");
	m_labelStatus->setAlignment(Qt::AlignHCenter);
	m_labelStatus->setAlignment(Qt::AlignVCenter);
	m_labelStatus->setMinimumSize(m_labelStatus->sizeHint());
	statusBar()->addWidget(m_labelStatus);
}
void mainwindow::createPanels()
{
	m_dlg_setting_IO = new IOSettingDialog(this);
	m_dlg_setting_calib = new CalibSettingDialog(this);

	m_resultViewer = new CalibResultViewer(&m_calibScene,this);
	addDockWidget(Qt::RightDockWidgetArea,m_resultViewer);

	QWidget *_centralWidget = new QWidget(this); 

	m_infoPanel = new InfoPanel(this);
	setOutBrowser(m_infoPanel->getBrowser());
	addDockWidget(Qt::LeftDockWidgetArea,m_infoPanel);

	m_view = new qimgviewer(this);
	m_zoomView = new qzoomview(this);
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

	m_butLoad = new QPushButton(FromCHN("导入图像"),this);
	m_butAdd = new QPushButton(FromCHN("添加图像"),this);
	m_butImport = new QPushButton(FromCHN("导入结果"),this);
	m_butExport = new QPushButton(FromCHN("导出结果"),this);
	m_butFindCorners = new QPushButton(FromCHN("检测角点"),this);
	m_butCliab = new QPushButton(FromCHN("相机标定"),this);
	m_butClear = new QPushButton(FromCHN(" 清  除 "),this);

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

	m_butOk = new QPushButton(FromCHN("确认"),this);
	m_butNo = new QPushButton(FromCHN("取消"),this);

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
	QFile txtFile("version");
	txtFile.open(QIODevice::ReadOnly);
	QString txt = QString(txtFile.readAll());
	txtFile.close();
	msgOut(txt);
	msgOut("------------------------------------------");
}
void mainwindow::readSettings()
{
	QSettings settings("setting.ini", QSettings::IniFormat);

	m_estFocalRatio		= settings.value("Setting_Calib/bEstFocalRatio").toBool();
	m_estPrincipalPoint = settings.value("Setting_Calib/bEstPrincipalPoint").toBool();
	m_estDistMode		= settings.value("Setting_Calib/iEstDistModo").toInt();
	
	m_IO_camerafile		= settings.value("Setting_IO/camera_file").toString();
	m_IO_calibfile		= settings.value("Setting_IO/calib_file").toString();
	m_IO_gridpts		= settings.value("Setting_IO/bGridPts").toBool();
	m_IO_Rt				= settings.value("Setting_IO/bRtPos").toBool();
}
void mainwindow::writeSettings()
{
	QSettings settings("setting.ini", QSettings::IniFormat);

	settings.setValue("Setting_Calib/bEstPrincipalPoint", m_estPrincipalPoint);
	settings.setValue("Setting_Calib/bEstFocalRatio"	, m_estFocalRatio);
	settings.setValue("Setting_Calib/iEstDistModo"		, m_estDistMode);


	settings.setValue("Setting_IO/camera_file"			, m_IO_camerafile);
	settings.setValue("Setting_IO/calib_file"			, m_IO_calibfile);
	settings.setValue("Setting_IO/bGridPts"				, m_IO_gridpts);
	settings.setValue("Setting_IO/bRtPos"				, m_IO_Rt);
}

void mainwindow::updateBut()
{
	bool bHasView = (m_calibScene.viewCount() == 0)? false : true;
	bool bOn = (m_state == Op_Observing) ?  true:false;

	m_actLoad_Files->setEnabled(bOn);
	m_actLoad_Folder->setEnabled(bOn);
	m_actLoad_ListTxt->setEnabled(bOn);
	m_actAdd_Files->setEnabled(bOn && bHasView);
	m_actAdd_Folder->setEnabled(bOn && bHasView);
	m_actAdd_ListTxt->setEnabled(bOn && bHasView);
	m_actImport->setEnabled(bOn && bHasView);
	m_actExport->setEnabled(bOn && bHasView);


	m_butPreviousOne->setEnabled(bOn && bHasView);
	m_butNextOne->setEnabled(bOn && bHasView);
	m_infoIndex->setReadOnly(!(bOn && bHasView));

	m_butLoad->setEnabled(bOn);
	m_butAdd->setEnabled(bOn && bHasView);
	m_butFindCorners->setEnabled(bOn && bHasView);
	m_butCliab->setEnabled(bOn && bHasView);
	m_butImport->setEnabled(bOn && bHasView);
	m_butExport->setEnabled(bOn && bHasView);
	m_butClear->setEnabled(bOn && bHasView);

	if (m_state == Op_Picking) {
		m_butOk->show();
		m_butNo->show();
		m_butOk->setEnabled(false);
		m_spin_cornerSize->show();
		m_spin_barSize->show();
	} else {
		m_butOk->hide();
		m_butNo->hide();
		m_spin_cornerSize->hide();
		m_spin_barSize->hide();
	}
}


void mainwindow::updateDisp()
{
	if (m_calibScene.viewCount() == 0){
		m_view->cleanView();
		m_infoIndex->setText("0");
		m_infoCount->setText("/0");
		m_infoName->setText("");

		m_infoPanel->updateCameraView(0);
		m_infoPanel->updateCamera(0);

		m_dispIndex = -1;
	}
	if (m_calibScene.viewCount()>0){
		if (m_dispIndex>=m_calibScene.viewCount() || 
			m_dispIndex<0)
			m_dispIndex = 0;
	
			m_view->setView(m_calibScene.m_views.at(m_dispIndex));
			m_infoIndex->setText(QString("%1").arg(m_dispIndex+1));
			m_infoCount->setText(QString("/%1").arg(m_calibScene.viewCount()));
			m_infoName->setText(m_calibScene.m_views.at(m_dispIndex)->getFileName());

			m_infoPanel->updateCameraView(m_calibScene.m_views.at(m_dispIndex));
			m_infoPanel->updateCamera(&m_calibScene.m_camera);
	}
}
void mainwindow::removeImages(const QStringList &fileList)
{
	if (! fileList.isEmpty()) {
		msgOut("\n移除图像...");
		int imgCnt = fileList.size();
		for (int i=0; i<imgCnt; i++) {

			QString fileName = fileList.at(i);
			int n = m_calibScene.contains(fileName);
			if (n == -1) {
				_msgOut(FromCHN("%1/%2:[未加载]%3 ").arg(i+1).arg(imgCnt).arg(fileName));
				continue;
			} else {
				_msgOut(FromCHN("%1/%2:[移除]%3 ").arg(i+1).arg(imgCnt).arg(fileName));
				m_calibScene.removeView(fileName);
				if (m_dispIndex>n)
					m_dispIndex--;
			}
		}
		updateDisp();
		m_resultViewer->updateScene();
		m_resultViewer->setView(m_dispIndex);
	}
}
void mainwindow::loadImages(const QStringList &fileList)
{
	if (! fileList.isEmpty()) {
		msgOut("\n导入图像...");
		int imgCnt = fileList.size();
		for (int i=0; i<imgCnt; i++) {
			QString fileName = fileList.at(i);

			if (m_calibScene.contains(fileName) != -1) {
				_msgOut(FromCHN("%1/%2:[已加载]%3").arg(i+1).arg(imgCnt).arg(fileName));
				continue;
			} else {		
				cameraView *newView = new cameraView(fileName);
				if (newView->getQImage() != 0) {
					_msgOut(FromCHN("%1/%2:[成功]%3").arg(i+1).arg(imgCnt).arg(fileName));
					m_calibScene.addView(newView);
				}
				else {
					_msgOut(FromCHN("%1/%2:[失败]%3").arg(i+1).arg(imgCnt).arg(fileName));
					delete newView;
				}
			}
		}
		updateDisp();
		m_resultViewer->updateScene();
		m_resultViewer->setView(m_dispIndex);
	}
	if (m_calibScene.viewCount() > 0) {
		m_infoIndex->setReadOnly(false);
		m_infoIndex->setValidator(new QIntValidator(1,m_calibScene.viewCount()));
	}
}
void mainwindow::clearScene()
{
	m_infoIndex->setReadOnly(true);
	m_calibScene.clean();
	updateDisp();
	m_resultViewer->updateScene();
	m_resultViewer->setView(m_dispIndex);
}

void importView(cameraScene *scene, FILE *fin,
				const bool bGridPts,const bool bRt)
{
	if (scene == 0 ||
		fin == NULL ||
		(!bGridPts && !bRt))
		return;

	char fileName[1024];
	while (fgets(fileName,1024,fin)!=NULL) {
		int nx, ny;
		fscanf_s(fin,"%d %d\n",&nx, &ny);
		for (int i=0; i<1024; ++i)
			if (fileName[i] == '\n')
				fileName[i] = '\0';
		int viewIdx = scene->contains(QString(fileName));
		if (viewIdx == -1) {
			char buff[1024];
			for (int i=0; i<ny+2; ++i)
				fgets(buff,1024,fin);
		}
		else {
			_msgOut(FromCHN("导入标定信息:%1").arg(fileName));
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
void exportView(cameraView *view, FILE *fout,
				const bool bGridPts,const bool bRt)
{
	if (fout == NULL)
		return;
	fprintf(fout,"%s\n",ToCHN(QFileInfo(view->getFileName()).absoluteFilePath()));
	
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
void mainwindow::importResults(const QString cameraFile, const QString calibFile,
							   const bool bGridPts, const bool bRt)
{
	msgOut("\n导入结果...");
	if (!cameraFile.isEmpty()) {
		_msgOut(FromCHN("导入相机参数->%1").arg(cameraFile));
		if (!QFileInfo(cameraFile).exists())
			_msgOut(FromCHN("相机文件%1不存在!").arg(cameraFile));
		else {
			FILE *fin_c = 0;
			fopen_s(&fin_c,ToCHN(cameraFile), "r");
			if (fin_c == NULL)
				_msgOut("文件打开失败!");
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
		_msgOut(FromCHN("导入标定结果->%1").arg(calibFile));
		if (!QFileInfo(calibFile).exists())
			_msgOut(FromCHN("文件%1不存在!").arg(calibFile));
		else {
			FILE *fin_calib = 0;
			fopen_s(&fin_calib,ToCHN(calibFile), "r");
			if (fin_calib == NULL)
				_msgOut("文件打开失败!");
			else {
				importView(&m_calibScene, fin_calib,bGridPts,bRt);
				fclose(fin_calib);
			}
		}
	}

	updateDisp();
	m_resultViewer->updateScene();
	m_resultViewer->setView(m_dispIndex);
}
void mainwindow::exportResults(const QString cameraFile, const QString calibFile,
							   const bool bGridPts, const bool bRt)
{
	msgOut("\n导出结果...");
	if (!cameraFile.isEmpty()) {
		_msgOut(FromCHN("导出相机参数->%1").arg(cameraFile));
		FILE *fout_c = 0;
		fopen_s(&fout_c,ToCHN(cameraFile), "w");
		if (fout_c == NULL)
			_msgOut("文件打开失败!");
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
		_msgOut(FromCHN("导出标定结果->%1").arg(calibFile));
		FILE *fout_calib = 0;
		fopen_s(&fout_calib,ToCHN(calibFile), "w");
		if (fout_calib == NULL)
			_msgOut("文件打开失败!");
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
	updateDisp();
	m_resultViewer->setView(m_dispIndex);
	slotProgressEnd();
}
void mainwindow::slotNextView()
{
	slotProgressBegin(Op_Observing);
	m_dispIndex++;
	if (m_dispIndex == m_calibScene.viewCount())
		m_dispIndex = 0;
	updateDisp();
	m_resultViewer->setView(m_dispIndex);
	slotProgressEnd();
}
void mainwindow::slotSetNView()
{
	slotProgressBegin(Op_Observing);
	m_dispIndex = m_infoIndex->text().toInt()-1;
	updateDisp();
	m_resultViewer->setView(m_dispIndex);
	slotProgressEnd();
}

void mainwindow::slotLoadFiles()
{
	slotProgressBegin(Op_Import);
	QStringList fileList = QFileDialog::getOpenFileNames( this,
		FromCHN("导入图像..."), "", "Image File (*.bmp;*.jpg;*.tif;*.tiff;*.pgm;*.png)");
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
	QString folder = QFileDialog::getExistingDirectory(this, FromCHN("导入文件夹..."));
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
		FromCHN("打开文件列表..."), "",
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
	m_dlg_setting_IO->setParams(m_IO_camerafile,m_IO_calibfile,m_IO_gridpts,m_IO_Rt);
	if (QDialog::Accepted == m_dlg_setting_IO->show_Import()) {
		QString cameraFile,  calibFile;
		bool bGridPts, bRt;
		m_dlg_setting_IO->getParams(cameraFile,calibFile,bGridPts, bRt);
		importResults(cameraFile,calibFile,bGridPts, bRt);
	}
}
void mainwindow::slotExport()
{
	m_dlg_setting_IO->setParams(m_IO_camerafile,m_IO_calibfile,m_IO_gridpts,m_IO_Rt);
	if (QDialog::Accepted == m_dlg_setting_IO->show_Export()) {
		QString cameraFile,  calibFile;
		bool bGridPts, bRt;
		m_dlg_setting_IO->getParams(cameraFile,calibFile,bGridPts, bRt);
		exportResults(cameraFile,calibFile,bGridPts, bRt);
	}
}
void mainwindow::slotClearScene()
{
	slotProgressBegin(Op_Observing);
	msgOut("\n清除场景...");
	clearScene();
	msgOut("\n********************************");
	msgOut("\n================================");
	slotProgressEnd();
}

void mainwindow::slotViewCalibResult()
{
	m_resultViewer->setVisible(true);
}
void mainwindow::slotErrorCalibResult()
{
	if (m_calibScene.viewCount() == 0 ||
		m_calibScene.m_camera.cx < 10.0) {
		msgOut("\n场景未标定过!");
		return;
	}
	ErrorCheckDialog *errDlg = new ErrorCheckDialog(&m_calibScene,this);
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
		m_calibScene.m_views.at(m_dispIndex)->setGridCorners(m_view->getNX(), m_view->getNY(), m_view->getGridCorners());
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
		m_labelStatus->setText(FromCHN("导入图像..."));
		break;
	case Op_Export:
		m_labelStatus->setText(FromCHN("导除数据..."));
		break;
	case Op_Picking:
		m_labelStatus->setText(FromCHN("角点检测..."));
		break;
	case Op_Calibrating:
		m_labelStatus->setText(FromCHN("标定..."));
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
		updateDisp();
		m_resultViewer->updateScene();
		m_resultViewer->setView(m_dispIndex);
		m_resultViewer->setVisible(true);
	}
	m_state = Op_Observing;
	m_labelStatus->setText("Ready");
	updateBut();
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
	ViewPickDialog *pickDlg = new ViewPickDialog(&m_calibScene,this);
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
	QFile txtFile("version");
	txtFile.open(QIODevice::ReadOnly);
	QString txt = QString(txtFile.readAll());
	txtFile.close();
	QMessageBox::about(this,FromCHN("关于Caliboy"),txt);
}
void mainwindow::slotHowto()
{
	QWebView *webView = new QWebView();
	webView->setWindowIcon(QIcon(PIC_ICON));
	webView->setWindowTitle("Caliboy Help");
	webView->setContextMenuPolicy(Qt::NoContextMenu);
	QFileInfo fileInfo("./Doc/html/home.html");
	webView->load(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
	webView->show();
}

#include "dlgsettingIO.h"
#include <QGroupBox>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLayout>
#include <QFileDialog>
#include "qtChinese.h"


IOSettingDialog::IOSettingDialog(QWidget *parent)
	: QDialog(parent)
{	
	setFont(QFont(FromCHN("微软雅黑")));
	setupme();
}
IOSettingDialog::~IOSettingDialog(void)
{

}
void IOSettingDialog::setParams(const QString cameraFile, const QString calibFile,
								const bool bGridPts, const bool bRt)
{
	m_text_cameraFile->setText(cameraFile);
	m_text_calibFile->setText(calibFile);
	m_checkbox_calib_gridPts->setChecked(bGridPts);
	m_checkbox_calib_Rt->setChecked(bRt);
}
void IOSettingDialog::getParams(QString &cameraFile, QString &calibFile,
								bool &bGridPts, bool &bRt)
{
	cameraFile = m_text_cameraFile->text();
	calibFile = m_text_calibFile->text();
	bGridPts = m_checkbox_calib_gridPts->isChecked();
	bRt = m_checkbox_calib_Rt->isChecked();
}

void IOSettingDialog::openImportPath()
{
	QPushButton *senderBut = qobject_cast<QPushButton*>( sender() );
	QString oldPath, path;
	if (senderBut == m_but_browseCameraFile)
		oldPath = m_text_cameraFile->text();
	else
		oldPath = m_text_calibFile->text();

	if (m_state == _import)
		path = QFileDialog::getOpenFileName(this,FromCHN("导入..."),oldPath);
	else
		path = QFileDialog::getSaveFileName(this,FromCHN("打开..."),oldPath);
	
	if (!path.isEmpty()) {
		if (senderBut == m_but_browseCameraFile)
			m_text_cameraFile->setText(path);
		else
			m_text_calibFile->setText(path);
	}
}
void IOSettingDialog::setupme()
{
	m_but_ok = new QPushButton(FromCHN("确定"));
	m_but_cancel = new QPushButton(FromCHN("取消"));
	connect(m_but_ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_but_cancel, SIGNAL(clicked()), this, SLOT(reject()));

	m_groupbox_camera = new QGroupBox(FromCHN("相机"),this);
	QLabel* label_camera = new QLabel(FromCHN("路径："),this);
	m_text_cameraFile = new QLineEdit();
	m_text_cameraFile->setReadOnly(true);
	m_but_browseCameraFile = new QPushButton(FromCHN("浏览..."),this);
	connect(m_but_browseCameraFile, SIGNAL(clicked()), this, SLOT(openImportPath()));

	m_groupbox_calib = new QGroupBox(FromCHN("标定结果"),this);
	QLabel* label_calib = new QLabel(FromCHN("路径："),this);
	m_text_calibFile = new QLineEdit();
	m_text_calibFile->setReadOnly(true);
	m_but_browseCalibFile = new QPushButton(FromCHN("浏览..."),this);
	connect(m_but_browseCalibFile, SIGNAL(clicked()), this, SLOT(openImportPath()));

	m_checkbox_calib_gridPts = new QCheckBox(FromCHN("角点坐标"),this);
	m_checkbox_calib_Rt = new QCheckBox(FromCHN("相对位置"),this);

	QHBoxLayout *hLay1 = new QHBoxLayout();
	QHBoxLayout *hLay2 = new QHBoxLayout();
	QHBoxLayout *hLay3 = new QHBoxLayout();
	QHBoxLayout *hLay4 = new QHBoxLayout();
	QVBoxLayout *vLay1 = new QVBoxLayout();
	QVBoxLayout *vLay2 = new QVBoxLayout();

	hLay1->addWidget(label_camera);
	hLay1->addWidget(m_text_cameraFile);
	hLay1->addWidget(m_but_browseCameraFile);
	m_groupbox_camera->setLayout(hLay1);

	hLay2->addWidget(label_calib);
	hLay2->addWidget(m_text_calibFile);
	hLay2->addWidget(m_but_browseCalibFile);
	hLay3->addWidget(m_checkbox_calib_gridPts);
	hLay3->addWidget(m_checkbox_calib_Rt);
	hLay3->addStretch();
	vLay1->addLayout(hLay2);
	vLay1->addLayout(hLay3);
	m_groupbox_calib->setLayout(vLay1);


	hLay4->addStretch();
	hLay4->addWidget(m_but_ok);
	hLay4->addWidget(m_but_cancel);

	vLay2->addWidget(m_groupbox_camera);
	vLay2->addWidget(m_groupbox_calib);
	vLay2->addLayout(hLay4);

	setLayout(vLay2);
	setFixedHeight(sizeHint().height());
}
int IOSettingDialog::show_IOSetting()
{
	setWindowTitle(FromCHN("参数设置_导入导出"));
	m_state = _setting;
	return exec();
}
int IOSettingDialog::show_Import()
{
	setWindowTitle(FromCHN("导入"));
	m_state = _import;
	return exec();
}
int IOSettingDialog::show_Export()
{
	setWindowTitle(FromCHN("导出"));
	m_state = _export;
	return exec();
}
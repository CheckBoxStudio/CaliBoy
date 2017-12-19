#include "dlgsettingcalib.h"
#include <Qlabel>
#include <QComboBox>
#include <QPushButton>

#include <QLayout>
#include "qtChinese.h"


CalibSettingDialog::CalibSettingDialog(QWidget *parent)
	: QDialog(parent)
	, m_estPrincipalPoint(true)
	, m_estFocalRatio(true)
	, m_estDistMode(5)
{	
	setWindowTitle(FromCHN("参数设置_标定"));
	setFont(QFont(FromCHN("微软雅黑")));
	setupme();
	updateComboes();
}
CalibSettingDialog::CalibSettingDialog(
	bool estPrincipalPoint, bool estFocalRatio,
	unsigned int estDistMode,
	QWidget *parent)
	: QDialog(parent)
	, m_estPrincipalPoint(estPrincipalPoint)
	, m_estFocalRatio(estFocalRatio)
	, m_estDistMode(estDistMode)
{	
	setWindowTitle(FromCHN("参数设置"));
	setFont(QFont(FromCHN("微软雅黑")));
	setupme();
	if (estDistMode>5)
		estDistMode = 5;
	updateComboes();
}
CalibSettingDialog::~CalibSettingDialog(void)
{

}

void CalibSettingDialog::setupme()
{
	m_but_ok = new QPushButton(FromCHN("确定"));
	m_but_cancel = new QPushButton(FromCHN("取消"));
	connect(m_but_ok, SIGNAL(clicked()), this, SLOT(apply()));
	connect(m_but_cancel, SIGNAL(clicked()), this, SLOT(reject()));

	QLabel *label1 = new QLabel(FromCHN("主点位置:"),this);
	QLabel *label2 = new QLabel(FromCHN("fx / fy:"),this);
	QLabel *label3 = new QLabel(FromCHN("畸变参数:"),this);
	label1->setAlignment(Qt::AlignRight);
	label2->setAlignment(Qt::AlignRight);
	label3->setAlignment(Qt::AlignRight);

	m_combo_estPrincipalPoint	= new QComboBox(this);
	m_combo_estFocalRatio		= new QComboBox(this);
	m_combo_estDistMode			= new QComboBox(this);

	m_combo_estPrincipalPoint->addItem(FromCHN("固定为图像中心"));
	m_combo_estPrincipalPoint->addItem(FromCHN("参与优化"));

	m_combo_estFocalRatio->addItem(FromCHN("固定为1"));
	m_combo_estFocalRatio->addItem(FromCHN("不固定"));

	m_combo_estDistMode->addItem(FromCHN("无.            ([k1,k2,p1,p2,k3]==0)"));
	m_combo_estDistMode->addItem(FromCHN("k1.            ([--,k2,p1,p2,k3]==0)"));
	m_combo_estDistMode->addItem(FromCHN("k1,k2.         ([--,--,p1,p2,k3]==0)"));
	m_combo_estDistMode->addItem(FromCHN("k1,k2,p1,p2.   ([--,--,--,--,k3]==0)"));
	m_combo_estDistMode->addItem(FromCHN("k1,k2,k3.      ([--,--,p1,p2,--]==0)"));
	m_combo_estDistMode->addItem(FromCHN("k1,k2,p1,p2,k3."));

	QGridLayout *gridLay = new QGridLayout();
	QHBoxLayout *hLay = new QHBoxLayout();
	QVBoxLayout *vLay = new QVBoxLayout();
	gridLay->addWidget(label1,0,0);
	gridLay->addWidget(m_combo_estPrincipalPoint,0,1);
	gridLay->addWidget(label2,1,0);
	gridLay->addWidget(m_combo_estFocalRatio,1,1);
	gridLay->addWidget(label3,2,0);
	gridLay->addWidget(m_combo_estDistMode,2,1);

	hLay->addStretch();
	hLay->addWidget(m_but_ok);
	hLay->addWidget(m_but_cancel);

	vLay->addLayout(gridLay);
	vLay->addLayout(hLay);

	setLayout(vLay);

	setFixedSize(sizeHint());
}
void CalibSettingDialog::updateComboes()
{
	if (m_estPrincipalPoint)
		m_combo_estPrincipalPoint->setCurrentIndex(1);
	else
		m_combo_estPrincipalPoint->setCurrentIndex(0);

	if (m_estFocalRatio)
		m_combo_estFocalRatio->setCurrentIndex(1);
	else
		m_combo_estFocalRatio->setCurrentIndex(0);

	m_combo_estDistMode->setCurrentIndex(m_estDistMode);
}
void CalibSettingDialog::apply()
{
	if (m_combo_estPrincipalPoint->currentIndex() == 0)
		m_estPrincipalPoint = false;
	else
		m_estPrincipalPoint = true;
	if (m_combo_estFocalRatio->currentIndex() == 0)
		m_estFocalRatio = false;
	else
		m_estFocalRatio = true;

	m_estDistMode = m_combo_estDistMode->currentIndex();

	QDialog::accept();
}

void CalibSettingDialog::setParams(
	bool estPrincipalPoint, bool estFocalRatio,
	unsigned int estDistMode)
{
	m_estPrincipalPoint = estPrincipalPoint;
	m_estFocalRatio		= estFocalRatio;
	m_estDistMode	    = estDistMode;
	if (m_estDistMode>5)
		m_estDistMode = 5;
	updateComboes();
}

void CalibSettingDialog::getParams(
	bool &estPrincipalPoint, bool &estFocalRatio,
	unsigned int &estDistMode)
{
	estPrincipalPoint = m_estPrincipalPoint;
	estFocalRatio	  = m_estFocalRatio;
	estDistMode		  = m_estDistMode;
}
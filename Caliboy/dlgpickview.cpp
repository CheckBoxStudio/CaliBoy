#include "dlgpickview.h"
#include <Qlabel>
#include <QCheckBox>
#include <QLayout>
#include "qtChinese.h"

#include "Calib\camera.h"

ViewPickDialog::ViewPickDialog(cameraScene *scene,QWidget *parent)
	: QDialog(parent)
{	
	setWindowTitle(FromCHN("参数设置"));
	setFont(QFont(FromCHN("微软雅黑")));
	QLabel* _label = new QLabel(FromCHN("方格尺寸X|Y(mm):"),this);
	m_sizex = new QLineEdit(this);
	m_sizex->setValidator( new QDoubleValidator(0.000001,999999,0));
	m_sizex->setText("30");
	m_sizey = new QLineEdit(this);
	m_sizey->setValidator( new QDoubleValidator(0.000001,999999,0));
	m_sizey->setText("30");
	m_butok = new QPushButton(FromCHN("确定"),this);
	m_butclose = new QPushButton(FromCHN("取消"),this);
	
	//
	m_tw = new QTableWidget(scene->viewCount()+1, 4, this);
	m_tw->horizontalHeader()->setStretchLastSection(true);
	m_tw->horizontalHeader()->hide();
	m_tw->verticalHeader()->hide();
	m_tw->setShowGrid(true);
	
	QCheckBox *bCheckAll = new QCheckBox();
	QTableWidgetItem* _noitem = new QTableWidgetItem("No");
	QTableWidgetItem* _sizeitem = new QTableWidgetItem("Size");
	QTableWidgetItem* _nameitem = new QTableWidgetItem("Name");

	bCheckAll->setCheckState(Qt::Checked);
	_noitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
	_noitem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	_sizeitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
	_sizeitem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	_nameitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
	_nameitem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	m_tw->setCellWidget(0,0,bCheckAll);
	m_tw->setItem(0,1,_noitem);
	m_tw->setItem(0,2,_sizeitem);
	m_tw->setItem(0,3,_nameitem);

	for (int ii = 0;ii < scene->viewCount();++ii)
	{
		cameraView *_view = scene->m_views.at(ii);
		QTableWidgetItem* noitem = new QTableWidgetItem(QString("%1").arg(ii+1));
		QTableWidgetItem* sizeitem = new QTableWidgetItem(QString("%1x%2").arg(_view->getGridSizeX()).arg(_view->getGridSizeY()));
		QTableWidgetItem* nameitem = new QTableWidgetItem(_view->getFileName());
			
		noitem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		nameitem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		sizeitem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QCheckBox *bCheck = new QCheckBox();
		if (_view->isGridCornerPicked()) {
			bCheck->setChecked(true);
			noitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable));
			nameitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable));
			sizeitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable));
		}
		else {
			noitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
			nameitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
			sizeitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
			bCheck->setEnabled(false);
		}
		
		m_tw->setCellWidget(ii+1,0,bCheck);
		m_tw->setItem(ii+1,1,noitem);
		m_tw->setItem(ii+1,2,sizeitem);
		m_tw->setItem(ii+1,3,nameitem);

		connect(bCheck, SIGNAL(clicked(bool)), this, SLOT(checkChanged()));
	}
	m_tw->resizeColumnsToContents();
    m_tw->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	//
	QHBoxLayout *hLay1 =new QHBoxLayout();
	QHBoxLayout *hLay2 =new QHBoxLayout();
	QVBoxLayout *mainLay = new QVBoxLayout();
	hLay1->addWidget(_label);
	hLay1->addWidget(m_sizex);
	hLay1->addWidget(m_sizey);
	hLay1->addStretch();
	hLay2->addStretch();
	hLay2->addWidget(m_butok);
	hLay2->addWidget(m_butclose);
	mainLay->addLayout(hLay1);
	mainLay->addWidget(m_tw);
	mainLay->addLayout(hLay2);
	setLayout(mainLay);
	int totlen = m_tw->columnWidth(0) + m_tw->columnWidth(1) + this->frameSize().width();
	setMinimumWidth(totlen);

	connect( bCheckAll, SIGNAL(clicked(bool)), this, SLOT(checkAllChanged()));
	connect( m_butok, SIGNAL(clicked()), this, SLOT(apply()) );	
	connect( m_butclose, SIGNAL(clicked()), this, SLOT(close()) );	
}
ViewPickDialog::~ViewPickDialog(void)
{
	m_tw->clear();
	m_PickList.clear();
}

void ViewPickDialog::checkAll(bool bOn)
{
	for (int i=1; i<m_tw->rowCount(); ++i) {
		QCheckBox *checkBut = (QCheckBox*)m_tw->cellWidget(i,0);
		if (checkBut->isEnabled())
			checkBut->setChecked(bOn);
	}
}

void ViewPickDialog::checkAllChanged()
{
	int state = ((QCheckBox*)m_tw->cellWidget(0,0))->checkState();
	switch (state)
	{
	case Qt::Unchecked:
		checkAll(false);
		break;
	case Qt::PartiallyChecked:
		((QCheckBox*)m_tw->cellWidget(0,0))->setCheckState(Qt::Checked);
	case Qt::Checked:
		checkAll(true);
		break;
	}
}
void ViewPickDialog::checkChanged()
{
	bool hasChecked = false;
	bool hasUnChecked = false;
	for (int i=1; i<m_tw->rowCount(); ++i) {
		QCheckBox *checkBut = (QCheckBox*)m_tw->cellWidget(i,0);
		if (checkBut->isEnabled()) {
			if (checkBut->isChecked())
				hasChecked = true;
			else
				hasUnChecked = true;
		}
		if (hasChecked && hasUnChecked)
			break;
	}
	if (hasChecked && !hasUnChecked)
		((QCheckBox*)m_tw->cellWidget(0,0))->setCheckState(Qt::Checked);
	if (!hasChecked && hasUnChecked)
		((QCheckBox*)m_tw->cellWidget(0,0))->setCheckState(Qt::Unchecked);
	if (hasChecked && hasUnChecked)
		((QCheckBox*)m_tw->cellWidget(0,0))->setCheckState(Qt::PartiallyChecked);
}
void ViewPickDialog::apply()
{
	for (int i=1; i<m_tw->rowCount(); ++i) {
		QCheckBox *checkBut = (QCheckBox*)m_tw->cellWidget(i,0);
		if (checkBut->isChecked())
			m_PickList.push_back(i-1);
	}
	QDialog::accept();
}
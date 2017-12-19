#include "infopanel.h"
#include "Calib\camera.h"

#include <QTextBrowser>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>

InfoPanel::InfoPanel(QWidget *parent)
	: QDockWidget(parent)
{
	setWindowTitle(QString::fromLocal8Bit("运行信息"));
	setFeatures(QDockWidget::NoDockWidgetFeatures | QDockWidget::DockWidgetMovable);
	setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	setAttribute(Qt::WA_DeleteOnClose);
	setupme();
}
InfoPanel::~InfoPanel()
{

}
void InfoPanel::setupme()
{
	dockWidgetContents = new QWidget();
    setWidget(dockWidgetContents);

	m_textBrowser = new QTextBrowser(this);
	m_textBrowser->setLineWrapMode(QTextEdit::NoWrap);
	m_label_K = new QLabel("K:");
	m_label_k = new QLabel("k:");
	m_label_om = new QLabel("r:");
	m_label_R = new QLabel("R:");
	m_label_t = new QLabel("t");
	m_label_K->setAlignment(Qt::AlignRight);
	m_label_k->setAlignment(Qt::AlignRight);
	m_label_om->setAlignment(Qt::AlignRight);
	m_label_R->setAlignment(Qt::AlignRight);
	m_label_t->setAlignment(Qt::AlignRight);

	m_edit_K = new QTextBrowser(this);
	m_edit_k = new QLineEdit(this);
	m_edit_om = new QLineEdit(this);
	m_edit_R = new QTextBrowser(this);
	m_edit_t = new QLineEdit(this);
	m_edit_K->setReadOnly(true);
	m_edit_K->setFixedHeight(60);
	m_edit_k->setReadOnly(true);
	m_edit_om->setReadOnly(true);
	m_edit_R->setReadOnly(true);
	m_edit_R->setFixedHeight(60);
	m_edit_t->setReadOnly(true);
	m_edit_K->setText("[ [?,?,?] \n"
					  "  [?,?,?] \n"
					  "  [?,?,?] ]");
	m_edit_k->setText("[?,?,?,?,?]");
	m_edit_om->setText("[?,?,?]");
	m_edit_R->setText("[ [?,?,?] \n"
					  "  [?,?,?] \n"
					  "  [?,?,?] ]");
	m_edit_t->setText("[?,?,?]");

	QGridLayout *layout = new QGridLayout();
	layout->addWidget(m_textBrowser,0,0,1,2);
	layout->addWidget(m_label_K,1,0,1,1);
	layout->addWidget(m_edit_K,1,1,1,1);
	layout->addWidget(m_label_k,2,0,1,1);
	layout->addWidget(m_edit_k,2,1,1,1);
	layout->addWidget(m_label_om,3,0,1,1);
	layout->addWidget(m_edit_om,3,1,1,1);
	layout->addWidget(m_label_R,4,0,1,1);
	layout->addWidget(m_edit_R,4,1,1,1);
	layout->addWidget(m_label_t,5,0,1,1);
	layout->addWidget(m_edit_t,5,1,1,1);
	layout->setSpacing(4);
	layout->setContentsMargins(2,0,2,10);
	dockWidgetContents->setLayout(layout);
}
void InfoPanel::updateCamera(camera *cam)
{
	if (cam == 0) {
		m_edit_K->setText("[ [?,?,?] \n"
						  "  [?,?,?] \n"
						  "  [?,?,?] ]");
		m_edit_k->setText("[?,?,?,?,?]");
	} else {
		m_edit_K->setText(QString("[ [%1, 0, %3] \n"
								  "  [0, %2, %4] \n"
								  "  [0, 0, 1] ]")
								  .arg(cam->fx).arg(cam->fy)
								  .arg(cam->cx).arg(cam->cy));
	
		m_edit_k->setText(QString("[%1, %2, %3, %4, %5]")
			.arg(cam->k[0])
			.arg(cam->k[1])
			.arg(cam->k[2])
			.arg(cam->k[3])
			.arg(cam->k[4]));
	}
}
void InfoPanel::updateCameraView(cameraView *view)
{
	if (view == 0) {
		m_edit_om->setText("[?,?,?]");
		m_edit_R->setText("[ [?,?,?] \n"
						  "  [?,?,?] \n"
						  "  [?,?,?] ]");
		m_edit_t->setText("[?,?,?]");
	} else {
		m_edit_om->setText(QString("[%1, %2, %3]")
						   .arg(view->r.at<double>(0)).arg(view->r.at<double>(1)).arg(view->r.at<double>(2)));
		m_edit_R->setText(QString("[ [%1, %2, %3] \n"
								  "  [%4, %5, %6] \n"
								  "  [%7, %8, %9] ]")
								  .arg(view->R.at<double>(0)).arg(view->R.at<double>(1)).arg(view->R.at<double>(2))
								  .arg(view->R.at<double>(3)).arg(view->R.at<double>(4)).arg(view->R.at<double>(5))
								  .arg(view->R.at<double>(6)).arg(view->R.at<double>(7)).arg(view->R.at<double>(8)));
		m_edit_t->setText(QString("[%1, %2, %3]")
			.arg(view->t.at<double>(0)).arg(view->t.at<double>(1)).arg(view->t.at<double>(2)));
	}
}		
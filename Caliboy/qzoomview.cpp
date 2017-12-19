#include "qzoomview.h"
#include "uiconfig.h"

#include <QLayout>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QLabel>
#include <QLineEdit>

#include "qimgviewer.h"

qzoomview::qzoomview(QWidget *parent)
	: QWidget(parent)
	, m_sourceView(0)
	, m_scale(I_SCALE)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setupme();
}
qzoomview::~qzoomview()
{
	cleanDrawed();
}
void qzoomview::setupme()
{
	setStyleSheet("border:0;");
	m_labelX = new QLabel("X",this);
	m_labelY = new QLabel("Y",this);
	m_labelX->setAlignment(Qt::AlignCenter);
	m_labelX->setFixedWidth(15);
	m_labelY->setAlignment(Qt::AlignCenter);
	m_labelY->setFixedWidth(15);
	m_labelX->setStyleSheet("background-color: rgb(199, 199, 199);");
	m_labelY->setStyleSheet("background-color: rgb(199, 199, 199);");
	m_XLoc = new QLineEdit(this);
	m_XLoc->setReadOnly(true);
	m_XLoc->setText("0");
	m_YLoc = new QLineEdit(this);
	m_YLoc->setReadOnly(true);
	m_YLoc->setText("0");
	m_XLoc->setFixedWidth(SZ_ZOOMVIEW/2-10);
	m_YLoc->setFixedWidth(SZ_ZOOMVIEW/2-10);

	m_GScene = new QGraphicsScene(this);
	m_GView = new QGraphicsView(this);
	m_GScene->setBackgroundBrush(QBrush(QColor(C_BACKDOT), Qt::Dense7Pattern));
	m_GView->setSceneRect(0,0,SZ_ZOOMVIEW+1,SZ_ZOOMVIEW+1);
	m_GView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_GView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_GView->setScene(m_GScene);
	m_GView->setFixedWidth(SZ_ZOOMVIEW+19);
	m_GView->setFixedHeight(SZ_ZOOMVIEW+19);

	m_GPixItem = m_GScene->addPixmap(QPixmap());
	m_GPixItem->setZValue(-2);
	m_GCorssItem[0] = m_GScene->addLine(
		SZ_ZOOMVIEW/2-SZ_ZOOMVIEW_CROSS, SZ_ZOOMVIEW/2,
		SZ_ZOOMVIEW/2+SZ_ZOOMVIEW_CROSS, SZ_ZOOMVIEW/2,
		QPen(QColor(C_ZOOMVIEW_CROSS)) );
	m_GCorssItem[1] = m_GScene->addLine(
		SZ_ZOOMVIEW/2, SZ_ZOOMVIEW/2-SZ_ZOOMVIEW_CROSS,
		SZ_ZOOMVIEW/2, SZ_ZOOMVIEW/2+SZ_ZOOMVIEW_CROSS,
		QPen(QColor(C_ZOOMVIEW_CROSS)) );
	m_GCorssItem[0]->setZValue(3);
	m_GCorssItem[1]->setZValue(3);

	QGridLayout *Layout = new QGridLayout(this);

	Layout->setMargin(0);
	Layout->setVerticalSpacing(2);
	Layout->addWidget(m_GView,0,0,1,4);
	Layout->addWidget(m_labelX,1,0,1,1);
	Layout->addWidget(m_XLoc,1,1,1,1);
	Layout->addWidget(m_labelY,1,2,1,1);
	Layout->addWidget(m_YLoc,1,3,1,1);
	Layout->setSpacing(1);
	setLayout(Layout);
	setFixedSize(sizeHint());
}

void qzoomview::setZoomSourceView(qimgviewer *view)
{
	cleanDrawed();
	if (0 != m_sourceView) {
		disconnect(m_sourceView,SIGNAL(signalViewChanged()),this,SLOT(slotViewChanged()));
		disconnect(m_sourceView,SIGNAL(signalFocusChanged(int,int)),this,SLOT(slotFocusChanged(int,int)));
		disconnect(m_sourceView,SIGNAL(signalCornerPicked(QPointF&)),this,SLOT(slotCornerPicked(QPointF&)));
		disconnect(m_sourceView,SIGNAL(signalCornerRemoved()),this,SLOT(slotCornerRemoved()));
		disconnect(m_sourceView,SIGNAL(signalDrawGridCorners()),this,SLOT(slotDrawGridCorners()));
	}
	if (0 != view) {
		connect(view,SIGNAL(signalViewChanged()),this,SLOT(slotViewChanged()));
		connect(view,SIGNAL(signalFocusChanged(int,int)),this,SLOT(slotFocusChanged(int,int)));
		connect(view,SIGNAL(signalCornerPicked(QPointF&)),this,SLOT(slotCornerPicked(QPointF&)));
		connect(view,SIGNAL(signalCornerRemoved()),this,SLOT(slotCornerRemoved()));
		connect(view,SIGNAL(signalDrawGridCorners()),this,SLOT(slotDrawGridCorners()));
	}
	m_sourceView = view;
	slotViewChanged();
}
void qzoomview::copyGridCorners()
{
	vector<QPointF> boundCorners = m_sourceView->getBoundCorners();
	vector<QPointF> pointsScaled;
	for (int i=0; i<4; ++i) {
		CornerItem cornerItem 
			= drawCorner(m_GScene,boundCorners[i]*m_scale,
						 SZ_CORNER_CROSS_R,SZ_CORNER_SQURE_R,
						 C_CORNER_BOUNDCORNER_CROSS,C_CORNER_BOUNDCORNER_SQURE,
						 1);
		m_GBoundCorners.push_back(cornerItem);

		pointsScaled.push_back(boundCorners[i]*m_scale);
	}
	drawPoly(m_GScene, pointsScaled, m_GBoundLines, C_CORNER_BOUNDLINE, -1);
	slotDrawGridCorners();
}
void qzoomview::cleanDrawed()
{
	for (int i=0; i<m_GBoundLines.size(); i++) {
		m_GScene->removeItem(m_GBoundLines.at(i));
		delete m_GBoundLines.at(i);
	}
	m_GBoundLines.clear();

	removeCorners(m_GScene, m_GGridCorners);
	removeCorners(m_GScene, m_GBoundCorners);
}
void qzoomview::updatePos()
{
	qreal pos_x = mx*m_scale + m_scale/2 - SZ_ZOOMVIEW/2;
	qreal pos_y = my*m_scale + m_scale/2 - SZ_ZOOMVIEW/2;
	m_GCorssItem[0]->setPos(pos_x,pos_y);
	m_GCorssItem[1]->setPos(pos_x,pos_y);
	m_GView->setSceneRect(pos_x,pos_y, SZ_ZOOMVIEW+1,SZ_ZOOMVIEW+1);
	m_XLoc->setText(QString("%1").arg(mx));
	m_YLoc->setText(QString("%1").arg(my));
	m_GView->viewport()->update();
}

void qzoomview::slotViewChanged()
{
	cleanDrawed();

	QImage *image = m_sourceView->getImage();
	if (0 != image) {
		m_GPixItem->setScale(1);
		m_GPixItem->setPixmap(QPixmap::fromImage(*image));
		m_GPixItem->setScale(m_scale);
		mx = image->width()/2;
		my = image->height()/2;

		if (m_sourceView->isGridCornersFound())
			copyGridCorners();

		updatePos();
	}
	else {
		m_GPixItem->setPixmap(QPixmap());
		mx = 0;
		my = 0;
		updatePos();
	}
}
void qzoomview::slotFocusChanged(int x, int y)
{
	mx = x;
	my = y;
	updatePos();
}
void qzoomview::slotCornerPicked(QPointF &corner)
{
	const int n = m_sourceView->getPickedNum();
	vector<QPointF> points = m_sourceView->getBoundCorners();
	CornerItem cornerItem 
			= drawCorner(m_GScene,corner*m_scale,
						 SZ_CORNER_CROSS_R,SZ_CORNER_SQURE_R,
						 C_CORNER_BOUNDCORNER_CROSS,C_CORNER_BOUNDCORNER_SQURE,
						 1);
	m_GBoundCorners.push_back(cornerItem);
	if (n>=2) {
		QGraphicsLineItem *line = 
			m_GScene->addLine(points[n-2].x()*m_scale,points[n-2].y()*m_scale,
								corner.x()*m_scale,corner.y()*m_scale,
								QPen(QColor(C_CORNER_BOUNDLINE)));
		line->setZValue(-1);
		m_GBoundLines.push_back(line);
	}
	if (n == 4) {
		QGraphicsLineItem *line = 
			m_GScene->addLine(points[0].x()*m_scale, points[0].y()*m_scale,
								corner.x()*m_scale, corner.y()*m_scale,
								QPen(QColor(C_CORNER_BOUNDLINE)));
		m_GBoundLines.push_back(line);
		line->setZValue(-1);
	}
	m_GView->viewport()->update();
}
void qzoomview::slotCornerRemoved()
{		
	const int n = m_sourceView->getPickedNum();

	if (n == 3){
		removeCorners(m_GScene,  m_GGridCorners);
		m_GScene->removeItem(*(m_GBoundLines.end()-1));
		delete *(m_GBoundLines.end()-1);
		m_GBoundLines.pop_back();
	}

	if (n >= 1) {
		m_GScene->removeItem(*(m_GBoundLines.end()-1));
		delete *(m_GBoundLines.end()-1);
		m_GBoundLines.pop_back();
	}

	removeCorner(m_GScene,*(m_GBoundCorners.end()-1));
	m_GBoundCorners.pop_back();

	m_GView->viewport()->update();
}
void qzoomview::slotDrawGridCorners()
{
	vector<cv::Point2f> gridCorners = m_sourceView->getGridCorners();
	removeCorners(m_GScene, m_GGridCorners);
	for (int i=0; i<gridCorners.size(); ++i) {
		QPointF point = QPointF(gridCorners.at(i).x,gridCorners.at(i).y);
		CornerItem cornerItem 
			= drawCorner(m_GScene,point*m_scale,
						 SZ_CORNER_CROSS_R,SZ_CORNER_SQURE_R,
						 C_CORNER_CROSS,C_CORNER_SQURE);
		m_GGridCorners.push_back(cornerItem);
	}
	m_GView->viewport()->update();
}
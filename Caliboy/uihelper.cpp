#include "uihelper.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>

CornerItem drawCorner(QGraphicsScene *scene, const QPointF &point,
					  const int l1, const int l2,
					  const QColor &c1, const QColor &c2,
					  const int z)
{
	CornerItem cornerItem;
	cornerItem.mLine[0] = scene->addLine(point.x()-l1, point.y(), point.x()+l1, point.y(), QPen(QColor(c1)));
	cornerItem.mLine[1] = scene->addLine(point.x(), point.y()-l1, point.x(), point.y()+l1, QPen(QColor(c1)));
	cornerItem.mSqure = scene->addRect(point.x()-l2, point.y()-l2, l2*2, l2*2, QPen(QColor(c2)));
	cornerItem.mLine[0]->setZValue(z);
	cornerItem.mLine[1]->setZValue(z);
	cornerItem.mSqure->setZValue(z);
	return cornerItem;
}
void drawPoly(QGraphicsScene *scene, const vector<QPointF> &points,
			  vector<QGraphicsLineItem*> &container,
			  const QColor &c, const int z)
{
	if (points.size()<3)
		return;
	container.clear();
	for (int i=0; i<points.size()-1; ++i) {
		QGraphicsLineItem *line = 
			scene->addLine(points[i].x(),points[i].y(),
						   points[i+1].x(),points[i+1].y(),
						   QPen(QColor(c)));
		line->setZValue(z);
		container.push_back(line);
	}
	QGraphicsLineItem *line = 
		scene->addLine(points[points.size()-1].x(),points[points.size()-1].y(),
						points[0].x(),points[0].y(),
						QPen(QColor(c)));
	line->setZValue(z);
	container.push_back(line);
}
void removeCorner(QGraphicsScene *scene, CornerItem &corner)
{
	scene->removeItem(corner.mLine[0]);
	scene->removeItem(corner.mLine[1]);
	scene->removeItem(corner.mSqure);
	delete corner.mLine[0];
	delete corner.mLine[1];
	delete corner.mSqure;
}
void removeCorners(QGraphicsScene *scene, vector<CornerItem> &corners)
{
	for (int i=0; i<corners.size(); i++)
		removeCorner(scene,corners.at(i));
	corners.clear();
}
/* uihelper.h
 * 主要用于辅助qzoomview和qimgviewer
 */
#ifndef UI_HELPER_H
#define UI_HELPER_H

#include <QPoint>
#include <QColor>
#include <vector>
using std::vector;

class QGraphicsScene;
class QGraphicsLineItem;
class QGraphicsRectItem;

/* 角点类
 */
typedef struct{
	QGraphicsLineItem *mLine[2];
	QGraphicsRectItem *mSqure;
} CornerItem;

/* 画角点[PlusSquare]
 * @param scene : 所属场景;
 * @param point : 角点位置;
 * @param l1 : 十字宽度;
 * @param l2 : 方框宽度;
 * @param c1 : 十字颜色;
 * @param c2 : 方框颜色;
 * @param z : Z值;
 * @return : 所画角点.
 */
CornerItem drawCorner(QGraphicsScene *scene, const QPointF &point,
					  const int l1, const int l2,
					  const QColor &c1, const QColor &c2,
					  const int z = 0);

/* 画矩形
 * @param scene : 所属场景;
 * @param points : 角点位置;
 * @param[out] container: 所画线条;
 * @param c : 颜色;
 * @param z : Z值;
 */
void drawPoly(QGraphicsScene *scene, const vector<QPointF> &points,
			  vector<QGraphicsLineItem*> &container,
			  const QColor &c, const int z = 0);
/* 移除单个角点
 * @param scene : 所属场景;
 * @param corner : 角点;
 */
void removeCorner(QGraphicsScene *scene, CornerItem &corner);

/* 移除角点序列
 * @param scene : 所属场景;
 * @param corner : 角点;
 */
void removeCorners(QGraphicsScene *scene, vector<CornerItem> &corners);


#endif // UI_HELPER_H

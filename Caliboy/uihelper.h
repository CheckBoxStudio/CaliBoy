/* uihelper.h
 * ��Ҫ���ڸ���qzoomview��qimgviewer
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

/* �ǵ���
 */
typedef struct{
	QGraphicsLineItem *mLine[2];
	QGraphicsRectItem *mSqure;
} CornerItem;

/* ���ǵ�[PlusSquare]
 * @param scene : ��������;
 * @param point : �ǵ�λ��;
 * @param l1 : ʮ�ֿ��;
 * @param l2 : ������;
 * @param c1 : ʮ����ɫ;
 * @param c2 : ������ɫ;
 * @param z : Zֵ;
 * @return : �����ǵ�.
 */
CornerItem drawCorner(QGraphicsScene *scene, const QPointF &point,
					  const int l1, const int l2,
					  const QColor &c1, const QColor &c2,
					  const int z = 0);

/* ������
 * @param scene : ��������;
 * @param points : �ǵ�λ��;
 * @param[out] container: ��������;
 * @param c : ��ɫ;
 * @param z : Zֵ;
 */
void drawPoly(QGraphicsScene *scene, const vector<QPointF> &points,
			  vector<QGraphicsLineItem*> &container,
			  const QColor &c, const int z = 0);
/* �Ƴ������ǵ�
 * @param scene : ��������;
 * @param corner : �ǵ�;
 */
void removeCorner(QGraphicsScene *scene, CornerItem &corner);

/* �Ƴ��ǵ�����
 * @param scene : ��������;
 * @param corner : �ǵ�;
 */
void removeCorners(QGraphicsScene *scene, vector<CornerItem> &corners);


#endif // UI_HELPER_H

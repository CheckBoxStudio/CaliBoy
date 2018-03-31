/* girdcorner.h
 * To draw corners.
 */
#ifndef GRID_CORNER
#define GRID_CORNER

#include <QPoint>
#include <QColor>
#include <vector>
using std::vector;

class QGraphicsScene;
class QGraphicsLineItem;
class QGraphicsRectItem;

/* Corner
 */
typedef struct{
    QGraphicsLineItem *mLine[2];
    QGraphicsRectItem *mSqure;
} CornerItem;

/* Draw corner with square
 * @param scene : scene where to draw;
 * @param point : location of the corner;
 * @param l1 : size of the cross;
 * @param l2 : size of the square;
 * @param c1 : color of the cross;
 * @param c2 : color of the square;
 * @param z : z value of the layer;
 * @return : handle of the corner.
 */
CornerItem drawCorner(QGraphicsScene *scene, const QPointF &point,
  const int l1, const int l2,
  const QColor &c1, const QColor &c2,
  const int z = 0);

/* draw square
 * @param scene : scene where to draw;
 * @param points : vertex of the square;
 * @param[out] container: line of the square;
 * @param c : color;
 * @param z : Z value;
 */
void drawPoly(QGraphicsScene *scene, const vector<QPointF> &points,
  vector<QGraphicsLineItem*> &container,
  const QColor &c, const int z = 0);
/* remove a single corner
 * @param scene : scene where the corner is drawn;
 * @param corner : corner to be removed;
 */
void removeCorner(QGraphicsScene *scene, CornerItem &corner);

/* remove corners
 * @param scene : scene where the corners are drawn;
 * @param corners : corners;
 */
void removeCorners(QGraphicsScene *scene, vector<CornerItem> &corners);


#endif // GRID_CORNER

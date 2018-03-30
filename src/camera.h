#ifndef CAMERA_H
#define CAMERA_H

#include <QString>
#include <QImage>
#include <cv.h>
#include <vector>

using cv::Mat;
using std::vector;
using cv::Point2f;

/*
 * A Camera,
 * i.e., the inner paremeters.
 */
class camera{
public:
    camera();
    ~camera();
    void clean();
    camera& operator=(camera &other);
public:
    double fx;   // focal in X direction.
    double fy;   // focal in Y direction.
    double cx;   // x coordinate of principal point.
    double cy;   // y coordinate of principal point.
    double s;    // inclination factor.
    double k[5]; // distortion paramaters k1,k2,p1,p2,k3.
};


/*
* A View or Image
* i.e., the extrinsic parameters.
*/
class cameraView{
public:
    cameraView();
    cameraView(QString name);
    ~cameraView();
    void clean();
    cameraView& operator=(cameraView &other);


    bool isGridCornerPicked();                          /* Whether the grid points have been picked. */
    void loadImage(QString file);                       /* Image path of this view. */
    void setGridCorners(                                /* Set gird points. */
        const int nx, const int ny,
        const vector<Point2f> &corners);

    QString getFileName() const { return m_name; }      /* Get image path. */
    QImage* getQImage() { return m_QImage; }            /* Get image for displayment. */
    Mat getMatImage() {return m_matImage; }             /* Get image for processing. */
    vector<Point2f>& getGridCorners(int &nx, int &ny);  /* Get grid points. */
    void getGridSize(int &nx, int &ny) {                /* Get grid count. */
        nx = m_nX;
        ny = m_nY;
    }
    int getGridSizeX() {return m_nX;}       /* Get grid count in X direction. */
    int getGridSizeY() {return m_nY;}       /* Get grid count in Y direction. */
    int width() {return m_matImage.cols; }  /* Get image width. */
    int height() {return m_matImage.rows; } /* Get image height. */
public:
    Mat r;              /* Rotation vector. */
    Mat R;              /* Rotation matrix. */
    Mat t;              /* Translation vector. */
private:
    QString m_name;     /* Image path. */
    QImage *m_QImage;   /* Image for displayment. */
    Mat m_matImage;     /* image for processing. */
    int m_nX;           /* Grid count in X direction. */
    int m_nY;           /* Grid count in Y direction. */
    vector<Point2f> m_gridCorners;/* Grid count. */
};


/*
* Scene,
* i.e., a camera along with multi-views.
*/
class cameraScene {
public:
    cameraScene();
    ~cameraScene();
    void clean();

    void addView(cameraView *newView);              /* Add a view by image path. */
    void addView(const QString newFile);            /* Add a view. */
    int contains(const cameraView *newView);        /* If this view exits in this scene, -1 for no. */
    int contains(const QString viewName);           /* If this view exits in this scene, -1 for no. */
    void removeView(const QString removedView);     /* Remove view by path. */
    void removeView(cameraView *removedView);       /* Remove view. */
    int viewCount() const {return m_views.size();}; /* Couont of views. */

    void getQuadSize(double &x, double &y) const {
        x = m_sizeX;
        y = m_sizeY;
    }
    void setQuadSize(const double x, const double y) {
        m_sizeX = x;
        m_sizeY = y;
    }
public:
    double m_sizeX;                 /* Physical size of grid in X directio[mm]. */
    double m_sizeY;                 /* Physical size of grid in Y directio[mm]. */
    camera m_camera;                /* Camera */
    vector<cameraView *> m_views;   /* Views */
};
#endif // !CAMERA_H
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
 * i.e., the intrinsic parameters.
 */
class camera{
public:
    camera();
    ~camera();
    void clean();
    camera& operator=(camera &other);
public:
    double fx;   // Focal length by pixel in X direction.
    double fy;   // Focal length by pixel in Y direction.
    double cx;   // X coordinate of principal point.
    double cy;   // Y coordinate of principal point.
    double s;    // Inclination factor.
    double k[5]; // Distortion parameters, i.e., k1, k2, p1, p2, k3.
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
    void loadImage(QString file);                       /* Load image view by file path. */
    void setGridCorners(                                /* Handily set gird points. */
        const int nx, const int ny,
        const vector<Point2f> &corners);

    QString getFileName() const { return m_name; }      /* Get image path. */
    QImage* getQImage() { return m_QImage; }            /* Get image for display. */
    Mat getMatImage() {return m_matImage; }             /* Get image for processing. */
    vector<Point2f>& getGridCorners(int &nx, int &ny);  /* Get grid points. */
    void getGridSize(int &nx, int &ny) {                /* Get column and raw count of grid points. */
        nx = m_nX;
        ny = m_nY;
    }
    int getGridSizeX() {return m_nX;}       /* Get column count of grid points. */
    int getGridSizeY() {return m_nY;}       /* Get raw count of grid points. */
    int width() {return m_matImage.cols; }  /* Get image width. */
    int height() {return m_matImage.rows; } /* Get image height. */
public:
    Mat r;                              /* Rotation vector. */
    Mat R;                              /* Rotation matrix. */
    Mat t;                              /* Translation vector. */
private:
    QString m_name;                     /* Image path. */
    QImage *m_QImage;                   /* Image for display. */
    Mat m_matImage;                     /* image for processing. */
    int m_nX;                           /* Column count of grid points. */
    int m_nY;                           /* Raw count of grid points. */
    vector<Point2f> m_gridCorners;      /* Grid points. */
};


/*
* Scene,
* i.e., a camera along with multi-views captured by the camera.
*/
class cameraScene {
public:
    cameraScene();
    ~cameraScene();
    void clean();

    void addView(cameraView *newView);              /* Add a view.*/
    void addView(const QString newFile);            /* Add a view by image path. */
    int contains(const cameraView *newView);        /* If this view exits in this scene, -1 for not exists. */
    int contains(const QString viewName);           /* If this view exits in this scene, -1 for not exists. */ 
    void removeView(cameraView *removedView);       /* Remove view. */
    void removeView(const QString removedView);     /* Remove view by path. */
    int viewCount() const {return m_views.size();}; /* Count of views. */

    void getQuadSize(double &x, double &y) const {
        x = m_sizeX;
        y = m_sizeY;
    }
    void setQuadSize(const double x, const double y) {
        m_sizeX = x;
        m_sizeY = y;
    }
public:
    double m_sizeX;                 /* Physical size of grid in X direction[mm]. */
    double m_sizeY;                 /* Physical size of grid in Y direction[mm]. */
    camera m_camera;                /* Camera */
    vector<cameraView *> m_views;   /* Views */
};
#endif // !CAMERA_H
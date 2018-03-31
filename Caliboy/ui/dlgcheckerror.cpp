#include "dlgcheckerror.h"

#include <QLayout>

#include "camera.h"

DlgCheckError::DlgCheckError(const cameraScene *scene,QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Calibration Error");
    setFont(QFont("Microsoft Yahei"));

    m_butOk = new QPushButton("Confirm",this);

    //
    m_TableWidget = new QTableWidget(scene->viewCount()+2, 6, this);
    m_TableWidget->horizontalHeader()->setStretchLastSection(true);
    m_TableWidget->horizontalHeader()->hide();
    m_TableWidget->verticalHeader()->hide();
    m_TableWidget->setShowGrid(true);

    QTableWidgetItem* _noitem = new QTableWidgetItem("No");
    QTableWidgetItem* _sizeitem = new QTableWidgetItem("Size");
    QTableWidgetItem* _erritemX = new QTableWidgetItem("Error_X");
    QTableWidgetItem* _erritemY = new QTableWidgetItem("Error_Y");
    QTableWidgetItem* _erritemD = new QTableWidgetItem("Error_R");
    QTableWidgetItem* _pathitem = new QTableWidgetItem("Path");

    _noitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
    _noitem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    _sizeitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
    _sizeitem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    _erritemX->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
    _erritemX->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    _erritemY->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
    _erritemY->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    _erritemD->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
    _erritemD->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    _pathitem->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
    _pathitem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    m_TableWidget->setItem(0,0,_noitem);
    m_TableWidget->setItem(0,1,_sizeitem);
    m_TableWidget->setItem(0,2,_erritemX);
    m_TableWidget->setItem(0,3,_erritemY);
    m_TableWidget->setItem(0,4,_erritemD);
    m_TableWidget->setItem(0,5,_pathitem);

    calErr(scene);

    m_TableWidget->resizeColumnsToContents();
    m_TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //
    QHBoxLayout *hLay =new QHBoxLayout();
    QVBoxLayout *vLay = new QVBoxLayout();

    hLay->addStretch();
    hLay->addWidget(m_butOk);
    vLay->addWidget(m_TableWidget);
    vLay->addLayout(hLay);
    setLayout(vLay);

    int totlen = m_TableWidget->columnWidth(0) + m_TableWidget->columnWidth(1) +
                 m_TableWidget->columnWidth(2) + m_TableWidget->columnWidth(3) +
                 m_TableWidget->columnWidth(4) + m_TableWidget->columnWidth(5) +
                 this->frameSize().width();
    setMinimumWidth(totlen);

    connect( m_butOk, SIGNAL(clicked()), this, SLOT(accept()) );
}
DlgCheckError::~DlgCheckError(void)
{
    m_TableWidget->clear();
}

QTableWidgetItem** DlgCheckError::addTableWidgetItem(const int i)
{
    QTableWidgetItem** items = new QTableWidgetItem*[6];
    items[0] = new QTableWidgetItem(QString("%1").arg(i+1));
    items[1] = new QTableWidgetItem("0x0");
    items[2] = new QTableWidgetItem(QString::fromLocal8Bit("0.000 ¡À 0.000"));
    items[3] = new QTableWidgetItem(QString::fromLocal8Bit("0.000 ¡À 0.000"));
    items[4] = new QTableWidgetItem(QString::fromLocal8Bit("0.000 ¡À 0.000"));
    items[5] = new QTableWidgetItem("--");

    for (int ii=0; ii<5; ++ii) {
        items[ii]->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        items[ii]->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
        m_TableWidget->setItem(i+1,ii,items[ii]);
    }
    items[5]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    items[5]->setFlags(~(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled));
    m_TableWidget->setItem(i+1,5,items[5]);

    return items;
}
void DlgCheckError::myMeanStdDev(
    const std::vector<cv::Point3f> &XYD,
    double &Miu_Dx, double &Miu_Dy, double &Miu_Dd,
    double &StdDev_Dx, double &StdDev_Dy, double &StdDev_Dd)
{
    double u_x = 0.0;
    double u_y = 0.0;
    double u_d = 0.0;
    double d_x = 0.0;
    double d_y = 0.0;
    double d_d = 0.0;
    const int N = XYD.size();
    for (int i=0; i<N; ++i) {
        double x = XYD.at(i).x;
        double y = XYD.at(i).y;
        double d = XYD.at(i).z;
        u_x += x;
        u_y += y;
        u_d += d;
        d_x += x*x;
        d_y += y*y;
        d_d += d*d;
    }
    u_x /= N;
    u_y /= N;
    u_d /= N;
    d_x = sqrt(d_x / N - u_x*u_x);
    d_y = sqrt(d_y / N - u_y*u_y);
    d_d = sqrt(d_d / N - u_d*u_d);

    Miu_Dx = u_x;
    Miu_Dy = u_y;
    Miu_Dd = u_d;
    StdDev_Dx = d_x;
    StdDev_Dy = d_y;
    StdDev_Dd= d_d;
}
void DlgCheckError::errStatics(
    const std::vector<cv::Point2f> &imgPts,
    const std::vector<cv::Point2f> &projPts,
    double &Miu_Dx, double &Miu_Dy, double &Miu_Dd,
    double &StdDev_Dx, double &StdDev_Dy, double &StdDev_Dd,
    std::vector<cv::Point3f> &errContainer)
{
    assert(imgPts.size() == projPts.size());
    std::vector<cv::Point3f> errs;
    for (int i=0; i<imgPts.size(); ++i) {
        float dx = abs(imgPts.at(i).x - projPts.at(i).x);
        float dy = abs(imgPts.at(i).y - projPts.at(i).y);
        float dd = sqrt(dx*dx+dy*dy);
        errs.push_back(cv::Point3f(dx,dy,dd));
    }
    errContainer.swap(errs);
    myMeanStdDev(errContainer,Miu_Dx,Miu_Dy,Miu_Dd,StdDev_Dx,StdDev_Dy,StdDev_Dd);
}
void DlgCheckError::calErr(const cameraScene *scene)
{
    cv::Mat K = cv::Mat::eye(3,3,CV_64F);
    K.at<double>(0) = scene->m_camera.fx;
    K.at<double>(4) = scene->m_camera.fy;
    K.at<double>(2) = scene->m_camera.cx;
    K.at<double>(5) = scene->m_camera.cy;
    cv::Mat distK = cv::Mat(5,1,CV_64F);
    for (int i = 0; i < 5; ++i)
        distK.at<double>(i) = scene->m_camera.k[i];

    double sizeX, sizeY;
    scene->getQuadSize(sizeX,sizeY);

    //---------------------------------------

    std::vector<cv::Point3f> totalErr;
    double ux, uy, ud, dx, dy, dd;

    for (int i=0; i<scene->viewCount(); ++i) {
        cameraView *view = scene->m_views.at(i);
        QTableWidgetItem **items = addTableWidgetItem(i);

        if (view->isGridCornerPicked()) {
            int nx, ny;
            std::vector<cv::Point2f> imgPts = view->getGridCorners(nx,ny);
            std::vector<cv::Point3f> worldPts;
            std::vector<cv::Point2f> projPts;
            for (int j=0; j<ny; ++j) {
                for (int i=0; i<nx; ++i) {
                    cv::Point3f p(i*sizeX,j*sizeY,0);
                    worldPts.push_back(p);
                }
            }
            cv::projectPoints(worldPts,
              view->r,view->t,K,distK,
              projPts);
            errStatics(imgPts,projPts,
               ux,uy,ud,dx,dy,dd,
               totalErr);
            imgPts.clear();
            worldPts.clear();
            projPts.clear();

            items[1]->setText(QString("%1*%2").arg(nx).arg(ny));
            items[2]->setText(QString::fromLocal8Bit("%1 ¡À %2").arg(ux,5,'g',3,'0').arg(dx,5,'g',3,'0'));
            items[3]->setText(QString::fromLocal8Bit("%1 ¡À %2").arg(uy,5,'g',3,'0').arg(dy,5,'g',3,'0'));
            items[4]->setText(QString::fromLocal8Bit("%1 ¡À %2").arg(ud,5,'g',3,'0').arg(dd,5,'g',3,'0'));
        }
        items[5]->setText(view->getFileName());
        delete[] items;
    }
    int n = scene->viewCount();
    QTableWidgetItem **items = addTableWidgetItem(n);
    myMeanStdDev(totalErr,ux,uy,ud,dx,dy,dd);
    items[0]->setText("Total");
    items[1]->setText(QString("%1").arg(totalErr.size()));
    items[2]->setText(QString::fromLocal8Bit("%1 ¡À %2").arg(ux,5,'g',3,'0').arg(dx,5,'g',3,'0'));
    items[3]->setText(QString::fromLocal8Bit("%1 ¡À %2").arg(uy,5,'g',3,'0').arg(dy,5,'g',3,'0'));
    items[4]->setText(QString::fromLocal8Bit("%1 ¡À %2").arg(ud,5,'g',3,'0').arg(dd,5,'g',3,'0'));
    totalErr.clear();
    delete[] items;
}

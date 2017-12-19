#include "threadCalib.h"
#include <QTime>
#include "qtChinese.h"
#include "qmsg.h"

#include "Calib\camera.h"
#include "Calib\estimate.h"


QThreadCalib::QThreadCalib()
	: m_calibScene(0)
{
}
QThreadCalib::~QThreadCalib()
{
	m_calibList.clear();
}

void QThreadCalib::setCalibParams(
	bool estPrincipalPoint, bool estFocalRatio,
	unsigned int estDistMode)
{
	QMutexLocker locker(&mutex);
	m_estPrincipalPoint = estPrincipalPoint;
	m_estFocalRatio		= estFocalRatio;
	m_estDistMode	    = estDistMode;
	if (m_estDistMode>5)
		m_estDistMode = 5;
}
void QThreadCalib::calib(cameraScene *scene, vector<int> *list)
{
	QMutexLocker locker(&mutex);
	m_calibScene = scene;
	m_calibList.clear();
	for (int i=0; i<list->size(); ++i)
		m_calibList.push_back(list->at(i));

	todoCode = ToDo_Calib;
	msgOut(FromCHN("\n相机标定..."));
	start();
}


//------------------------------------------------------------------
void QThreadCalib::run()
{
	QMutexLocker locker(&mutex);

	if (!checkInput()) {
		msgOut(FromCHN("输入不全!"));
		emit signalThreadEnd();
		return;
	}

	QTime time;
	time.start();
	_msgOut("...");
	//=====================
	if (todoCode == ToDo_Calib) {
		
		double projerr = doCalib_CVZhang();
		_msgOut(FromCHN("平均误差:%1.").arg(projerr));
	}
	//=====================
	_msgOut(QString("Elapsed %1 s.").arg(time.elapsed()/1000.0));
	emit signalThreadEnd();
}
//
bool QThreadCalib::checkInput()
{
	bool bOk = true;
	if (m_calibScene == 0)
		bOk = false;
	if (m_calibList.size() == 0)
		bOk = false;
	return bOk;
}
int QThreadCalib::calibFlag()
{
	int flag = 0;

	if (!m_estPrincipalPoint)
		flag |= CV_CALIB_FIX_PRINCIPAL_POINT;
	if (!m_estFocalRatio)
		flag |= CV_CALIB_FIX_ASPECT_RATIO;
	switch (m_estDistMode)
	{
	case 4:
		flag |= CV_CALIB_ZERO_TANGENT_DIST;
		break;	
	case 0:
		flag |= CV_CALIB_FIX_K1;
	case 1:
		flag |= CV_CALIB_FIX_K2;
	case 2:
		flag |= CV_CALIB_ZERO_TANGENT_DIST;
	case 3:
		flag |= CV_CALIB_FIX_K3;
		break;
	default:
		break;
	}

	return flag;
}
//
/* 【张氏标定】
 *  - 基于OpenCV实现,无法标定畸变因子!
 * ----------------------------------
 * 1. 内参估计
 *  - 畸变因子为0;
 *  - 主点位置设置为图像中心;
 *    note: 由于畸变因子与主点位置已设定,内参矩阵仅有fx和fy两个未知数,
 *			一幅图像既能求得一个内存矩阵;
 *  - 利用所有图像的单应矩阵(图像坐标中心移至主点)求取最小二乘内参矩阵;
 *
 * 2. 外参估计
 *  - R矩阵由Rodrigues获得以保证正交;
 *  - 最终[R|t]由最优化计算得到(LevMarq);
 *
 * 3. 反投影优化(LevMarq)K[R_i|t_i];
 * ----------------------------------
 * http://research.microsoft.com/en-us/um/people/zhang/Papers/TR98-71.pdf
 * http://www.opencv.org.cn/opencvdoc/2.3.2/html/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html?
 */
double QThreadCalib::doCalib_CVZhang()
{ 
	double sx, sy;
	m_calibScene->getQuadSize(sx,sy); 
    vector<vector<cv::Point2f>> cornersVect;  
    vector<vector<cv::Point3f>> worldPointsVect;
	for (int i=0; i<m_calibList.size(); ++i)
	{
		cameraView *view = m_calibScene->m_views.at(m_calibList.at(i));
		int nx, ny;
		vector<cv::Point2f> corners = view->getGridCorners(nx,ny);
		cornersVect.push_back(corners);

		_msgOut( QString("%1(%2x%3)...").arg(m_calibList.at(i)+1).arg(nx).arg(ny) );

		vector<cv::Point3f> worldPoints;
		for (int row=0;row<ny;++row)
			for (int col=0;col<nx;++col)
				worldPoints.push_back(cv::Point3f(col*sx,row*sy,0));
		worldPointsVect.push_back(worldPoints);
	}


    cv::Mat cameraMatirx = cv::Mat::eye(3,3,CV_64F);
	cv::Mat distCoeffs = cv::Mat::zeros(1,5,CV_64F);
    std::vector<cv::Mat> rvecs,tvecs;
	cv::Mat img = m_calibScene->m_views.at(m_calibList.at(0))->getMatImage();
	
	int flag = calibFlag();
	double err = cv::calibrateCamera(worldPointsVect,cornersVect,
									 cv::Size(img.cols,img.rows),
									 cameraMatirx,distCoeffs,
									 rvecs,tvecs,
									 flag);

	for (int i=0; i<m_calibList.size(); ++i){
		cornersVect.at(i).clear();
		worldPointsVect.at(i).clear();
	}
	cornersVect.clear();
	worldPointsVect.clear();

	m_calibScene->m_camera.fx = cameraMatirx.at<double>(0);
	m_calibScene->m_camera.fy = cameraMatirx.at<double>(4);
	m_calibScene->m_camera.cx = cameraMatirx.at<double>(2);
	m_calibScene->m_camera.cy = cameraMatirx.at<double>(5);
	for (int i=0; i<5; ++i)
		m_calibScene->m_camera.k[i] = distCoeffs.at<double>(i);
	for (int i=0; i<m_calibList.size(); ++i)
	{
		cameraView *view = m_calibScene->m_views.at(m_calibList.at(i));
		cv::Rodrigues(rvecs.at(i),view->R);
		view->r = rvecs.at(i);
		view->t = tvecs.at(i);
	}
	return err;
}
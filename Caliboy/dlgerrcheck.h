/* dlgerrcheck.h
 * 标定误差统计.
 */

#ifndef DLG_ERROR_CHECK_H
#define DLG_ERROR_CHECK_H

#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <vector>
#include <cv.h>

class cameraScene;

class ErrorCheckDialog : public QDialog
{
	Q_OBJECT
private:
	QTableWidget* m_tw; 
	QPushButton* m_butok;

public:
	ErrorCheckDialog(cameraScene *scene,QWidget *parent = 0);
	~ErrorCheckDialog();

private:
	
	QTableWidgetItem** addTableWidgetItem(const int i);
	void myMeanStdDev(const std::vector<cv::Point3f> &XYD,
					  double &Miu_Dx, double &Miu_Dy, double &Miu_Dd,
					  double &StdDev_Dx, double &StdDev_Dy, double &StdDev_Dd);
	void errStatics(const std::vector<cv::Point2f> &imgPts,
					const std::vector<cv::Point2f> &projPts,
					double &Miu_Dx, double &Miu_Dy, double &Miu_Dd,
					double &StdDev_Dx, double &StdDev_Dy, double &StdDev_Dd,
					std::vector<cv::Point3f> &errContainer);
	void errCal(cameraScene *scene);
};

#endif// !DLG_ERROR_CHECK_H

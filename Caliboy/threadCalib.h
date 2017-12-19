/* threadCalib.h
 * �궨�����߳�.
 */
#ifndef QT_THREAD_CALIB_H
#define QT_THREAD_CALIB_H

#include <QThread>
#include <QMutex>
#include <vector>

using std::vector;

enum TodoCode{
	ToDo_Calib
};

class cameraScene;

class QThreadCalib : public QThread
{
	Q_OBJECT

public:
	QThreadCalib();
	~QThreadCalib();

	void setCalibParams(bool estPrincipalPoint, bool estFocalRatio, unsigned int estDistMode);
	void calib(cameraScene *scene, vector<int> *list);
protected:
	void run();	
private:
	QMutex mutex;
	TodoCode todoCode;

	cameraScene *m_calibScene;
	vector<int> m_calibList;
	bool m_estPrincipalPoint;		/* �궨����-�̶�����λ��[ͼ������] */
	bool m_estFocalRatio;			/* �궨����-�̶�fx/fy[1] */
	unsigned int m_estDistMode;		/* �궨����-�����������
									 * [0]: ��
									 * [1]: k1
									 * [2]: k1,k2
									 * [3]: k1,k2,k3
									 * [4]: k1,k2,p1,p2
									 * [5]: k1,k2,p2,p2,k3
									 */
private:
	bool checkInput();
	int calibFlag();
	double doCalib_CVZhang();
signals:
	void signalThreadEnd();
};

#endif// !QT_THREAD_CALIB_H

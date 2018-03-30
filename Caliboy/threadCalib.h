/* threadCalib.h
 * Processing thread for calibration.
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
    bool m_estPrincipalPoint;       /* Whether to estmate the principal point. */
    bool m_estFocalRatio;           /* Whether to estmate the focal ratio.
                                       If not, the focal ratio will be set 1. */
    unsigned int m_estDistMode;     /* Distoration model:
                                     * [0]: None
                                     * [1]: Only k1
                                     * [2]: Only k1,k2
                                     * [3]: Only k1,k2,k3
                                     * [4]: Only k1,k2,p1,p2
                                     * [5]: All k1,k2,p2,p2,k3
                                     */
private:
    bool checkInput();
    int calibFlag();
    double doCalib_CVZhang();
signals:
    void signalThreadEnd();
};

#endif// !QT_THREAD_CALIB_H

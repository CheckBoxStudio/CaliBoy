/* dlgsettingcalib.h
 * Calibratin setting dailog.
 */

#ifndef DLG_SETTING_CALIB_H
#define DLG_SETTING_CALIB_H

#include <QDialog>

class QPushButton;
class QComboBox;

class DlgSettingCalib : public QDialog
{
Q_OBJECT
private:
    bool m_estPrincipalPoint;        /* Whether to estimate the principal point. */
    bool m_estFocalRatio;            /* Whether to estimate the focal ratio.
                                        If not, the focal ratio will be set 1. */
    unsigned int m_estDistMode;      /* Distortion model:
                                      * [0]: None
                                      * [1]: Only k1
                                      * [2]: Only k1,k2
                                      * [3]: Only k1,k2,k3
                                      * [4]: Only k1,k2,p1,p2
                                      * [5]: All k1,k2,p2,p2,k3
                                      */
public:
    DlgSettingCalib(QWidget *parent = 0);
    DlgSettingCalib(
        bool estPrincipalPoint, bool estFocalRatio,
        unsigned int estDistMode,
        QWidget *parent = 0);
    ~DlgSettingCalib();
    void setParams(
        bool estPrincipalPoint, bool estFocalRatio,
        unsigned int estDistMode);
    void getParams(
        bool &estPrincipalPoint, bool &estFocalRatio,
        unsigned int &estDistMode);
private:
    QComboBox *m_combo_estPrincipalPoint;
    QComboBox *m_combo_estFocalRatio;
    QComboBox *m_combo_estDistMode;

    QPushButton *m_but_ok;
    QPushButton *m_but_cancel;
private:
    void setupme();
    void updateComboes();
private slots:
    void apply();
};

#endif// !DLG_SETTING_CALIB_H

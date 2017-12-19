/* dlgsettingcalib.h
 * �궨���öԻ���.
 */

#ifndef DLG_SETTING_CALIB_H
#define DLG_SETTING_CALIB_H

#include <QDialog>

class QPushButton;
class QComboBox;

class CalibSettingDialog : public QDialog
{
	Q_OBJECT
private:
	bool m_estPrincipalPoint;							/* �궨����-�̶�����λ��[ͼ������] */
	bool m_estFocalRatio;								/* �궨����-�̶�fx/fy[1] */
	unsigned int m_estDistMode;							/* �궨����-�����������
														 * [0]: ��
														 * [1]: k1
														 * [2]: k1,k2
														 * [3]: k1,k2,k3
														 * [4]: k1,k2,p1,p2
														 * [5]: k1,k2,p2,p2,k3
														 */
public:
	CalibSettingDialog(QWidget *parent = 0);
	CalibSettingDialog(bool estPrincipalPoint, bool estFocalRatio,
					   unsigned int estDistMode,
					   QWidget *parent = 0);
	~CalibSettingDialog();
	void setParams(bool estPrincipalPoint, bool estFocalRatio,
				   unsigned int estDistMode);
	void getParams(bool &estPrincipalPoint, bool &estFocalRatio,
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

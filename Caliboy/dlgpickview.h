/* dlgpickview.h
 * 选择参与标定的图像.
 */

#ifndef DLG_VIEWPICK_H
#define DLG_VIEWPICK_H

#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <vector>

using std::vector;
class cameraScene;

class ViewPickDialog : public QDialog
{
	Q_OBJECT
private:
	vector<int> m_PickList;
	QTableWidget* m_tw; 
	QLineEdit* m_sizex;
	QLineEdit* m_sizey;
	QPushButton* m_butclose;
	QPushButton* m_butok;

public:
	ViewPickDialog(cameraScene *scene,QWidget *parent = 0);
	~ViewPickDialog();
	vector<int> getPickList() {return m_PickList;}
	void getSize(float &x, float &y) {
		x = getSizeX();
		y = getSizeY();
	}
	float getSizeX() {return m_sizey->text().toFloat();}
	float getSizeY() {return m_sizey->text().toFloat();}
private:
	void checkAll(bool bOn);
private slots:
	void checkAllChanged();
	void checkChanged();

	void apply();
};

#endif// !DLG_VIEWPICK_H

/* dlgsettingIO.h
 * Import/export setting dialog.
 */

#ifndef DLG_SETTING_IO_H
#define DLG_SETTING_IO_H

#include <QDialog>

class QGroupBox;
class QLineEdit;
class QPushButton;
class QCheckBox;

class DlgSettingIO : public QDialog
{
Q_OBJECT

public:
    DlgSettingIO(QWidget *parent = 0);
    ~DlgSettingIO();
    void setParams(
        const QString cameraFile, const QString calibFile,
        const bool bGridPts, const bool bRt);
    void getParams(
        QString &cameraFile, QString &calibFile,
        bool &bGridPts, bool &bRt);

    int show_IOSetting();
    int show_Import();
    int show_Export();
private:

    QGroupBox *m_groupbox_camera;
    QLineEdit *m_text_cameraFile;
    QPushButton *m_but_browseCameraFile;

    QGroupBox *m_groupbox_calib;
    QLineEdit *m_text_calibFile;
    QPushButton *m_but_browseCalibFile;
    QCheckBox *m_checkbox_calib_gridPts;
    QCheckBox *m_checkbox_calib_Rt;

    QPushButton *m_but_ok;
    QPushButton *m_but_cancel;

    enum state{
        _setting,
        _export,
        _import
    };
    state m_state;
private:
    void setupme();
private slots:
    void openImportPath();
};

#endif// !DLG_SETTING_CALIB_H

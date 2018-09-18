#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QLabel>

#include "configuration.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

protected:
    QLabel *p_msg_status_lbl, *p_logging_status_lbl;
    QRadioButton *p_current_msg_btn, *p_current_logging_btn;
    qlogger::Configuration::Level m_log_level = qlogger::Configuration::q4DEBUG, m_msg_level = qlogger::Configuration::q4DEBUG;

    void setMessageLogLevel(qlogger::Configuration::Level);
    void setLoggingLogLevel(qlogger::Configuration::Level);
    void msgLevelBtnsClicked();
    void loggingLevelBtnsClicked();
    void sendBtnClicked();
    void sendMessage(QString msg);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

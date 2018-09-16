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
    enum Level {
        FATAL, ERROR, WARN, INFO, DEBUG, TRACE,
    };
    QLabel *p_display_status_lbl, *p_logging_status_lbl;
    QRadioButton *p_current_btn;
    Level m_level = DEBUG, m_log_level = DEBUG;

    void setDisplayLogLevel(Level);
    void setLoggingLogLevel(Level);
    void levelBtnsClicked();
    void sendBtnClicked();
    void sendMessage(QString msg);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

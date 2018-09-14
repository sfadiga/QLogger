#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>

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
    void levelBtnsClicked();
    void sendBtnClicked();
    void sendMessage(QString msg);

private:
    Ui::MainWindow *ui;
    QRadioButton *p_current_btn;
    Level m_level = DEBUG;
};

#endif // MAINWINDOW_H

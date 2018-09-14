#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qlogger.h"

using namespace qlogger;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    p_current_btn = ui->pDebugBtn;

    // QLogger configurations
    //    QLogger::addLogger("sigs", Configuration::q0FATAL, Configuration::SIGNAL);
    //    QLogger::addLogger("sigs", Configuration::q1ERROR, Configuration::SIGNAL);
    QLogger::addLogger("fatal", Configuration::q2WARN, Configuration::SIGNAL);
    //    QLogger::addLogger("sigs", Configuration::q3INFO, Configuration::SIGNAL);
    //    QLogger::addLogger("sigs", Configuration::q4DEBUG, Configuration::SIGNAL);
    //    QLogger::addLogger("sigs", Configuration::q5TRACE, Configuration::SIGNAL);

    // QLogger signals => QWidget links
    connect(QLogger::getInstance(), &QLogger::logMessage,
            ui->pFatalErrors, &QPlainTextEdit::appendPlainText);

    // ui setup.
    connect(ui->pFatalBtn, &QRadioButton::clicked, this, &MainWindow::levelBtnsClicked);
    connect(ui->pErrorBtn, &QRadioButton::clicked, this, &MainWindow::levelBtnsClicked);
    connect(ui->pDebugBtn, &QRadioButton::clicked, this, &MainWindow::levelBtnsClicked);
    connect(ui->pInfoBtn, &QRadioButton::clicked, this, &MainWindow::levelBtnsClicked);
    connect(ui->pWarnBtn, &QRadioButton::clicked, this, &MainWindow::levelBtnsClicked);
    connect(ui->pTraceBtn, &QRadioButton::clicked, this, &MainWindow::levelBtnsClicked);
    connect(ui->pSendBtn, &QPushButton::clicked, this, &MainWindow::sendBtnClicked);

    //    sendMessage("Inital message - should be debug.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::levelBtnsClicked()
{
    QObject *src = sender();

    if (src == p_current_btn) {
        return;

    } else if (src == ui->pFatalBtn) {
        m_level = FATAL;
        p_current_btn = ui->pFatalBtn;
        //        sendMessage("Level changed to FATAL");

    } else if (src == ui->pErrorBtn) {
        m_level = ERROR;
        p_current_btn = ui->pErrorBtn;
        //        sendMessage("Level changed to ERROR");

    } else if (src == ui->pDebugBtn) {
        m_level = DEBUG;
        p_current_btn = ui->pDebugBtn;
        //        sendMessage("Level changed to DEBUG");

    } else if (src == ui->pInfoBtn) {
        m_level = INFO;
        p_current_btn = ui->pInfoBtn;
        //        sendMessage("Level changed to INFO");

    } else if (src == ui->pWarnBtn) {
        m_level = WARN;
        p_current_btn = ui->pWarnBtn;
        //        sendMessage("Level changed to WARN");

    } else if (src == ui->pTraceBtn) {
        m_level = TRACE;
        p_current_btn = ui->pTraceBtn;
        //        sendMessage("Level changed to TRACE");
    }
}

void MainWindow::sendBtnClicked()
{
    sendMessage("The send button was clicked at set level.");
}

void MainWindow::sendMessage(QString msg)
{
    switch (m_level) {
    case FATAL:
        QLOG_FATAL(msg, "sigs");
        break;

    case ERROR:
        QLOG_ERROR(msg, "sigs");
        break;

    case WARN:
        QLOG_WARN(msg, "sigs");
        break;

    case DEBUG:
        QLOG_DEBUG(msg, "sigs");
        break;

    case INFO:
        QLOG_INFO(msg, "sigs");
        break;

    case TRACE:
        QLOG_TRACE(msg, "sigs");
        break;
    }
}


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qlogger.h"

using namespace qlogger;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    p_current_msg_btn = ui->pDebugBtn;
    p_current_logging_btn = ui->pLogDebugBtn;

    // QLogger configurations
    QLogger::addLogger("sigs", Configuration::q4DEBUG, Configuration::SIGNAL);

    // QLogger signals => QWidget links
    connect(QLogger::getInstance(), &QLogger::logMessage,
            ui->pErrorsDisplay, &QPlainTextEdit::appendPlainText);


    QString status_style = "QLabel {"
                           "border: 2px solid black;"
                           "background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #eef, stop: 1 #ccf);"
                           "}";

    p_msg_status_lbl = new QLabel(this);
    p_msg_status_lbl->setStyleSheet(status_style);
    ui->pStatusBar->addPermanentWidget(p_msg_status_lbl, 0);
    setMessageLogLevel(Configuration::q4DEBUG);

    p_logging_status_lbl = new QLabel(this);
    p_logging_status_lbl->setStyleSheet(status_style);
    ui->pStatusBar->addPermanentWidget(p_logging_status_lbl, 1);
    setLoggingLogLevel(Configuration::q4DEBUG);

    QLabel *filler = new QLabel();
    ui->pStatusBar->addPermanentWidget(filler, 2);


    // ui setup.
    connect(ui->pFatalBtn, &QRadioButton::clicked, this, &MainWindow::msgLevelBtnsClicked);
    connect(ui->pErrorBtn, &QRadioButton::clicked, this, &MainWindow::msgLevelBtnsClicked);
    connect(ui->pDebugBtn, &QRadioButton::clicked, this, &MainWindow::msgLevelBtnsClicked);
    connect(ui->pInfoBtn, &QRadioButton::clicked, this, &MainWindow::msgLevelBtnsClicked);
    connect(ui->pWarnBtn, &QRadioButton::clicked, this, &MainWindow::msgLevelBtnsClicked);
    connect(ui->pTraceBtn, &QRadioButton::clicked, this, &MainWindow::msgLevelBtnsClicked);

    connect(ui->pLogFatalBtn, &QRadioButton::clicked, this, &MainWindow::loggingLevelBtnsClicked);
    connect(ui->pLogErrorBtn, &QRadioButton::clicked, this, &MainWindow::loggingLevelBtnsClicked);
    connect(ui->pLogDebugBtn, &QRadioButton::clicked, this, &MainWindow::loggingLevelBtnsClicked);
    connect(ui->pLogInfoBtn, &QRadioButton::clicked, this, &MainWindow::loggingLevelBtnsClicked);
    connect(ui->pLogWarnBtn, &QRadioButton::clicked, this, &MainWindow::loggingLevelBtnsClicked);
    connect(ui->pLogTraceBtn, &QRadioButton::clicked, this, &MainWindow::loggingLevelBtnsClicked);

    connect(ui->pSendBtn, &QPushButton::clicked, this, &MainWindow::sendBtnClicked);

    //    sendMessage("Inital message - should be debug.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMessageLogLevel(Configuration::Level level)
{
    if (level == Configuration::q0FATAL) {
        m_msg_level = Configuration::q0FATAL;
        ui->pFatalBtn->setChecked(true);
        p_msg_status_lbl->setText("Message level : FATAL");

    } else if (level == Configuration::q1ERROR) {
        m_msg_level = Configuration::q1ERROR;
        ui->pErrorBtn->setChecked(true);
        p_msg_status_lbl->setText("Message level : ERROR");

    } else if (level == Configuration::q2WARN) {
        m_msg_level = Configuration::q2WARN;
        ui->pWarnBtn->setChecked(true);
        p_msg_status_lbl->setText("Message level : WARN");

    } else if (level == Configuration::q3INFO) {
        m_msg_level = Configuration::q3INFO;
        ui->pInfoBtn->setChecked(true);
        p_msg_status_lbl->setText("Message level : INFO");

    } else if (level == Configuration::q4DEBUG) {
        m_msg_level = Configuration::q4DEBUG;
        ui->pDebugBtn->setChecked(true);
        p_msg_status_lbl->setText("Message level : DEBUG");

    } else if (level == Configuration::q5TRACE) {
        m_msg_level = Configuration::q5TRACE;
        ui->pTraceBtn->setChecked(true);
        p_msg_status_lbl->setText("Message level : TRACE");
    }
}

void MainWindow::setLoggingLogLevel(Configuration::Level level)
{
    if (level == Configuration::q0FATAL) {
        m_log_level = Configuration::q0FATAL;
        ui->pLogFatalBtn->setChecked(true);
        p_logging_status_lbl->setText("Log level : FATAL");
        QLogger::changelogLevel("sigs", Configuration::q0FATAL, Configuration::SIGNAL);

    } else if (level == Configuration::q1ERROR) {
        m_log_level = Configuration::q1ERROR;
        ui->pLogErrorBtn->setChecked(true);
        p_logging_status_lbl->setText("Log level : ERROR");
        QLogger::changelogLevel("sigs", Configuration::q1ERROR, Configuration::SIGNAL);

    } else if (level == Configuration::q2WARN) {
        m_log_level = Configuration::q2WARN;
        ui->pLogWarnBtn->setChecked(true);
        p_logging_status_lbl->setText("Log level : WARN");
        QLogger::changelogLevel("sigs", Configuration::q2WARN, Configuration::SIGNAL);

    } else if (level == Configuration::q3INFO) {
        m_log_level = Configuration::q3INFO;
        ui->pLogInfoBtn->setChecked(true);
        p_logging_status_lbl->setText("Log level : INFO");
        QLogger::changelogLevel("sigs", Configuration::q3INFO, Configuration::SIGNAL);

    } else if (level == Configuration::q4DEBUG) {
        m_log_level = Configuration::q4DEBUG;
        ui->pLogDebugBtn->setChecked(true);
        p_logging_status_lbl->setText("Log level : DEBUG");
        QLogger::changelogLevel("sigs", Configuration::q4DEBUG, Configuration::SIGNAL);

    } else if (level == Configuration::q5TRACE) {
        m_log_level = Configuration::q5TRACE;
        ui->pLogTraceBtn->setChecked(true);
        p_logging_status_lbl->setText("Log level : TRACE");
        QLogger::changelogLevel("sigs", Configuration::q5TRACE, Configuration::SIGNAL);
    }
}

void MainWindow::msgLevelBtnsClicked()
{
    QObject *src = sender();

    if (src == p_current_logging_btn) {
        return;

    } else if (src == ui->pFatalBtn) {
        setMessageLogLevel(Configuration::q0FATAL);
        p_current_msg_btn = ui->pFatalBtn;

    } else if (src == ui->pErrorBtn) {
        setMessageLogLevel(Configuration::q1ERROR);
        p_current_msg_btn = ui->pErrorBtn;

    } else if (src == ui->pWarnBtn) {
        setMessageLogLevel(Configuration::q2WARN);
        p_current_msg_btn = ui->pWarnBtn;

    } else if (src == ui->pInfoBtn) {
        setMessageLogLevel(Configuration::q3INFO);
        p_current_msg_btn = ui->pInfoBtn;

    } else if (src == ui->pDebugBtn) {
        setMessageLogLevel(Configuration::q4DEBUG);
        p_current_msg_btn = ui->pDebugBtn;

    } else if (src == ui->pTraceBtn) {
        setMessageLogLevel(Configuration::q5TRACE);
        p_current_msg_btn = ui->pTraceBtn;
    }
}

void MainWindow::loggingLevelBtnsClicked()
{
    QObject *src = sender();

    if (src == p_current_logging_btn) {
        return;

    } else if (src == ui->pLogFatalBtn) {
        setLoggingLogLevel(Configuration::q0FATAL);
        p_current_logging_btn = ui->pLogFatalBtn;

    } else if (src == ui->pLogErrorBtn) {
        setLoggingLogLevel(Configuration::q1ERROR);
        p_current_logging_btn = ui->pLogErrorBtn;

    } else if (src == ui->pLogWarnBtn) {
        setLoggingLogLevel(Configuration::q2WARN);
        p_current_logging_btn = ui->pLogWarnBtn;

    } else if (src == ui->pLogInfoBtn) {
        setLoggingLogLevel(Configuration::q3INFO);
        p_current_logging_btn = ui->pLogInfoBtn;

    } else if (src == ui->pLogDebugBtn) {
        setLoggingLogLevel(Configuration::q4DEBUG);
        p_current_logging_btn = ui->pLogDebugBtn;

    } else if (src == ui->pLogTraceBtn) {
        setLoggingLogLevel(Configuration::q5TRACE);
        p_current_logging_btn = ui->pLogTraceBtn;
    }

}

void MainWindow::sendBtnClicked()
{
    sendMessage("The send button was clicked at set level.");
}

void MainWindow::sendMessage(QString msg)
{
    switch (m_msg_level) {
    case Configuration::q0FATAL:
        QLOG_FATAL(msg, "sigs");
        break;

    case Configuration::q1ERROR:
        QLOG_ERROR(msg, "sigs");
        break;

    case Configuration::q2WARN:
        QLOG_WARN(msg, "sigs");
        break;

    case Configuration::q3INFO:
        QLOG_INFO(msg, "sigs");
        break;

    case Configuration::q4DEBUG:
        QLOG_DEBUG(msg, "sigs");
        break;

    case Configuration::q5TRACE:
        QLOG_TRACE(msg, "sigs");
        break;
    }
}


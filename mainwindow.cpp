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

    p_display_status_lbl = new QLabel(this);
    ui->pStatusBar->addPermanentWidget(p_display_status_lbl, 0);
    p_logging_status_lbl = new QLabel(this);
    ui->pStatusBar->addPermanentWidget(p_logging_status_lbl, 1);

    // QLogger configurations
    //    QLogger::addLogger("sigs", Configuration::q0FATAL, Configuration::SIGNAL);
    //    QLogger::addLogger("sigs", Configuration::q1ERROR, Configuration::SIGNAL);
    //    QLogger::addLogger("fatal", Configuration::q2WARN, Configuration::SIGNAL);
    //    QLogger::addLogger("sigs", Configuration::q3INFO, Configuration::SIGNAL);
    QLogger::addLogger("sigs", Configuration::q4DEBUG, Configuration::SIGNAL);
    //    QLogger::addLogger("sigs", Configuration::q5TRACE, Configuration::SIGNAL);

    // QLogger signals => QWidget links
    connect(QLogger::getInstance(), &QLogger::logMessage,
            ui->pErrorsDisplay, &QPlainTextEdit::appendPlainText);

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

void MainWindow::setDisplayLogLevel(MainWindow::Level level)
{
    if (level == FATAL) {
        m_log_level = FATAL;
        p_display_status_lbl->setText("Log level : FATAL");

    } else if (level == ERROR) {
        m_log_level = ERROR;
        p_display_status_lbl->setText("Log level : ERROR");

    } else if (level == DEBUG) {
        m_log_level = DEBUG;
        p_display_status_lbl->setText("Log level : DEBUG");

    } else if (level == INFO) {
        m_log_level = INFO;
        p_display_status_lbl->setText("Log level : INFO");

    } else if (level == WARN) {
        m_log_level = WARN;
        p_display_status_lbl->setText("Log level : WARN");

    } else if (level == TRACE) {
        m_log_level = TRACE;
        p_display_status_lbl->setText("Log level : TRACE");
    }
}

void MainWindow::setLoggingLogLevel(MainWindow::Level level)
{
    if (level == FATAL) {
        m_log_level = FATAL;
        p_logging_status_lbl->setText("Log level : FATAL");

    } else if (level == ERROR) {
        m_log_level = ERROR;
        p_logging_status_lbl->setText("Log level : ERROR");

    } else if (level == DEBUG) {
        m_log_level = DEBUG;
        p_logging_status_lbl->setText("Log level : DEBUG");

    } else if (src == ui->pInfoBtn) {
        m_log_level = INFO;
        p_logging_status_lbl->setText("Log level : INFO");

    } else if (src == ui->pWarnBtn) {
        m_log_level = WARN;
        p_logging_status_lbl->setText("Log level : WARN");

    } else if (src == ui->pTraceBtn) {
        m_log_level = TRACE;
        p_logging_status_lbl->setText("Log level : TRACE");
    }
}

void MainWindow::levelBtnsClicked()
{
    QObject *src = sender();

    if (src == p_current_btn) {
        return;

    } else if (src == ui->pFatalBtn) {
        m_level = FATAL;
        p_current_btn = ui->pFatalBtn;
        p_display_status_lbl->setText("Log level : FATAL");

    } else if (src == ui->pErrorBtn) {
        m_level = ERROR;
        p_current_btn = ui->pErrorBtn;
        p_display_status_lbl->setText("Log level : ERROR");

    } else if (src == ui->pDebugBtn) {
        m_level = DEBUG;
        p_current_btn = ui->pDebugBtn;
        p_display_status_lbl->setText("Log level : DEBUG");

    } else if (src == ui->pInfoBtn) {
        m_level = INFO;
        p_current_btn = ui->pInfoBtn;
        p_display_status_lbl->setText("Log level : INFO");

    } else if (src == ui->pWarnBtn) {
        m_level = WARN;
        p_current_btn = ui->pWarnBtn;
        p_display_status_lbl->setText("Log level : WARN");

    } else if (src == ui->pTraceBtn) {
        m_level = TRACE;
        p_current_btn = ui->pTraceBtn;
        p_display_status_lbl->setText("Log level : TRACE");
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


/*
 * QLogger - A tiny Qt logging framework.
 *
 * MIT License
 * Copyright (c) 2013 sandro fadiga
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <QtTest>
#include <QCoreApplication>

#include <QTime>
#include <QRunnable>
#include <QThreadPool>
#include <QDebug>
#include <QString>

// add necessary includes here
#include "qlogger.h"

using namespace qlogger;

#include <QtCore>

//!
//! \brief The QLoggerTest class - this is a broad test class for the qlogger project
//! it do some unit testing and also application testing
//!
class QLoggerTest : public QObject
{
    Q_OBJECT

public:
    QLoggerTest() = default;
    ~QLoggerTest() = default;

public slots:
   void signalSlotPrinter(QString msg);
   void signalSlotPrinter2(QString msg);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_caseQuickLogger();
    void test_caseOverrideRootLoggerBehaviour();
    void test_caseConfigurationEqual();
    void test_caseConfigurationValidate();
    void test_caseConfigurationLevelFromString();
    void test_caseConfigurationLevelToString();
    void test_caseOutputFromString();
    void test_caseConsoleFormatString();
    void test_caseQLoggerAdd();
    void test_caseHeavyLoadFileTest();
    void test_caseConfigFromFile();
    void test_caseSameLoggerMultiLevels();
    void test_caseChangeAllConfiguration();
    void test_caseXmlOutputFromIni();
    void test_caseLogSignals();
    void test_caseJSONOutput();
};

void QLoggerTest::signalSlotPrinter(QString msg)
{
    qDebug() << "test_caseSignalSlot:" << msg << endl;
}

void QLoggerTest::signalSlotPrinter2(QString msg)
{
    qDebug() << "test_caseSignalSlot 2:" << msg << endl;
}

void QLoggerTest::initTestCase()
{
    // add a logger with an output to a signal
    QLogger::addLogger("sig", q2WARN, SIGNAL);
    QLogger::addLogger("sig2", q4DEBUG, SIGNAL);
    // connects the signal of the slot with a slot on this test class
    connect(QLogger::getSignal("sig"), &SignalOutput::qlogger, this, &QLoggerTest::signalSlotPrinter);
    connect(QLogger::getSignal("sig2"), &SignalOutput::qlogger, this, &QLoggerTest::signalSlotPrinter2);
    //try to connect to a non existent signal output
    connect(QLogger::getSignal("sig3"), &SignalOutput::qlogger, this, &QLoggerTest::signalSlotPrinter2);
}

void QLoggerTest::cleanupTestCase()
{
}

void QLoggerTest::test_caseConfigurationEqual()
{
    Configuration c1("owner", Level::q1ERROR);
    Configuration c2("owner", Level::q1ERROR);
    QVERIFY2(c1 == c2, "c1 and c2 must be equal");

    Configuration c3("noowner");
    QVERIFY2(!(c1 == c3), "c1 and c3 must be different");

    Configuration c4 = c2;
    QVERIFY2(c1 == c4, "c1 and c4 must be equal");
}

void QLoggerTest::test_caseConfigurationValidate()
{
    Configuration c1("owner", Level::q1ERROR);
    QVERIFY2(c1.validate(), "c1 must return validate true");

    Configuration c2("", q1ERROR);
    QVERIFY2(c2.validate() == false, "c2 must return validate false");

    Configuration c3(nullptr, q1ERROR);
    QVERIFY2(c3.validate() == false, "c3 must return validate false");
}

void QLoggerTest::test_caseConfigurationLevelFromString()
{
    Configuration c1("owner", q0FATAL);
    QVERIFY2(levelFromString(FATAL_LEVEL) == c1.getLogLevel(), "equal levels");

    Configuration c2("owner", q1ERROR);
    QVERIFY2(levelFromString(ERROR_LEVEL) == c2.getLogLevel(), "equal levels");

    Configuration c3("owner", q2WARN);
    QVERIFY2(levelFromString(WARN_LEVEL) == c3.getLogLevel(), "equal levels");

    Configuration c4("owner", q3INFO);
    QVERIFY2(levelFromString(INFO_LEVEL) == c4.getLogLevel(), "equal levels");

    Configuration c5("owner", q4DEBUG);
    QVERIFY2(levelFromString(DEBUG_LEVEL) == c5.getLogLevel(), "equal levels");

    Configuration c6("owner", q5TRACE);
    QVERIFY2(levelFromString(TRACE_LEVEL) == c6.getLogLevel(), "equal levels");

    QVERIFY2(levelFromString(FATAL_LEVEL) != c6.getLogLevel(), "not equal levels");
}

void QLoggerTest::test_caseConfigurationLevelToString()
{
    QVERIFY2(levelToString(q0FATAL) == FATAL_LEVEL, "equal levels");

    QVERIFY2(levelToString(q1ERROR) == ERROR_LEVEL, "equal levels");

    QVERIFY2(levelToString(q2WARN) == WARN_LEVEL, "equal levels");

    QVERIFY2(levelToString(q3INFO) == INFO_LEVEL, "equal levels");

    QVERIFY2(levelToString(q4DEBUG) == DEBUG_LEVEL, "equal levels");

    QVERIFY2(levelToString(q5TRACE) == TRACE_LEVEL, "equal levels");

    QVERIFY2(levelToString(q5TRACE) != FATAL_LEVEL, "not equal levels");
}

void QLoggerTest::test_caseOutputFromString()
{
    QVERIFY2(ouputFromString("console") == CONSOLE, "CONSOLE output is equal");
    QVERIFY2(ouputFromString("xml") == XMLFILE, "XML FILE output is equal");
    QVERIFY2(ouputFromString("text") == TEXTFILE, "PLAIN TEXT FILE output is equal");
    QVERIFY2(ouputFromString("signal") == SIGNAL, "SIGNAL output is equal");
}

void QLoggerTest::test_caseConsoleFormatString()
{
    Configuration *cf = new Configuration("owner", Level::q1ERROR);
    ConsoleOutput *co = new ConsoleOutput(cf);

    co->write("test case on console", "owner", q2WARN, QDateTime::currentDateTime(), "test_caseConsoleFormatString", 123);

    QSharedPointer<Configuration> cfg = co->getConfiguration();

    //QString msg = co->formatLogText("", "", "", "", "", "", 1);

    QVERIFY2(cfg.get() == cf, "test same reference");
}

void QLoggerTest::test_caseQuickLogger()
{
    QLOG_FATAL("this is quick logged using the default root logger");
    QLOG_ERROR("the default root logger has a ERROR level and CONSOLE output as default");
    QLOG_FATAL("this default behaviour can be easily changed, see: overrideRootLoggerBehaviour()");
}

void QLoggerTest::test_caseOverrideRootLoggerBehaviour()
{
    //changes the default logger from error and fatal to all levels (top level, trace).
    QLOG_TRACE("this will not be displayed, for now root is error and fatal only"); //will not appear on the console !
    QLOG_ERROR("this will be displayed, its an error");
    QLogger::addLogger("root", q5TRACE, CONSOLE);
    QLOG_TRACE("now this trace log will be displayed");
}

void QLoggerTest::test_caseQLoggerAdd()
{
    Configuration *cf = new Configuration("owner", q1ERROR);
    ConsoleOutput *co = new ConsoleOutput(cf);
    QLogger::addLogger(cf, co);
    QVERIFY2(cf != nullptr, "configuration added to the logger sucessfully");
    QVERIFY2(co != nullptr, "output added to the logger sucessfully");
    QLogger::error("test message","owner");

    QLOG_ERROR("end of test_caseQLoggerAdd");
}

void QLoggerTest::test_caseHeavyLoadFileTest()
{
    QLogger::addLogger("heavy", q1ERROR, TEXTFILE);
    QLogger::addLogger("heavy", q0FATAL, CONSOLE);
    QLOG_FATAL(QString("start:%1").arg(QTime::currentTime().toString("mmss")));
    const int count = 100000;

    QLOG_FATAL("this will not be logged on the file", "heavy");

    QLOG_FATAL(QString("end:%1").arg(QTime::currentTime().toString("mmss")));

    QLOG_FATAL(QString("start:%1").arg(QTime::currentTime().toString("mmss")));

    for (int i = 0; i != count; ++i)
    {
       QLOG_ERROR("this warning is logged on the file", "heavy");
       if(i % 10000 == 0)
           QLOG_FATAL(QString("STILL WORKING PLEASE WAIT! %1").arg(QTime::currentTime().toString("mmss")));

    }
    QLOG_FATAL(QString("end:%1").arg(QTime::currentTime().toString("mmss")));
}

void QLoggerTest::test_caseConfigFromFile()
{
    QLOG_ERROR("this log will test config files, and save a file to more than one log file with 10mb", "cons");
    QLOG_ERROR("the config file must be on application path, can be on any folder together with or below the executable", "cons");
    QLOG_ERROR("this will take a while", "cons");

    const int count = 50000;
    for (int i = 0; i != count; ++i)
    {
        QLOG_WARN(QString("a warn log from config and save to a file n:%1 !").arg(i), "file");
        QLOG_ERROR(QString("an error log from config and save to a file n:%1 !").arg(i), "file");
        QLOG_FATAL(QString("a fatal a log from config and save to a file n:%1 !").arg(i), "file");
    }
    for (int i = 0; i != 5; ++i)
    {
        QLOG_INFO("config from file, log to console","cons");
        QLOG_TRACE("config from file, log to console","cons");
    }
    QLOG_ERROR("end of test, check for the files on c:");
}

void QLoggerTest::test_caseSameLoggerMultiLevels()
{
    //one can have a logger with diferent levels to diferent outputs
    QLogger::addLogger("multilevel", q0FATAL, CONSOLE);
    QLogger::addLogger("multilevel", q5TRACE, TEXTFILE);
    const int count = 100;
    for (int i = 0; i != count; ++i)
    {
        QLOG_ERROR("will not be logged to console, but will to the file", "multilevel");
        QLOG_FATAL("will be logged to console and to the file", "multilevel");
        QLOG_WARN("will be logged only to the file", "multilevel")
    }
}

void QLoggerTest::test_caseChangeAllConfiguration()
{
    Configuration* cfg = new Configuration("config", q3INFO,
                              "%t [%l] <%o> : %m", "dd-MM-yyyy hh:mm:ss",
                              "myfile_%3_%2_%1.log", "ddMMyyyy_hhmmss", "c:\\temp", 10000);

    QLogger::addLogger(cfg, TEXTFILE);

    const int count = 100;
    for (int i = 0; i != count; ++i)
    {
        QLOG_WARN("this will be saved on a file at c:/temp", "config");
    }
    QLOG_FATAL("end of this test: test_caseChangeAllConfiguration()");
}

void QLoggerTest::test_caseXmlOutputFromIni()
{
    QLOG_ERROR("this log will test an xml output format");

    //QLOG_ERROR("this will take a while", "cons");

    const int count = 500;
    for (int i = 0; i != count; ++i)
    {
        QLOG_TRACE(QString("a TRACE log from config and save to a XML file n:%1 !").arg(i), "xml");
        QLOG_ERROR(QString("an ERROR log from config and save to a XML file n:%1 !").arg(i), "xml");
        QLOG_FATAL(QString("a FATAL log from config and save to a XML file n:%1 !").arg(i), "xml");
    }

    QLOG_ERROR("end of test, check for the files on c:/temp");
}

void QLoggerTest::test_caseLogSignals()
{
    QLOG_WARN("signal slot test", "sig");
    QLOG_WARN("signal slot test", "sig");
    QLOG_WARN("signal slot test", "sig");
    QLOG_WARN("signal slot test", "sig");
    QLOG_WARN("signal slot test", "sig");

    QLOG_DEBUG("signal slot test 2", "sig2");
    QLOG_DEBUG("signal slot test 2", "sig2");
    QLOG_ERROR("signal slot test 2", "sig2");
    QLOG_ERROR("signal slot test 2", "sig2");
    QLOG_ERROR("signal slot test 2", "sig2");

    // the expected test output is to nothing happens
    // since there is no sig3 to connect with
    // but a runtime error will be displayed in application console:
    // QWARN  : QLoggerTest::initTestCase() QObject::connect: invalid null parameter
    QLOG_ERROR("non existent signal slot test 3", "sig3");
    QLOG_ERROR("non existent signal slot test 3", "sig3");
    QLOG_ERROR("non existent signal slot test 3", "sig3");
}

void QLoggerTest::test_caseJSONOutput()
{
    QLogger::addLogger("json", q5TRACE, JSON);
    const int count = 100000;
/*    QLOG_FATAL(QString("start:%1").arg(QTime::currentTime().toString("mmss")));
    const int count = 100000;
    for (int i = 0; i != count; ++i)
    {
     if(i % 10000 == 0)
         QLOG_WARN("this will not be logged on the file", "json");
    }
    QLOG_FATAL(QString("end:%1").arg(QTime::currentTime().toString("mmss")));
*/
    QLOG_FATAL(QString("start:%1").arg(QTime::currentTime().toString("mmss")));

    for (int i = 0; i != count; ++i)
    {
    QLOG_FATAL("this warning is logged on the JSON file", "json");
    if(i % 10000 == 0)
        QLOG_FATAL(QString("STILL WORKING PLEASE WAIT! %1").arg(QTime::currentTime().toString("mmss")));

    }
    QLOG_FATAL(QString("end:%1").arg(QTime::currentTime().toString("mmss")));

    QLOG_INFO("fim test_caseJSONOutput");
}


QTEST_MAIN(QLoggerTest)

#include "tst_qloggertest.moc"

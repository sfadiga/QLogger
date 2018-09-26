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

/*
 *  This is document / test program to show some of the QLogger capabilities.
 */

#include <QCoreApplication>

#include "qlogger.h"

#include <QTime>
#include <QRunnable>
#include <QThreadPool>

using namespace qlogger;

class LoggerA : public QRunnable
{
    void run()
    {
        for(int i = 0 ; i < 1000000 ; i++)
            QLOG_ERROR("Logger A speaking !", "threads");
    }
};
class LoggerB : public QRunnable
{
    void run()
    {
        for(int i = 0 ; i < 1000000 ; i++)
            QLOG_FATAL("Logger B speaking !", "threads");
    }
};

void threadHeavyLoadTest()
{
    QLogger::addLogger("threads");
    LoggerA* loga = new LoggerA;
    LoggerB* logb = new LoggerB;
    QThreadPool::globalInstance()->start(loga);
    QThreadPool::globalInstance()->start(logb);
}

void heavyLoadToFileTest()
{
    qlogger::QLogger::addLogger("root", qlogger::Configuration::q2WARN, qlogger::Configuration::TEXTFILE);
    QLOG_FATAL(QString("start:%1").arg(QTime::currentTime().toString("mmss")));
    const int count = 1000000;
    for (int i = 0; i != count; ++i)
    {
        QLOG_INFO("this is not executed");
    }
    QLOG_FATAL(QString("end:").arg(QTime::currentTime().toString("mmss")));

    QLOG_FATAL(QString("start:%1").arg(QTime::currentTime().toString("mmss")));

    for (int i = 0; i != count; ++i)
    {
        QLOG_WARN("this is executed");
    }
    QLOG_FATAL(QString("end:").arg(QTime::currentTime().toString("mmss")));

    QLOG_INFO("fim", "teste");
}

void configFromFileTest()
{
    QLOG_ERROR("this log will test config files, and save a file to more than one log file with 10mb", "cons");
    QLOG_ERROR("the config file must be on application path, can be on any folder together with or below the executable", "cons");
    QLOG_ERROR("this will take a while", "cons");

    const int count = 5000;
    for (int i = 0; i != count; ++i)
    {
        QLOG_WARN(QString("a warn log from config and save to a file n:%1 !").arg(i), "file");
        QLOG_ERROR(QString("an error log from config and save to a file n:%1 !").arg(i), "file");
        QLOG_FATAL(QString("a fatal a log from config and save to a file n:%1 !").arg(i), "file");
    }
    for (int i = 0; i != 10; ++i)
    {
        QLOG_INFO("config from file, log to console","cons");
        QLOG_TRACE("config from file, log to console","cons");
    }
    QLOG_ERROR("end of test, check for the files on c:");
}

void sameLoggerMultiLevels()
{
    //one can have a logger with diferent levels to diferent outputs
    QLogger::addLogger("multilevel", Configuration::q0FATAL, Configuration::CONSOLE);
    QLogger::addLogger("multilevel", Configuration::q5TRACE, Configuration::TEXTFILE);
    const int count = 100;
    for (int i = 0; i != count; ++i)
    {
        QLOG_ERROR("will not be logged to console, but will to the file", "multilevel");
        QLOG_FATAL("will be logged to console and to the file", "multilevel");
        QLOG_WARN("will be logged only to the file", "multilevel")
    }
}

void changeAllConfiguration()
{
    shared_ptr<Configuration> cfg =
            shared_ptr<Configuration>(new Configuration("config", Configuration::q3INFO, Configuration::TEXTFILE,
                              "dd-MM-yyyy hh:mm:ss", "%t [%l] <%o> : %m",
                              "myfile_%3_%2_%1.log", "ddMMyyyy_hhmmss", "c:\\", 10000));
    QLogger::addLogger(cfg);
    const int count = 100;
    for (int i = 0; i != count; ++i)
    {
        QLOG_WARN("this will be saved on a file at c:", "config");
    }
    QLOG_FATAL("end of this test");
}

void overrideRootLoggerBehaviour()
{
    //changes the default logger from error and fatal to all levels (top level, trace).
    QLOG_TRACE("this will not be displayed, for now root is error and fatal only");
    QLOG_ERROR("this will be displayed, its an error");
    QLogger::addLogger("root", Configuration::q5TRACE, Configuration::CONSOLE);
    QLOG_TRACE("now this trace log will be displayed");
}

void quickLogger()
{
    QLOG_FATAL("this is quick logged using the default root logger");
    QLOG_ERROR("the default root logger has a ERROR level and CONSOLE output as default");
    QLOG_FATAL("this default behaviour can be easily changed, see: overrideRootLoggerBehaviour()");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //heavyLoadToFileTest();

    //threadHeavyLoadTest();

    //sameLoggerMultiLevels();

    //changeAllConfiguration();

    configFromFileTest();

    //overrideRootLoggerBehaviour();

    //quickLogger();

    return a.exec();
}




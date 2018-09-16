/*
    QLogger - A tiny Qt logging framework.

    MIT License
    Copyright (c) 2013 sandro fadiga

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
    AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "qlogger.h"

#include <QDateTime>
#include <QTextCodec>

#include "configfilehandler.h"

namespace qlogger {

QAtomicPointer<QLogger> QLogger::instance = 0;
QLoggerDestroyer QLogger::destroyer;
QMutex QLogger::mutex;
QMutex QLogger::writex;

QLoggerDestroyer::QLoggerDestroyer(QAtomicPointer<QLogger> s)
{
    singleton = s;
}

QLoggerDestroyer::~QLoggerDestroyer()
{
    QLogger::mutex.lock();
    // call the close file methods for all loggers
    QLoggerDestroyer::getPointerInstance()->close();
    delete QLoggerDestroyer::getPointerInstance();
    QLogger::mutex.unlock();
}

void QLoggerDestroyer::SetSingleton(QAtomicPointer<QLogger> s)
{
    singleton = s;
}

QLogger::QLogger()
{}

QLogger::~QLogger()
{}

void QLogger::close()
{
    QList<Configuration *> cfgList = loggers.values();
    const int size = cfgList.size();

    for (int i = 0 ; i < size ; i++) {
        Configuration *cfg = cfgList.at(i);
        cfg->getOutputLog()->close();
    }
}

QLogger *QLogger::getInstance()
{
    if (!QLogger::getPointerInstance()) {
        mutex.lock();

        if (instance.testAndSetOrdered(0, new QLogger())) {
            destroyer.SetSingleton(instance);
            QList<Configuration *> lst;
            //add the default "root" logger
            lst.append(new Configuration("root", Configuration::q1ERROR));
            // configuration file read, then start with the default logger
            ConfigFileHandler::parseConfigurationFile(lst);//QLogger::getPointerInstance()->loggers);
            int size =  lst.size();

            for (int i = 0 ; i < size ; i++) {
                Configuration *config = lst.at(i);
                QLogger::getPointerInstance()->loggers.insert(config->getLogOwner(), config);

                if (config->getOutputType() == Configuration::SIGNAL) {
                    connect(config, &Configuration::logMessage,
                            QLogger::getPointerInstance(), &QLogger::logMessage);
                }
            }
        }

        mutex.unlock();
    }

    return QLogger::getPointerInstance();
}

void QLogger::addLogger(QString logOwner, Configuration *configuration)
{
    QLogger::getInstance();
    mutex.lock();

    if (!QLogger::getPointerInstance()->loggers.values(logOwner).contains(configuration)) {
        configuration->setLogOwner(logOwner);
        QLogger::getPointerInstance()->loggers.insert(logOwner, configuration);

        if (configuration->getOutputType() == Configuration::SIGNAL) {
            connect(configuration, &Configuration::logMessage,
                    QLogger::getPointerInstance(), &QLogger::logMessage);
        }
    }

    mutex.unlock();
}

void QLogger::addLogger(Configuration *configuration)
{
    QString logOwner = configuration != NULL ? configuration->getLogOwner() : QString();
    addLogger(logOwner, configuration);
}

void QLogger::addLogger(QString logOwner)
{
    QLogger::addLogger(logOwner, new Configuration(logOwner));
}

void QLogger::addLogger(QString logOwner, Configuration::Level lvl,
                        Configuration::OutputType ouputType,
                        QString timestampFormat)
{
    Configuration *cfg = new Configuration(logOwner, lvl, ouputType, timestampFormat);
    QLogger::addLogger(logOwner, cfg);
}

void QLogger::log(Configuration::Level lvl, QString message, QString functionName, int lineNumber, QString owner)
{
    writex.lock();
    QList<Configuration *> configs = QLogger::getInstance()->loggers.values(owner);
    const int size = configs.size();

    for (int i = 0 ; i < size ; i++) {
        Configuration *cfg = configs.at(i);

        if (lvl <= cfg->getLogLevel()) {
            cfg->getOutputLog()->write(message, Configuration::levelToString(lvl), owner,
                                       QDateTime::currentDateTime().toString(cfg->getTimestampFormat()), functionName, lineNumber);
        }
    }

    writex.unlock();
}

void QLogger::changelogLevel(QString owner, Configuration::Level lvl, Configuration::OutputType outputType)
{
    writex.lock();
    QList<Configuration *> configs = QLogger::getInstance()->loggers.values(owner);
    const int size = configs.size();

    for (int i = 0 ; i < size ; i++) {
        Configuration *cfg = configs.at(i);

        if (cfg->getOutputType() == outputType) {
            cfg->setLogLevel(lvl);
        }
    }

    writex.unlock();
}



}

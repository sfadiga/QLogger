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

#include "qlogger.h"

#include <QDateTime>
#include <QTextCodec>
#include <QDirIterator>
#include <QSettings>

#include "consoleoutput.h"
#include "textoutput.h"
#include "xmloutput.h"
#include "signaloutput.h"
#include "jsonoutput.h"

namespace qlogger
{

QMutex QLogger::writex;

bool QLogger::instanceFlag = false;

QLogger& QLogger::instance()
{
    static QLogger instance;

    if(!instanceFlag)
    {
        instanceFlag = true;
        //add the default "root" logger
        QLogger::addLogger("root");

        // load configurations from cfg file (if any)
        QLogger::readConfigurationFile();
    }

    return instance;
}

void QLogger::addLogger(Configuration* configuration, Output *output)
{
    if(configuration != nullptr && output != nullptr)
    {
        if(configuration->validate())
        {
            instance().loggers.insert(configuration->getLogOwner(), QSharedPointer<Output>(output));
        }
        else //if no logger is created then cleanup memory
        {
            delete configuration;
            delete output;
        }
    }
}

void QLogger::addLogger(Configuration* configuration, OutputType type)
{
    if(configuration != nullptr)
    {
        QLogger::addLogger(configuration->getLogOwner(),
                           configuration->getLogLevel(),
                           type,
                           configuration->getLogTextMask(),
                           configuration->getTimestampFormat(),
                           configuration->getFileNameMask(),
                           configuration->getFileNameTimestampFormat(),
                           configuration->getFilePath(),
                           configuration->getFileMaxSizeInKb());
        delete configuration;
    }
}

void QLogger::addLogger(QString logOwner)
{
    QLogger::addLogger(new Configuration(std::move(logOwner)), CONSOLE);
}

void QLogger::addLogger(QString logOwner, Level lvl, OutputType ouputType,
                        QString logTextMask, QString timestampFormat, QString fileNameMask,
                        QString fileNameTimestampFormat, QString filePath, int fileMaxSizeInKb)
{
    Configuration* configuration = new Configuration(std::move(logOwner), lvl,
                                                     std::move(logTextMask),
                                                     std::move(timestampFormat),
                                                     std::move(fileNameMask),
                                                     std::move(fileNameTimestampFormat),
                                                     std::move(filePath), fileMaxSizeInKb);
    Output* output = nullptr;
    switch(ouputType)
    {
        case CONSOLE:
            output = new ConsoleOutput(configuration);
            break;
        case TEXTFILE:
            output = new PlainTextOutput(configuration);
            break;
        case XMLFILE:
            output = new XmlOutput(configuration);
            break;
        case SIGNAL:
            output = new SignalOutput(configuration);
            break;
        case JSON:
            output = new JSONOutput(configuration);
            break;
    };
    addLogger(configuration, output);
}

void QLogger::log(Level lvl, const QString &message, const QString &functionName, int lineNumber, const QString &owner)
{
    writex.lock();

    QList<QSharedPointer<Output>> outList = instance().loggers.values(owner);
    foreach(QSharedPointer<Output> out, outList)
    {
        if(!out.isNull() && lvl <= out->getConfiguration()->getLogLevel())
        {
            out->write(message, owner, lvl, QDateTime::currentDateTime(), functionName, lineNumber);
        }
    }
    writex.unlock();
}

void QLogger::fatal(const QString &message, const QString &owner, const QString &functionName, int lineNumber)
{
    QLogger::log(q0FATAL, message, functionName, lineNumber, owner);
}

void QLogger::error(const QString &message, const QString &owner, const QString &functionName, int lineNumber)
{
    QLogger::log(q1ERROR, message, functionName, lineNumber, owner);
}

void QLogger::warn(const QString &message, const QString &owner, const QString &functionName, int lineNumber)
{
    QLogger::log(q2WARN, message, functionName, lineNumber, owner);
}

void QLogger::info(const QString &message, const QString &owner, const QString &functionName, int lineNumber)
{
    QLogger::log(q3INFO, message, functionName, lineNumber, owner);
}

void QLogger::debug(const QString &message, const QString &owner, const QString &functionName, int lineNumber)
{
    QLogger::log(q4DEBUG, message, functionName, lineNumber, owner);
}

void QLogger::trace(const QString &message, const QString &owner, const QString &functionName, int lineNumber)
{
    QLogger::log(q5TRACE, message, functionName, lineNumber, owner);
}

SignalOutput *QLogger::getSignal(const QString &owner)
{
    QSharedPointer<Output> out = instance().loggers.value(owner);
    auto sig = dynamic_cast<SignalOutput*>(out.get());
    return sig;
}

QString QLogger::getConfigFilePath(const QString &startingPath, const QString &fileName)
{
    QString path = QDir::cleanPath(startingPath);
    QStringList filter;
    if (!fileName.isEmpty())
        filter << fileName;

    QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    if(it.hasNext())
        return it.next();

    return ""; // file not found
}

void QLogger::readConfigurationFile()
{
    QSettings settings(getConfigFilePath(QDir::currentPath(), CH_CONFIG_FILE_NAME), QSettings::IniFormat);

    //traverse the "log owners settings"
    QStringList owners = settings.childGroups();
    QStringList::const_iterator it;
    for(it = owners.constBegin() ; it != owners.constEnd() ; ++it)
    {
        settings.beginGroup((*it)); // start reading setting from the current owner
            QString level = settings.value(CH_LEVEL, ERROR_LEVEL).toString();
            QString outStr = settings.value(CH_OUTPUT_TYPE, CONSOLE_OUTPUT).toString(); //console
            QString mask = settings.value(CH_LOG_MASK, DEFAULT_TEXT_MASK).toString();
            QString timestamp = settings.value(CH_TIMESTAMP_FORMAT, TIMESTAMP_QLOGGER_FORMAT).toString();
            QString fileName = settings.value(CH_FILE_NAME, TEXT_FILE_NAME_MASK).toString();
            QString fileTimestamp = settings.value(CH_FILE_NAME_TIMESTAMP, FILE_NAME_TIMESTAMP_FORMAT).toString();
            QString path = settings.value(CH_PATH, DEFAULT_LOG_PATH).toString();
            int fileSize = settings.value(CH_MAX_FILE_SIZE, DEFAULT_FILE_SIZE).toInt();
        settings.endGroup();
        instance().addLogger((*it), levelFromString(level), ouputFromString(outStr), mask, timestamp, fileName, fileTimestamp, path, fileSize);
    }

}



}

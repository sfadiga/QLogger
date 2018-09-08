/*
    QLogger - A tiny Qt logging framework.

    MIT License
    Copyright (c) 2013 sandro fadiga

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "configuration.h"

namespace qlogger {

Configuration::Configuration()
    : logOwner(QString()),
      logLevel(Level(-1)),
      outputType(OutputType(-1)),
      outputLog(Q_NULLPTR),  // Changed NULL to Q_NULLPTR to avoid warning
      timestampFormat(QString()) {}

Configuration::Configuration(QString logOwner, Configuration::Level lvl,
                             OutputType outputType, QString timestampFormat,
                             QString logTextMask, QString fileNameMask,
                             QString fileNameTimestampFormat, QString filePath,
                             int fileMaxSizeInKb)
    : logOwner(logOwner),
      logLevel(lvl),
      outputType(outputType),
      outputLog(Q_NULLPTR),  // Changed NULL to Q_NULLPTR to avoid warning
      timestampFormat(timestampFormat)
{
    if (this->timestampFormat.isEmpty()) {
        this->timestampFormat = TIMESTAMP_QLOGGER_FORMAT;
    }

    switch (outputType) {
    case XMLFILE:
        this->outputLog =
            new XmlOutput(logOwner, fileNameMask, fileNameTimestampFormat,
                          filePath, fileMaxSizeInKb);
        break;

    case TEXTFILE:
        this->outputLog =
            new TextFileOutput(logOwner, logTextMask, filePath, fileMaxSizeInKb,
                               fileNameMask, fileNameTimestampFormat);
        break;

    case SIGNAL: {
        SignalOutput *sigOutput = new SignalOutput(logTextMask);
        this->outputLog = sigOutput;
        connect(sigOutput, &SignalOutput::logMessage,
                this, &Configuration::logMessage);
        break;
    }

    case CONSOLE:
        //    default: // Not actually needed as all options used. Gives Warning
        //    in QtCreator.
        this->outputLog = new ConsoleOutput(logTextMask);
        break;
    }
}

Configuration::~Configuration() {}

bool Configuration::operator==(const Configuration &rh)
{
    return (this->logOwner == rh.logOwner && this->logLevel == rh.logLevel &&
            this->outputType == rh.outputType);
}

QString Configuration::levelToString(const Level level)
{
    static const char *const buffer[] = {"FATAL", "ERROR", "WARN",
                                         "INFO",  "DEBUG", "TRACE"
                                        };
    return buffer[level];
}

Configuration::Level Configuration::levelFromString(const QString level)
{
    QString lvl = level.trimmed().toUpper();

    if (lvl == "FATAL") {
        return Configuration::q0FATAL;

    } else if (lvl == "WARN") {
        return Configuration::q2WARN;

    } else if (lvl == "INFO") {
        return Configuration::q3INFO;

    } else if (lvl == "DEBUG") {
        return Configuration::q4DEBUG;

    } else if (lvl == "TRACE") {
        return Configuration::q5TRACE;

    } else { // return default level
        return Configuration::q1ERROR;
    }
}

const QString Configuration::getLogOwner() const
{
    return logOwner;
}

void Configuration::setLogOwner(const QString owner)
{
    this->logOwner = QString(owner);
}

Configuration::Level Configuration::getLogLevel() const
{
    return logLevel;
}

Configuration::OutputType Configuration::getOutputType() const
{
    return outputType;
}

Output *Configuration::getOutputLog() const
{
    return outputLog;
}

const QString Configuration::getTimestampFormat() const
{
    return timestampFormat;
}

}  // namespace qlogger

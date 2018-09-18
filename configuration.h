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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "textoutput.h"
#include "xmloutput.h"
#include <QString>
#include <QWeakPointer>
#include <QObject>

namespace qlogger {

//!
static const QString TIMESTAMP_QLOGGER_FORMAT = "MM/dd/yyyy hh:mm:ss";

//! This class stores the configuration details for each log to be created.
//! Default configuration can be created with the default constructor.
class Configuration : public QObject {
    Q_OBJECT
public:
    // http://en.wikipedia.org/wiki/Java_logging_framework
    // http://commons.apache.org/logging/guide.html#Message%20Priorities/Levels
    //!
    enum Level { q0FATAL, q1ERROR, q2WARN, q3INFO, q4DEBUG, q5TRACE };

    //!
    enum OutputType {
        CONSOLE,
        TEXTFILE,
        XMLFILE,
        SIGNAL,
    };

public:
    Configuration();

    Configuration(QString logOwner, Level lvl = q1ERROR,
                  OutputType ouputType = CONSOLE,
                  QString timestampFormat = TIMESTAMP_QLOGGER_FORMAT,
                  QString logTextMask = DEFAULT_TEXT_MASK,
                  QString fileNameMask = QString(),
                  QString fileNameTimestampFormat = FILE_NAME_TIMESTAMP_FORMAT,
                  QString filePath = QDir::currentPath(),
                  int fileMaxSizeInKb = 1000);

    //! destructor, check for outputLog and closes it
    virtual ~Configuration();

    //! equals operator is used to check if a configuration have the same level
    //! and output type only
    bool operator==(const Configuration &rh);



    //! utility to convert the level enum to string
    static QString levelToString(const Configuration::Level level);

    static Configuration::Level levelFromString(const QString level);

signals:
    void logMessage(QString);

private:
    //! store the name of the log owner to pass it to the output classes
    QString logOwner;

    //! level of this configuration
    Level logLevel;

    //! output medium to be used to log the message
    OutputType outputType;

    //! this attribute is responsible for writing the log on the output
    Output *outputLog;

    //! the date time format to be displayed on the log text it must use Qt Date
    //! Time format convention
    QString timestampFormat;

public:
    const QString getLogOwner() const;
    void setLogOwner(const QString owner);
    Level getLogLevel() const;
    void setLogLevel(const Level);
    Configuration::OutputType getOutputType() const;
    Output *getOutputLog() const;
    const QString getTimestampFormat() const;
};

} // namespace qlogger

#endif // CONFIGURATION_H

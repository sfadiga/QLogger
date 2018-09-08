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

#ifndef TEXTOUTPUT_H
#define TEXTOUTPUT_H

#include "output.h"

#include <QCoreApplication>
#include <QFile>
#include <QObject>
#include <QTextStream>

#include <QPointer>

// %t - datetime
// %o - owner
// %l - level
// %m - message
static const QString DEFAULT_TEXT_MASK = "%t [%o] <%l> (%f) {line:%n} - %m";

//! log_appname_logname_datetime.txt %1 = application name , %2 = owner , %3 =
//! timestamp
static const QString TEXT_FILE_NAME_MASK = "log_%1_%2_%3.txt";

//! will be used on the file name mask
static const QString FILE_NAME_TIMESTAMP_FORMAT = "yyyyMMdd_hhmmss";

namespace qlogger {

class BaseOutput : public Output {
public:
    BaseOutput(QString mask = DEFAULT_TEXT_MASK);
    virtual ~BaseOutput();

    //! return the current mask used to format the log text
    QString getLogTextMask() const;

protected:
    //! the text mask used to format the log output
    QString logTextMask;

    //! uses the mask param and log message to generate the text string which will
    //! be logged out.
    QString formatLogText(const QString logFormatMask, const QString message,
                          const QString owner, const QString lvl,
                          const QString timestamp, const QString functionName,
                          const int lineNumber);
};

class ConsoleOutput : public BaseOutput {
public:
    //! This default constructor sets the class to be on console mode, all logs
    //! will be redirected to stdout
    ConsoleOutput(QString mask);
    // ConsoleOutput();
    virtual ~ConsoleOutput();

    //! this method is responsible for write the log text in the selected output
    virtual void write(const QString message, const QString owner,
                       const QString lvl, const QString timestamp,
                       const QString functionName, const int lineNumber);
    //! implement from output
    virtual void close();

protected:
    //! the qt class to output texts
    QTextStream *qTextStream;
};

//! this class is responsible for write plain text log messages for file or
//! stdout (console)
class TextFileOutput : public ConsoleOutput {

public:
    //! This constructor sets the class to operate in text file mode, all logs
    //! will be written in a file
    TextFileOutput(QString logOwner, QString logFormatMask, QString filePath,
                   int fileMaxSizeInKb,
                   QString fileNameMask = TEXT_FILE_NAME_MASK,
                   QString fileNameTimestampFormat = FILE_NAME_TIMESTAMP_FORMAT);
    virtual ~TextFileOutput();

    //! this method is responsible for write the log text in the selected output
    virtual void write(const QString message, const QString owner,
                       const QString lvl, const QString timestamp,
                       const QString functionName, const int lineNumber);

    virtual void close();

protected:
    //! this method is used to generate a file name based on the application name,
    //! log owner and date time
    QString getFileName() const;

    //! this method is called when a new file needs to be created due to size
    //! limitation
    virtual void createNextFile();

protected:
    //! this is the log owner name of this output text, used to form the file name
    QString logOwner;

    //! this attribute stores the desired path to save the log files
    QString filePath;

    //! this attribute stores the maximum file size in bytes
    int fileMaxSizeInBytes;

    //! mask used to generate
    QString fileNameMask;

    //! the timestamp format to be used in the file name
    QString fileNameTimestampFormat;

    //! this attribute represents the current file being used, when in file mode,
    //! otherwise = NULL
    QFile *currentFile;
};

class SignalOutput : public QObject, public BaseOutput {
    Q_OBJECT
public:
    SignalOutput(QString logFormatMask);
    ~SignalOutput();

    //! this method is responsible for write the log text in the selected output
    virtual void write(const QString message, const QString owner,
                       const QString lvl, const QString timestamp,
                       const QString functionName, const int lineNumber);

    virtual void close();

signals:
    void logMessage(QString);
};

} // namespace qlogger

#endif // TEXTOUTPUT_H

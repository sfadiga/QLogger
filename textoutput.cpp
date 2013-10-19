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

#include "textoutput.h"

#include <QDateTime>
#include <QFile>
#include <QDir>
#include <iostream>
#include <QTextCodec>

namespace qlogger
{

ConsoleOutput::ConsoleOutput(QString mask)
    : qTextStream(new QTextStream(stdout))
{
    // check if a valid log mask is informed, only the log owner is not obligatory
    if(!mask.contains("%t") || !mask.contains("%m") || !mask.contains("%l"))
        this->logTextMask = DEFAULT_TEXT_MASK;
    else
        this->logTextMask = mask;

	qTextStream->device()->setTextModeEnabled(true);
}

ConsoleOutput::~ConsoleOutput()
{
	if(qTextStream)
		delete qTextStream;
}

QString ConsoleOutput::getLogTextMask() const
{
    return logTextMask;
}

void ConsoleOutput::write(const QString message,
                          const QString owner,
                          const QString lvl,
                          const QString timestamp,
                          const QString functionName,
                          const int lineNumber)
{
    //QByteArray msg = QTextCodec::convertFromUnicode(); TODO correct the conversion to from utf-8 to the console output (local)
    *qTextStream << formatLogText(this->logTextMask, message, owner, lvl, timestamp, functionName, lineNumber) << endl;
}

QString ConsoleOutput::formatLogText(const QString logFormatMask,
                                     const QString message,
                                     const QString owner,
                                     const QString lvl,
                                     const QString timestamp,
                                     const QString functionName,
                                     const int lineNumber)
{
    QString text = QString(logFormatMask);
    text = text.replace("%t", timestamp);
    text = text.replace("%m", message);
    text = text.replace("%l", lvl);
    text = text.replace("%o", owner);
    text = text.replace("%f", functionName);
    text = text.replace("%n", QString::number(lineNumber));
    return text;
}

void ConsoleOutput::close()
{
    qTextStream->flush();
}

TextFileOutput::TextFileOutput(QString logOwner, QString logFormatMask, QString path,
                               int fileSizeInKb, QString fileMask,
                               QString timestampFormat)
    : ConsoleOutput(logFormatMask),
      logOwner(logOwner), filePath(path), fileMaxSizeInBytes(fileSizeInKb * 1024),
      fileNameMask(fileMask), fileNameTimestampFormat(timestampFormat), currentFile(NULL)
{
    qTextStream = NULL;

    // check if a valid size was informed, minimum of 10k
    if(fileMaxSizeInBytes < 10240)
       fileMaxSizeInBytes = 10240;

    // check if a valid path was informed
    QDir test(filePath);
    if(filePath.isEmpty() || !test.exists())
		filePath = QDir::currentPath();
    else
        filePath = path;

    if(fileNameTimestampFormat.isEmpty())
        fileNameTimestampFormat = FILE_NAME_TIMESTAMP_FORMAT;

    if(fileNameMask.isEmpty() || !(fileNameMask.contains("%1") && fileNameMask.contains("%2") && fileNameMask.contains("%3")))
        fileNameMask = TEXT_FILE_NAME_MASK;
    else
        fileNameMask = fileMask;

    createNextFile();
}

TextFileOutput::~TextFileOutput()
{
	if(qTextStream)
		delete qTextStream;
	if(currentFile)
		delete currentFile;
}

void TextFileOutput::createNextFile()
{
	if(qTextStream && currentFile)
    {
        qTextStream->flush();
        delete qTextStream;
        currentFile->flush();
        currentFile->close();
    }
    // TEXT FILE MODE
	QDir dir(filePath);
	QString myFile = dir.absoluteFilePath(getFileName());
	currentFile = new QFile(myFile);

    if(currentFile->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qTextStream = new QTextStream(currentFile);
	}	
    else // Fall back to console mode
	{
        qTextStream = new QTextStream(stdout);
	}

    // enables the output to text mode and have correct line breaks
    qTextStream->device()->setTextModeEnabled(true);
    qTextStream->setCodec(QTextCodec::codecForName("UTF-8"));
}

QString TextFileOutput::getFileName() const
{
    QString name = fileNameMask
            .arg(QCoreApplication::applicationName())
            .arg(logOwner)
            .arg(QDateTime::currentDateTime().toString(fileNameTimestampFormat));
    return name;
}

void TextFileOutput::write(const QString message,
                           const QString owner,
                           const QString lvl,
                           const QString timestamp,
                           const QString functionName,
                           const int lineNumber)
{
    if(currentFile && currentFile->size() > fileMaxSizeInBytes)
    {
        createNextFile();
    }
    ConsoleOutput::write(message, lvl, owner, timestamp, functionName, lineNumber);
}

//!
void TextFileOutput::close()
{
    qTextStream->flush();
	currentFile->close();
}

}

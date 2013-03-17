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

#ifndef XMLOUTPUT_H
#define XMLOUTPUT_H

#include "textoutput.h"

#include <QXmlStreamWriter>
#include <QPointer>


namespace qlogger
{

//! log_appname_logname_datetime.txt %1 = application name , %2 = owner , %3 = timestamp
static const QString XML_FILE_NAME_MASK = "log_%1_%2_%3.xml";

static const QString XML_TAG = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
static const QString ROOT_TAG_OPEN = "<logs>";
static const QString ROOT_TAG_CLOSE = "</logs>";
static const QString LOG_TAG_OPEN = "<log>";
static const QString LOG_TAG_CLOSE = "</log>";
static const QString DATE_TIME_TAG = "<date_time>%1</date_time>";
static const QString OWNER_TAG = "<owner>%1</owner>";
static const QString MESSAGE_TAG = "<message>%1</message>";
static const QString LINE_TAG = "<line_number>%1</line_number>";
static const QString FUNCTION_TAG = "<function>%1</function>";
static const QString LEVEL_TAG = "<level>%1</level>";

//!
class XmlOutput : public TextFileOutput
{
public:
    XmlOutput(QString logOwner,  QString fileNameMask = XML_FILE_NAME_MASK,
              QString fileNameTimestampFormat = FILE_NAME_TIMESTAMP_FORMAT,
              QString filePath = QCoreApplication::applicationDirPath(), int fileMaxSizeInKb = 100);
    virtual ~XmlOutput();

    //! reimplemented to write a xml on the file
    virtual void write(QString message, QString owner, QString lvl, QString timestamp, QString functionName, int lineNumber);

    //!
    virtual void close();

protected:
    //! this method is called when a new file needs to be created due to size limitation
    virtual void createNextFile();
	
};

}

#endif // XMLOUTPUT_H

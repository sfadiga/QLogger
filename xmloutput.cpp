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

#include "xmloutput.h"

#include <QDir>
#include <QDateTime>
#include <QTextCodec>

namespace qlogger
{

XmlOutput::XmlOutput(QString logOwner,
                     QString fileNameMask,
                     QString fileNameTimestampFormat,
                     QString filePath,
                     int fileMaxSizeInKb)
    : TextFileOutput(logOwner, DEFAULT_TEXT_MASK, filePath, fileMaxSizeInKb, fileNameMask, fileNameTimestampFormat)
{
    if(fileNameMask.isEmpty() || !(fileNameMask.contains("%1") && fileNameMask.contains("%2") && fileNameMask.contains("%3")))
        fileNameMask = XML_FILE_NAME_MASK;

	QDir::setCurrent(filePath);
    createNextFile();
}

XmlOutput::~XmlOutput()
{	
}

//!
void XmlOutput::close()
{
}

void XmlOutput::createNextFile()
{
	if(currentFile && qTextStream)
	{
		qTextStream->flush();
		currentFile->flush();
		currentFile->close();
	}
	currentFile = new QFile(getFileName());
	
    if(currentFile->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qTextStream = new QTextStream(currentFile);
		//qTextStream->setDevice(&currentFile);
	}
	// enables the output to text mode and have correct line breaks
	qTextStream->device()->setTextModeEnabled(true);
    qTextStream->setCodec(QTextCodec::codecForName("UTF-8"));

    *qTextStream << XML_TAG << endl;
    *qTextStream << ROOT_TAG_OPEN << endl;
}

void XmlOutput::write(QString message, QString owner, QString lvl, QString timestamp, QString functionName, int lineNumber)
{
    if(currentFile->size() > fileMaxSizeInBytes)
    {
        createNextFile();
	}

	*qTextStream << LOG_TAG_OPEN << endl;
    *qTextStream << DATE_TIME_TAG.arg(timestamp) << endl;
    *qTextStream << LEVEL_TAG.arg(lvl) << endl;
    *qTextStream << OWNER_TAG.arg(owner) << endl;
    *qTextStream << MESSAGE_TAG.arg(message) << endl;
    *qTextStream << LINE_TAG.arg(lineNumber) << endl;
    *qTextStream << FUNCTION_TAG.arg(functionName) << endl;
	*qTextStream << LOG_TAG_CLOSE << endl;
}


}

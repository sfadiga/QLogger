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
#include "qloggerlib_global.h"
#include "configuration.h"

#include <QDir>
#include <QDateTime>
#include <QTextCodec>

namespace qlogger
{

XmlOutput::XmlOutput(Configuration *conf) : PlainTextOutput(conf)
{
    this->configuration->setFileNameMask(XML_FILE_NAME_MASK);
}

XmlOutput::~XmlOutput()
{	
    if(!outputFile.isNull() && outputFile->isOpen())
    {
        // end current xml file.
        *outputStream << ROOT_CLOSE_TAG << endl;
    }
    PlainTextOutput::close();
}

void XmlOutput::write(const QString message,
                      const QString owner,
                      const Level lvl,
                      const QDateTime timestamp,
                      const QString functionName,
                      const int lineNumber)
{
    if(!outputFile.isNull()
            && outputFile->isOpen()
            && (outputFile->size() > configuration->getFileMaxSizeInBytes()))
    {
        // end current xml file.
        *outputStream << ROOT_CLOSE_TAG << endl;
    }

    if(outputFile.isNull() //if there is no file
            || !outputFile->isOpen() // or the file is not opened for writing
            || (outputFile->size() > configuration->getFileMaxSizeInBytes())) // or the file is already at max size
    {
        createNextFile(); // create a new file

        // start the xml file.
        *outputStream << XML_TAG << endl;
        *outputStream << ROOT_OPEN_TAG << endl;

    }

    *outputStream << LOG_TAG_OPEN << endl;
    *outputStream << DATE_TIME_TAG.arg(timestamp.toString(configuration->getTimestampFormat())) << endl;
    *outputStream << LEVEL_TAG.arg(lvl) << endl;
    *outputStream << OWNER_TAG.arg(owner) << endl;
    *outputStream << MESSAGE_TAG.arg(message) << endl;
    *outputStream << LINE_TAG.arg(lineNumber) << endl;
    *outputStream << FUNCTION_TAG.arg(functionName) << endl;
    *outputStream << LOG_TAG_CLOSE << endl;

}

}

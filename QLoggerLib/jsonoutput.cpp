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
#include "jsonoutput.h"

namespace qlogger
{

JSONOutput::JSONOutput(Configuration *cfg) : PlainTextOutput (cfg)
{
    this->configuration->setFileNameMask(JSON_FILE_NAME_MASK);
}

JSONOutput::~JSONOutput()
{
    if(!outputFile.isNull() && outputFile->isOpen())
    {
        // end current json file.
        close();
    }
    PlainTextOutput::close();
}

void JSONOutput::close()
{
    if(!outputFile.isNull())
    {
        // end current json file.
        jsonCurrentLog[JSON_LOGS] = jsonCurrentList;

        QJsonDocument jsonDoc(jsonCurrentLog);
        outputFile->write(jsonDoc.toJson());
        // release the file
        outputFile->close();
        // clean the json objects
        jsonCurrentLog.empty();
        jsonCurrentList.empty();
    }
}

void JSONOutput::write(const QString message, const QString owner,
                       const Level lvl, const QDateTime timestamp,
                       const QString functionName, const int lineNumber)
{

    if(!outputFile.isNull()
            && outputFile->isOpen()
            && (outputFile->size() > (configuration->getFileMaxSizeInKb() * 1024)))
    {
        close();
    }

    if(outputFile.isNull() //if there is no file
            || !outputFile->isOpen() // or the file is not opened for writing
            || (outputFile->size() > (configuration->getFileMaxSizeInKb() * 1024))) // or the file is already at max size
    {
        createNextFile(); // create a new file
    }

    QJsonObject json;
    json[JSON_LINE] = lineNumber;
    json[JSON_FUNCTION] = functionName;
    json[JSON_DATE_TIME] = timestamp.toString(configuration->getTimestampFormat());
    json[JSON_LEVEL] = levelToString(lvl);
    json[JSON_MESSAGE] = message;
    json[JSON_OWNER] = owner;
    jsonCurrentList.append(json);
}

}

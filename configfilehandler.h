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

#ifndef CONFIGFILEHANDLER_H
#define CONFIGFILEHANDLER_H

#include <QFileInfo>
#include <QDir>
#include <QMultiHash>
#include "configuration.h"

namespace qlogger
{

//! This class is responsible to search on program path for a configuration file
//! Then read and parse this configuration file to generate valid configurations from it
//!
//! The file must be a plain text file with the name qlogger.cfg
//! The following texts are valid on config file.
//!
//! # comment line example
//! owner.level = { FATAL, ERROR, WARN, INFO, DEBUG, TRACE }
//! owner.outputType = { CONSOLE, TEXT, XML }
//! owner.logMask =
//!    { for console and text can use the symbols %t %o %l %m , if does not contain %t %l and %m will use the default }
//! owner.maxFileSize = {in kb just a number 100, 1000... only for TEXT and XML}
//! owner.path = { a valid absolut path on the system, if invalid path is given then will default to app path }
//! owner.timestampFormat =
//!    { the Qt format for datetime used to format the %t part of console and text and date_time tag of XML, it defaults to platform short format }
//! owner.fileName = { file name mask, must contain all %1 %2 %3 params, example: log_%1_%2_%3.txt
//! owner.fileNameTimeStamp = { the timestamp that will be written in param %3 of the file name mask, must follow QTimeDate string format.

static const QString LEVEL = "level";

static const QString OUTPUT_TYPE = "outputType";

static const QString LOG_MASK = "logMask";

static const QString MAX_FILE_SIZE = "maxFileSize";

static const QString PATH = "path";

static const QString TIMESTAMP_FORMAT = "timestampFormat";

static const QString FILE_NAME = "fileName";

static const QString FILE_NAME_TIMESTAMP = "fileNameTimeStamp";

static const QString CONFIG_FILE_NAME = "qlogger.cfg";

class ConfigFileHandler
{

private:

    inline static void recursiveFolderSearch(QDir& dir, bool &found)
    {
        QDir::setCurrent(dir.absolutePath());
        QStringList list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < list.size(); ++i)
        {
            QString path = list.at(i);
            if(found)
            {
                return;
            }
            else if(QFile::exists(CONFIG_FILE_NAME))
            {
                found = true;
                return;
            }
            else
            {
                found = false;
                dir.cd(path);
                recursiveFolderSearch(dir, found);
                if(!found)
                    dir.cdUp();
            }
        }
    }

protected:

    inline static bool searchForConfigFile()
    {
        QDir dir(QCoreApplication::applicationDirPath());
        //search on the folder tree
        bool found = false;
        recursiveFolderSearch(dir, found);
        return found;
    }

    inline static bool createConfiguration(QMultiHash<QString, QString> &ownerConfigLines,
                                           QMultiHash<QString, Configuration*> &loggers)
    {
        QList<QString> configs = ownerConfigLines.keys();
        QString currentConfig = "";
        bool localTest = false; // the loggers reference could already been filled from somewhere else
        for(int i = 0 ; i < configs.size() ; i++)
        {
            QString config = configs.at(i);
            if(currentConfig != config)
            {
                QStringList values = ownerConfigLines.values(config);
                loggers.insert(config, parseAllLinesOfConfig(config, values));
                currentConfig = config;
                localTest = true;
            }
        }
        return localTest;
    }

    inline static Configuration* parseAllLinesOfConfig(QString owner, QStringList lines)
    {
        // parsing a structure like ( key=value ) * N
        Configuration::Level level = Configuration::q1ERROR;
        Configuration::OutputType outputType = Configuration::CONSOLE;
        QString logMask = "";
        int maxFileSize = 0;
        QString path = "";
        QString timestampFormat = "";
        QString fileName = "";
        QString fileNameTimestamp = "";
        foreach(QString line , lines)
        {
            QStringList configList = line.trimmed().split("=");
            QString key = configList.isEmpty() ? "" : configList.at(0); // left hand
            QString paramValue = configList.size() < 1 ? "" : configList.at(1); //right hand
            if(key == LEVEL)
            {
                level = Configuration::levelFromString(paramValue);
            }
            else if(key == OUTPUT_TYPE)
            {
                if(paramValue.toUpper().contains("TEXT"))
                {
                    outputType = Configuration::TEXTFILE;
                }
                else if(paramValue.toUpper().contains("XML"))
                {
                    outputType = Configuration::XMLFILE;
                }
                else
                {
                    outputType = Configuration::CONSOLE;
                }
            }
            else if(key == LOG_MASK)
            {
                logMask = paramValue;
            }
            else if(key == MAX_FILE_SIZE)
            {
                bool ok;
                maxFileSize = paramValue.toInt(&ok);
                if(!ok)
                    maxFileSize = 1000;
            }
            else if(key == PATH)
            {
                path = paramValue;
            }
            else if(key == TIMESTAMP_FORMAT)
            {
                timestampFormat = paramValue;
            }
            else if(key == FILE_NAME)
            {
                fileName = paramValue;
            }
            else if(key == FILE_NAME_TIMESTAMP)
            {
                fileNameTimestamp = paramValue;
            }
        }
        return new Configuration(owner, level, outputType, timestampFormat, logMask, fileName, fileNameTimestamp, path, maxFileSize);
    }

public:

    //! only public method, this will receive a hash by reference and fill it with found configurations
    //! false will be returned if no config is filled in
    inline static bool parseConfigurationFile(QMultiHash<QString, Configuration*> &loggers)
    {
        QMultiHash<QString, QString> ownerConfigLines;
        QFile* file = new QFile(CONFIG_FILE_NAME);
        if (ConfigFileHandler::searchForConfigFile() &&
                file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(file);
            QString configLine;
            do
            {
                configLine = ts.readLine();
                if(!configLine.trimmed().isEmpty() && !configLine.trimmed().startsWith("#"))
                {
                    QStringList temp = configLine.split("."); // separate the owner.key=value
                    QString owner = temp.isEmpty() ? "" : temp.at(0);
                    if(!owner.isEmpty() && temp.size() > 1)
                    {
                        ownerConfigLines.insert(owner, temp.at(1));
                    }
                }
            } while (!configLine.isNull());
            return createConfiguration(ownerConfigLines, loggers);
        }
        else
        {
            return false;
        }
    }

};

}

#endif // CONFIGFILEHANDLER_H
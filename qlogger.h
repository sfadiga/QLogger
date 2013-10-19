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

#ifndef QLOGGER_H
#define QLOGGER_H

#include <QAtomicPointer>
#include <QHash>
#include <QMutex>

#include "configuration.h"

namespace qlogger
{

class Configuration;
class QLogger;

//! This class is responsible for returning the memory of QLogger singleton and avoid mem leaks
class QLoggerDestroyer
{
public:
    QLoggerDestroyer(QAtomicPointer<QLogger> s = 0);
    ~QLoggerDestroyer();
    void SetSingleton(QAtomicPointer<QLogger> s);

private:
    QAtomicPointer<QLogger> singleton;

public:
	//! Handles the acess to QAtomicPointer methods based on Qt version
	inline QLogger* getPointerInstance()
	{
		#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
			return singleton;
		#else	
            return singleton.loadAcquire();
		#endif
	}

};

//! Main QLogger class, its a singleton responsible for register the log messages to
//! its respective owners and also load the runtime configurations wich defines how each
//! log message will be displayed.
class QLogger
{

public:
    //! retrives the global instance of the QLogger class, this is private so no ones need to call this directly
    static QLogger* getInstance();

protected:
    // allow the definition of the destroyer inside the singleton
    friend class QLoggerDestroyer;
    
	//! protected constructor to avoid instatiation outside the class
    QLogger();
    virtual ~QLogger();
	
	//!
	void close();

private:
    //! Singleton instance
    static QAtomicPointer<QLogger> instance;

    //! singleton creation mutex
    static QMutex mutex;

    //! log writing mutex
    static QMutex writex;

    //! this static instance is responsible for call the singleton destructor when program ends
    static QLoggerDestroyer destroyer;

	//! this methods adds a loger based on a configuration 
	static void addLogger(QString logOwner, Configuration* configuration);

private:
	//! Stop the compiler generating methods of copy the object
    QLogger(QLogger const& copy);            // Not Implemented
    QLogger& operator=(QLogger const& copy); // Not Implemented

	//! Handles the acess to QAtomicPointer methods based on Qt version
	static inline QLogger* getPointerInstance()
	{
		#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
			return instance;
		#else	
            return instance.loadAcquire();
		#endif
	}


private:
    //! logger added at runtime along with their configurations
    QMultiHash<QString, Configuration*> loggers;

public:

    //! adds a logger with a default configuration to the list of loggers in runtime
    static void addLogger(QString logOwner);

    static void addLogger(Configuration* configuration);

    //! adds a logger passing a configuration to the list of loggers in runtime
    static void addLogger(QString logOwner, Configuration::Level lvl,
                          Configuration::OutputType ouputType,
                          QString timestampFormat = TIMESTAMP_QLOGGER_FORMAT);

    //! log a message to a specific level
    static void log(Configuration::Level, QString message, QString functionName = QString(), int lineNumber = -1, QString owner = "root");

    //! logs directly to fatal level
    inline static void fatal(QString message, QString owner = "root", QString functionName = QString(), int lineNumber = -1)
    {
        QLogger::log(Configuration::q0FATAL, message, functionName, lineNumber, owner);
    }

    //! logs directly to error level
    inline static void error(QString message, QString owner = "root", QString functionName = QString(), int lineNumber = -1)
    {
        QLogger::log(Configuration::q1ERROR, message, functionName, lineNumber, owner);
    }

    //! logs directly to warn level
    inline static void warn(QString message, QString owner = "root", QString functionName = QString(), int lineNumber = -1)
    {
        QLogger::log(Configuration::q2WARN, message, functionName, lineNumber, owner);
    }

    //! logs directly to info level
    inline static void info(QString message, QString owner = "root", QString functionName = QString(), int lineNumber = -1)
    {
        QLogger::log(Configuration::q3INFO, message, functionName, lineNumber, owner);
    }

    //! logs directly to debug level
    inline static void debug(QString message, QString owner = "root", QString functionName = QString(), int lineNumber = -1)
    {
        QLogger::log(Configuration::q4DEBUG, message, functionName, lineNumber, owner);
    }

    //! logs directly to trace level
    inline static void trace(QString message, QString owner = "root", QString functionName = QString(), int lineNumber = -1)
    {
        QLogger::log(Configuration::q5TRACE, message, functionName, lineNumber, owner);
    }

};

// MACROS FOR THE PEOPLE!
#define QLOG_FATAL(message, ...) QLogger::log(Configuration::q0FATAL, message, __FUNCTION__ , __LINE__, ##__VA_ARGS__);
#define QLOG_ERROR(message, ...) QLogger::log(Configuration::q1ERROR, message, __FUNCTION__ , __LINE__ , ##__VA_ARGS__);
#define QLOG_WARN(message, ...) QLogger::log(Configuration::q2WARN, message, __FUNCTION__ , __LINE__ , ##__VA_ARGS__);
#define QLOG_INFO(message, ...) QLogger::log(Configuration::q3INFO, message, __FUNCTION__ , __LINE__ , ##__VA_ARGS__);
#define QLOG_DEBUG(message, ...) QLogger::log(Configuration::q4DEBUG, message, __FUNCTION__ , __LINE__ , ##__VA_ARGS__);
#define QLOG_TRACE(message, ...) QLogger::log(Configuration::q5TRACE, message, __FUNCTION__ , __LINE__ , ##__VA_ARGS__);

}



#endif // QLOGGER_H


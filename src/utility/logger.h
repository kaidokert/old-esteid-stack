#ifndef __LOGGER_H__
#define __LOGGER_H__

enum logTarget {
	log_to_CONSOLE,
	log_to_FILE,
	log_to_EVENTLOG,
};

#ifdef POCO_LOGGING
#include <Poco/Message.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/EventLogChannel.h>
#include "Poco/AutoPtr.h"

class logger : public Poco::LogStream {
	Poco::AutoPtr<Poco::ConsoleChannel> pCChannel;
	Poco::AutoPtr<Poco::EventLogChannel> pEChannel;
public:
	logger() : Poco::LogStream(Poco::Logger::root()) {
	  pCChannel = new Poco::ConsoleChannel();
	  Poco::Logger::root().setChannel(pCChannel);
	  }
	logger(logTarget target,std::string name) :
	  Poco::LogStream(Poco::Logger::root()) {
		if (target == log_to_CONSOLE) {
		  pCChannel = new Poco::ConsoleChannel();
		  Poco::Logger::root().setChannel(pCChannel);
		  }
		if (target == log_to_EVENTLOG) {
		  pEChannel = new Poco::EventLogChannel(name);
		  Poco::Logger::root().setChannel(pEChannel);
		  }
		}
  };
#else
class logger : public std::ostringstream {
public:
	logger() {}
	logger(logTarget target,std::string name) {}
  };
#endif

class ScopedLog {
    logger &m_log;
    const char *fn;
public:
    ScopedLog(const char *func_,logger & log) : m_log(log),fn(func_) {
      m_log << "-->function:" << fn << std::endl;
      }
    ~ScopedLog() {
      m_log << "<--function:" << fn << std::endl;
  }
};
#endif

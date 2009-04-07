#ifdef POCO_LOGGING
#include <Poco/Message.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/ConsoleChannel.h>
#include "Poco/AutoPtr.h"

class logger : public Poco::LogStream {
	Poco::AutoPtr<Poco::ConsoleChannel> pChannel;
public:
	logger() : Poco::LogStream(Poco::Logger::root()) {
	  pChannel = new Poco::ConsoleChannel();
	  Poco::Logger::root().setChannel(pChannel);
	  }
  };
#else
class logger : public std::ostream {
  };
#endif

#define FUNC_LOG() ScopedLog __l(__func__,d->log)

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


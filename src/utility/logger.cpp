/*!
	\file		logger.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include "logger.h"

class file_log_target : public log_target {
  std::ofstream file;
public:
  file_log_target(const std::string &name) : 
      file(std::string(name + ".log").c_str()),
      log_target(name) {}
  void writeLine(const std::string &line,logPrio prio) {
    if (file.good() ) 
      file << line << std::endl;
    }
};

class console_log_target : public log_target {
public:
  console_log_target(const std::string &name) : log_target(name) {}
  void writeLine(const std::string &line,logPrio prio) {
    std::cout << line << std::endl;
    }
};

#ifndef WIN32
#include <syslog.h>
class system_log_target : public log_target {
public:
  system_log_target(const std::string &name) : log_target(name) {
    openlog(name.c_str(),LOG_PID,LOG_USER);
    }
  ~system_log_target() {
    closelog();
    }
  void writeLine(const std::string &line,logPrio prio) {
    syslog(LOG_DEBUG,"%s",line.c_str());
    }
};

class window_log_target : public log_target {
public:
  window_log_target(const std::string &name)  : log_target(name) {}
  void writeLine(const std::string &line,logPrio prio) {
    }
};
#else

class system_log_target : public log_target {
public:
  system_log_target(const std::string &name) : log_target(name) {
    }
  ~system_log_target() {
    }
  void writeLine(const std::string &line,logPrio prio) {
    }
};

class window_log_target : public log_target {
public:
  window_log_target(const std::string &name)  : log_target(name) {}
  void writeLine(const std::string &line,logPrio prio) {
    }
};
#endif


log_streambuffer::log_streambuffer() {
  m_inputbuffer.reserve(160);
  };
log_streambuffer::~log_streambuffer() {
  while(targets.size()) { delete targets.back(); targets.pop_back();}
  }
int log_streambuffer::overflow(int p) {
  if (p == '\n') {
    m_inputbuffer.push_back('\0');
    for(std::vector<log_target *>::iterator i = targets.begin();i!=targets.end();i++)
      (*i)->writeLine(&m_inputbuffer[0]);
    m_inputbuffer.clear();
    }
  else
    m_inputbuffer.push_back(p);
  }

void logger::addTarget(logTarget target) {
  switch(target) {
    case log_to_CONSOLE : m_streambuffer.targets.push_back(new console_log_target(m_name));break;
    case log_to_FILE : m_streambuffer.targets.push_back(new file_log_target(m_name));break;
    case log_to_SYSTEMLOG : m_streambuffer.targets.push_back(new system_log_target(m_name));break;
    case log_to_WINDOW : m_streambuffer.targets.push_back(new window_log_target(m_name));
    }
  }

logger::logger(const std::string name) : 
      m_name(name),
      m_streambuffer(), std::ostream(&m_streambuffer) { 
  clear();
  addTarget(log_to_CONSOLE);
  addTarget(log_to_FILE);
  addTarget(log_to_SYSTEMLOG);
  }

logger::logger(const std::string name,logTarget target) : m_name(name),
    m_streambuffer(), std::ostream(&m_streambuffer) {
  clear();
  addTarget(target);
  }

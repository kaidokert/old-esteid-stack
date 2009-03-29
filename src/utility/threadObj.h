/*!
	\file		monitorThread.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2009-03-29 17:48:54 +0300 (Sun, 29 Mar 2009) $
*/

struct threadPriv;
struct mutexPriv;

class mutexObj {
    const char *m_name;
    mutexPriv *d;
public:
    mutexObj(const char *name);
    ~mutexObj();
	bool Lock();
	bool Unlock();
    };

class mutexObjLocker {
    mutexObj &mutex;
public:
    mutexObjLocker(mutexObj &ref) : mutex(ref)
     { mutex.Lock(); }
    ~mutexObjLocker()
     { mutex.Unlock(); }
    };

class threadObj {
    const char *m_name;
    threadPriv *d;
public:
    threadObj(const char *name);
    virtual ~threadObj();
    void start();
    void stop();
    virtual void execute() = 0;
    void wait(unsigned int milliseconds);
    };

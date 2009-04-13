/*!
	\file		threadObj.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

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
    virtual void start();
    virtual void stop();
    virtual void execute() = 0;
    static void wait(unsigned int milliseconds);
    };

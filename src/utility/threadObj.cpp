#include "threadObj.h"

#ifndef WIN32 //pthreads
#include <pthread.h>
struct mutexPriv {
	pthread_mutex_t m_mutex;
	mutexPriv() {
        pthread_mutex_init(&m_mutex,NULL);
        }
    ~mutexPriv() {
        pthread_mutex_destroy(&m_mutex);
        }
    bool Lock() {
       return  !pthread_mutex_lock(&m_mutex);
       }
    bool Unlock() {
        return pthread_mutex_unlock(&m_mutex);
        }
};
struct threadPriv {
    pthread_t m_id;
    threadObj &m_threadObj;
    static void * thread_entry(void * const thisPtr) {
        threadPriv * this_ = (threadPriv*) thisPtr;
        this_->m_threadObj.execute();
        }
    threadPriv(threadObj &ref) : m_threadObj(ref) {
        }
    void start() {
        pthread_create(&m_id,NULL,thread_entry,this);
       }
    void stop() {
        pthread_create(&m_id,NULL,thread_entry,this);
        }
    ~threadPriv() {
        stop();
        }
    void wait(unsigned int ms) {
        timespec timesp;
        timesp.tv_sec =(time_t) (ms/ 1000);
        timesp.tv_nsec=(time_t) ((ms% 1000) * 1000 * 1000);
        nanosleep(&timesp,NULL);
        }
};
#else
#include <windows.h>
struct threadPriv {
	void *cs;
};
struct mutexPriv {
};
#endif

mutexObj::mutexObj(const char *name) : m_name(name) , d(new mutexPriv){}
mutexObj::~mutexObj() {delete d;}
bool mutexObj::Lock() {return d->Lock();}
bool mutexObj::Unlock() {return d->Unlock();}

threadObj::threadObj(const char *name) : m_name(name), d(new threadPriv(*this)) {}
threadObj::~threadObj() {delete d;}
void threadObj::wait(unsigned int ms) {d->wait(ms);}
void threadObj::start() {d->start();}
void threadObj::stop() {d->stop();}


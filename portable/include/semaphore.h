#pragma once
#include <stdlib.h>
#include <stdio.h>

#ifdef _POSIX
#include <pthread.h>
#endif
#ifdef _WIN32
#	include <windows.h>
#endif

class Mutex {
public:
#ifdef _POSIX
	pthread_mutex_t sys_mutex;
#endif
#ifdef _WIN32
	CRITICAL_SECTION m_csCriticalSection;
#endif
public:
	Mutex();
	~Mutex();
	void Lock();
	void Unlock();
};

class semaphore_i
{
public:
	semaphore_i() {};
    virtual ~semaphore_i() {};
    virtual void wait() = 0;
    virtual void signal() = 0;
};

//TODO DAWD: Not convinced about implementation of semaphore in linux version...
// Behaviour: First wait always get though....maybe this is how it's meant to be
// Signalling next thread....does not work if a thread has already locked the mutex

class semaphore : public semaphore_i
{
private:
	int				v;
#ifdef _POSIX
	pthread_cond_t	sema;
#endif
#ifdef _WIN32
	HANDLE				m_hEvent;
	CRITICAL_SECTION	m_csCriticalSection;
	CONDITION_VARIABLE	m_cndVar;
#endif
	Mutex			mutex;

 public:

	semaphore();
	~semaphore();

//#ifdef _WIN32
//	bool UseLock();
//#endif
	void wait();
	void signal();
};




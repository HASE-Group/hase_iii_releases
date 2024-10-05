//---------------------------------------------------------------------------
#ifndef threadsH
#define threadsH
//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _POSIX
#include <pthread.h>
#endif
#ifdef _WIN32
#	include <Windows.h>
#endif

#include "simkernel.h"

class sim_entity;
extern void entity_thread(sim_entity *e);

typedef void* (*PF)(void*);
class thread_i {
public:
  thread_i(PF /*fn*/, void* /*arg*/) {};
  thread_i() {};
  virtual ~thread_i() {};
  virtual int is_running() = 0;
};

void init_threads();

class h_thread : public thread_i
{
private:
	bool error;
protected:
#ifdef _POSIX
	static void* Execute(void *ht)
	{
		entity_thread( ( (h_thread *)ht )->ent);
		return 0;
	}
#endif
public:
	sim_entity			*ent;
#ifdef _POSIX
	pthread_t			id;
#endif
#ifdef _WIN32
	HANDLE				m_hThread;
#endif

	h_thread(sim_entity *e);
	bool hasError() { return error; }

	int is_running()
	{
#ifdef _POSIX
		return pthread_equal(pthread_self(),id);
#endif
#ifdef _WIN32
		return m_hThread != NULL;
#endif
	}

	void Terminate();
	~h_thread() {}
};


#endif


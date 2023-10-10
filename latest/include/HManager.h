//---------------------------------------------------------------------------
#ifndef HManagerH
#define HManagerH
//---------------------------------------------------------------------------

#include <map>
#include <vector>
#include "entity.h"
#include "evqueue.h"
#ifdef _POSIX
#include <sys/time.h>
#endif
#ifdef _WIN32
#	include <windows.h>
#endif
#include <sstream>
#include <fstream>
#include "HProject.h"

//using namespace std;


typedef struct trace_list_str trace_list_str;

class HProject;

class HManager : public HAbstractManager
{
	bool            verbose;
	bool            animationOn;
	int             ecount;
	int             nentities;
	int             traceEntrySize;
	HProject        project;

protected:
	int trace_index;
	std::vector<sim_entity*>entities;
	std::vector<h_thread*>thrds;
	evqueue future, deferred;
	semaphore flag, oflag, onestopped, spflag;
	//semaphore onestopped;
	//Mutex flag;
	//Mutex oflag;
	//Mutex spflag;
	sim_time now;
	int trc_level;
	int is_running;

	struct trace_list_str* future_trace[1000];


public:

	HManager();

	virtual ~HManager()
	{
		std::vector<h_thread*>::iterator it;
		for(it=thrds.begin(); it!=thrds.end(); it++)
			delete*it;
		thrds.clear();
		entities.clear();
	}

#ifdef _WIN32
	LARGE_INTEGER	m_nFrequency;
	LARGE_INTEGER	m_nBaseValue;
#endif


	void setverbose(bool b);
	void reset();
	int get_nentities();
	void add_entity(sim_entity*e);
	void add_entity_dynamically(sim_entity*e);
	int running();
	sim_entity_id get_entity_id(const char*ename);
	sim_entity* current_ent();
	sim_entity* get_entity(sim_entity_id e);
	sim_time sim_clock();
	void set_trc_level(int t);
	int get_trc_level();
	bool mk_thread(sim_entity_id i);
	void mk_threads();
	void putback(sim_event&ev);
	void paused();
	void run_start();
	int run_until(double t);
	int run_tick();
	void run_runnables();
	void run_single_tick();
	void pop_event();
	void run(const char* animationFile);
	void startup();
	void report();
	void send(sim_entity_id src, sim_entity_id dest, sim_time delay, int tag, void*data, int ndata, int src_porti=-1);
	void wait(sim_entity_id src);
	void sim_printf(sim_entity_id src, char*msg, va_list va);
	void trace(char eventType, int stage, sim_entity_id src, const char* msg, va_list va);
	void trace(char eventType, int stage, sim_entity_id src, const char* msg);
	void trace(char eventType, int stage, sim_entity_id src, sim_time delay, const char* msg);
	void hold(sim_entity_id src, sim_time delay);
	int waiting(sim_entity_id src, sim_predicate&p);
	void select(sim_entity_id src, sim_predicate&p);
	int cancel(sim_entity_id src, sim_predicate&p);
};
#endif

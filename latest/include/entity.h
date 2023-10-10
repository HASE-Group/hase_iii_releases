#pragma once

#include <vector>
#include "kernel.h"

enum estate {
	RUNNABLE,	// Ready to run
    WAITING,	// Waiting for an event
    HOLDING,	// Holding for a time
    FINISHED,	// Completed
    TERMINATED	// Thread about to be destroyed
};

class entity
{
  estate				state;
  sim_entity_id			me;
  sim_event				evbuf;
  sim_predicate			*pred;
  semaphore				restart;
  h_thread				*thr;
  char					*name;
  std::vector<sim_port*>		ports;
public:
    entity(const char *n, sim_entity_id e=0);
	virtual ~entity()   {   delete [] name;    }
	const char* get_name() {return name;}
    estate get_state() {return state;}
    void set_state(estate e) {state = e;}
    void set_id(sim_entity_id ei) {me = ei;}
    sim_entity_id get_id() {return me;}
    void set_thread(h_thread *t) {thr = t;}
    void set_evbuf(sim_event e) {evbuf = e;}
    sim_event get_evbuf() {return evbuf;}
    void set_going() { restart.signal();}
    void join_port(sim_port &p, const char* pname);
    sim_port *get_port(sim_event &ev);
	sim_port *get_port(const char* pname);

    void send(sim_entity_id dest, sim_time delay, int tag, void *data, int ndata, int src_porti =-1)
	{
    	S->send(me,dest,delay,tag,data,ndata,src_porti);
    }

    void sim_printf(char *msg, va_list va)
	{
    	S->sim_printf(me,msg,va);
    }

	void trace(char eventType, int stage, const char *msg)
	{
    	S->trace(eventType, stage, me,msg);
    }

	void trace(char eventType, int stage, const char *msg, va_list va)
	{
    	S->trace(eventType, stage, me,msg,va);
    }

	void trace(char eventType, int stage, const char *msg, sim_time delay)
	{
    	S->trace(eventType, stage, me,delay,msg);
    }

	void restart_wait()
	{
    	restart.wait();
    }

	void hold(sim_time delay)
	{
    	S->hold(me,delay);
        S->paused();
        restart_wait();
    }

	bool isWaiting(sim_event &e);

    void wait(sim_predicate *p = 0)
	{
      pred=p;
      S->wait(me);
      S->paused();
      restart_wait();
    }

	int waiting(sim_predicate &p)
	{
    	return S->waiting(me,p);
    }

    void select(sim_predicate &p)
	{
    	S->select(me,p);
    }

    void done()
	{
    	set_state(FINISHED);
        for(;; S->paused(), restart_wait());
    }

    void start(sim_entity *e);
};

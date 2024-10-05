#pragma once
#include "simkernel.h"

class sim_event;
class sim_predicate;

class HAbstractManager
{
public:
	virtual ~HAbstractManager() { };
	virtual void reset()=0;
	virtual int get_nentities()=0;
	virtual int get_trc_level()=0;
	virtual void set_trc_level(int)=0;
	virtual void run(const char*)=0;
	virtual void run_start()=0;
	virtual int run_tick()=0;
	virtual int run_until(double)=0;
	virtual void mk_threads()=0;
	virtual void add_entity(sim_entity*e)=0;
	virtual sim_entity_id get_entity_id(const char*ename)=0;
	//virtual sim_entity*current_ent()=0;
	virtual sim_entity*get_entity(sim_entity_id)=0;
	virtual void paused()=0;
	virtual void send(sim_entity_id, sim_entity_id, sim_time, int, void*, int, int=-1)=0;
	virtual void trace(char, int, sim_entity_id, const char*)=0;
	virtual void trace(char, int, sim_entity_id, sim_time, const char*)=0;
	virtual void trace(char, int, sim_entity_id, const char*, va_list)=0;
	virtual void sim_printf(sim_entity_id, char*, va_list)=0;
	virtual void hold(sim_entity_id, sim_time)=0;
	virtual void wait(sim_entity_id)=0;
	virtual int waiting(sim_entity_id, sim_predicate&)=0;
	virtual void select(sim_entity_id, sim_predicate&)=0;
	virtual void putback(sim_event&ev)=0;
	virtual sim_time sim_clock()=0;
	virtual void setverbose(bool b)=0;
	virtual int cancel(sim_entity_id, sim_predicate&)=0;
	void writeTrace(void);
};

extern HAbstractManager *S;

void newHManager();

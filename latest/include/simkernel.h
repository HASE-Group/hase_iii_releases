#pragma once

#include <string.h>
#include <stdio.h>
#include <iostream>

//using namespace std;

//Internal event type
enum evtype {ENULL, SEND, HOLD_DONE, CREATE};

// Macro
extern void* hase_memdup(void*, size_t);


#define SIM_PUT(type,var) new type(var)

#define SIM_CAST(type,var,ev) {type* tmp=(type*)ev.get_data();var=*tmp; }
#define SIM_CAST_DEL(type,var,ev) {type* tmp=(type*)ev.get_data();var=*tmp; delete tmp; ev.data=0;}
#define SIM_GET(type,var,ev) {type* tmp=(type*)ev.get_data();var=*tmp;tmp->~type(); delete tmp;}

#include "sim_port.h"
#include "sim_event.h"
#include "sim_predicate.h"

extern sim_event EMPTY_EVENT;
const int POISON_TAG= -999;

// Event selection predicates
class sim_or : public sim_predicate {
  sim_predicate *p1, *p2;
 public:
  sim_or(sim_predicate &p1, sim_predicate &p2) : p1(&p1), p2(&p2) { tester = 9; }
  int match(sim_event &ev) { if (p1->match(ev)) return true; return p2->match(ev); }
};

class  sim_any_p : public sim_predicate {
public:
  sim_any_p() {tester=1;};
  int match(sim_event& /*ev*/) { return 1; }
};

extern sim_any_p SIM_ANY;

class  sim_none_p : public sim_predicate {
public:
	sim_none_p() {tester=2;};
    int match(sim_event& /*ev*/) { return 0; }
};

extern sim_none_p SIM_NONE;

class  sim_from_p : public sim_predicate {
	sim_entity_id src_e;
public:
	sim_from_p(sim_entity_id s) : src_e(s) {tester=3;};
    int match(sim_event &ev) { return (ev.get_src()==src_e); }
};

class  sim_type_p : public sim_predicate {
	int tag1,tag2,tag3,tag4;
    int ntags;
public:
	sim_type_p(int t1) : tag1(t1) {ntags = 1;tester=4;};
    sim_type_p(int t1, int t2) : tag1(t1),tag2(t2) {ntags = 2;tester=5;};
    sim_type_p(int t1, int t2, int t3) : tag1(t1), tag2(t2), tag3(t3) {ntags = 3;tester=6;};
    sim_type_p(int t1, int t2, int t3, int t4) : tag1(t1), tag2(t2), tag3(t3), tag4(t4) {ntags = 4;tester=7;};
    int match(sim_event &ev) {
    	switch (ntags) {
        	case 1: return (ev.get_tag()==tag1);
        	case 2: return (ev.get_tag()==tag1) || (ev.get_tag()==tag2);
        	case 3: return (ev.get_tag()==tag1) || (ev.get_tag()==tag2) || (ev.get_tag()==tag3);
        	case 4: return (ev.get_tag()==tag1) || (ev.get_tag()==tag2) || (ev.get_tag()==tag3) || (ev.get_tag()==tag4);
        }
        return 0;
    }
};

// sim_from_port added by F.Mallet to preserve encapsulation : 15/10/2001
class sim_from_port : public sim_predicate
{
	//sim_port& port;
	sim_port* port;
public:
	sim_from_port(sim_port &p)// : port(p)
	{
		port = &p;
		tester=8;
	}

	int match(sim_event &ev)
	{
	  return (ev.get_src()==port->get_dest() && (ev.get_srcport()==port->get_desti()));
	}
};

//  Class sim_entity
class entity;

class sim_entity {
	entity *rep;
public:
	// Initialisation
	sim_entity(const char *name);
    virtual ~sim_entity();
    entity* get_rep() { return rep; }
    void join_port(sim_port &p, const char* name);
    const char *get_name();
    sim_entity_id get_id();

    // Run time methods
    void sim_printf(char* msg ...);
    void sim_trace(char eventType, int stage, int level, const char *msg ...);
    void sim_trace(char eventType, int stage, int level, sim_time delay, const char* msg);
    void sim_schedule(sim_port &dest, sim_time delay, int ev_type, void *ev_buf=0, int ev_len=0);
    void sim_schedule(sim_entity_id dest, sim_time delay, int ev_type, void *ev_buf=0,int ev_len=0);
    void sim_reply(sim_event &ev, sim_time delay, void *ev_buf=0, int ev_len=0);
    int sim_waiting(sim_predicate &p=SIM_ANY);
    int sim_waiting(sim_event &ev, sim_predicate &p=SIM_ANY);
    void sim_select(sim_predicate &p, sim_event &ev);
    void sim_wait(sim_event &ev);
    void sim_wait_for(sim_predicate &p, sim_event &ev);
    void sim_get_next(sim_predicate &p, sim_event &ev);
// @return true si get an event that matches p before delay, false otherwise
    bool sim_get_next_before(sim_predicate &p, sim_event &ev, sim_time delay);
    bool sim_get_next_before(sim_event &ev, sim_time delay);
    void sim_get_next(sim_event &ev);
    int sim_cancel(sim_predicate &p);
    void sim_putback(sim_event &ev);
    void sim_hold(sim_time delay);
    sim_entity_id sim_current();
    void send_on(sim_event &ev, sim_port &p);
    sim_port* get_port(sim_event &ev);
    sim_port* get_port(const char *pname);
    virtual void body();
    virtual void startup();
    virtual void report();
    virtual void readParams(std::istream &is); // used in experiment mode
};

// sim system object
class  sim_system {
public:
   	sim_system();
    ~sim_system();
    void set_trc_level(int);
    void set_auto_trace(int);
    void add(sim_entity *e);
    void link_ports(sim_entity *e1, const char *p1, sim_entity *e2, const char *p2);
    void link_ports(const char *e1, const char *p1, const char *e2, const char *p2);
    void run(const char *projectName);
    sim_entity_id get_entity_id( const char *name);
    sim_entity* get_entity(sim_entity_id ei);
    //sim_entity* current_ent();
    int get_num_entities();
    sim_time sim_clock();
    void setverbose(bool b);
};

sim_time sim_clock();




//---------------------------------------------------------------------------
#ifndef sim_eventH
#define sim_eventH
//---------------------------------------------------------------------------
#include "sim_types.h"

class sim_event {
  sim_time time;
  int src_port;
  sim_entity_id src, dest;
  int tag;
  int dsize;

 public:  
  evtype etype;
  void *data;
  sim_event(evtype et=ENULL, sim_time t=0.0, sim_entity_id s=-1, sim_entity_id d=-1, int etag=0,void *edata=0, int edsize=0, int src_porti=-1);
  ~sim_event();
  sim_event *clone();
  void set_src(sim_entity_id s) { src = s; }
  void set_srcport(int s) {src_port = s; }
  void set_dest(sim_entity_id d) { dest = d; }
  sim_entity_id get_dest() { return dest; }
  sim_entity_id get_src() { return src; }
  int get_srcport() { return src_port; }
  sim_time event_time() { return time; }
  evtype get_type() { return etype; }
  int type() { return tag;}
  sim_entity_id scheduled_by() { return src;}
  int get_tag() { return tag;}
  void* get_data() { return data; }
  int get_dsize() { return dsize; }
  int from_port(sim_port &p);
};

#endif

#pragma once

typedef double sim_time;
typedef int sim_entity_id;
class sim_port;

//global sim_schedule function
extern "C" {
void sim_schedule(sim_port &p, sim_time delay, int type, void *ev_buf=0, int ev_len=0);
}

//
// File		Clocked.h
// Date:	June 27th, 2002
// Author:	Frederic Mallet

// Single phase Clocked entity
// see Clock, BiClocked, Pll

#ifndef __CLOCKED_H__
#define __CLOCKED_H__


#include <simkernel.h>         // Hase++

class Clocked : public sim_entity {
  int *__packet; // packet received from clock (value and status : 7/03/03)

public :
  char clockName[100];

int STATUS;
//entity references
  sim_entity_id __clk;

  void stopSimulation(int val=0);

  Clocked(const char *n, const char *cName = "CLOCK");

  virtual void dumpState(int stage, char type) = 0;
  void dump_state(int stage=0);
  void dump_state_i(int stage=0);
  virtual void startup();
  virtual void report() = 0;
 // services.
  virtual void pre() = 0;
  virtual void tick(int no) = 0;

  // can be used by child to get the status information
  virtual void status(int /*status*/) { /* nothing */ }

// body.
  void body();
};

#endif

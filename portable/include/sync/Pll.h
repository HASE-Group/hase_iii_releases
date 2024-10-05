#ifndef __PLL_H__
#define __PLL_H__

#include <sync/Clock.h>
#include <sync/Clocked.h>

class Pll : public Clock {
 private:
  sim_entity_id clk;
  int *__packet; // to contains the 2 ints sent by the clock

 public:
  int ratio;
  int pllCycle, mainCycle;
  char clockName[20];

  // cycle (out), dCycle(out) should not be here
  Pll(char *n, int cycle_i=0, int ratio_i=3, int pllCycle_i=0, const char *clockN="CLOCK");

  void dumpState(int stage, char type);
  void startup();
  void body();
};


#endif

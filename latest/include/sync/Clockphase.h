//
// File		Clockphase.h
// Date:	October 10th, 2002
// Author:	Frederic Mallet

// spies the clock phases
// This entity is used when using the clock mechanism
// and needing to have an entity that displays phases
// and the divided cycle number

#ifndef __CLOCKPHASE_H__
#define __CLOCKPHASE_H__

#include <sync/Biclocked.h>

class Clockphase : public Biclocked {
  int cycle;
public :
  Clockphase(const char *n, const char* clockName_i="CLOCK", int cycle_i=0);
  void dumpState(int stage, char type);

  void report() { } // must implement this abstract method from Clocked
  void pre();
  void phase0(int no);
  void phase1(int no);
 private:
  static const char* state_names[];
  static char S, P0, P1;
  char my_state;
};

#endif

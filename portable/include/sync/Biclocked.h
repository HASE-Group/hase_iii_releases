//
// File		Biclocked.h
// Date:	June 27th, 2002
// Author:	Frederic Mallet

// Double phase Clocked entity 
// see Clock, BiClocked, Pll

#ifndef __BICLOCKED_H__
#define __BICLOCKED_H__

#include <sync/Clocked.h>

class Biclocked : public Clocked {
public :
  Biclocked(const char *n, const char *cName = "CLOCK");
 void tick(int no);
 virtual void phase0(int no) = 0;
 virtual void phase1(int no) = 0;
};

#endif

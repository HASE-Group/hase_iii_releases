//
// File		BiclockedD.h
// Date:	September 4th, 2002
// Author:	Frederic Mallet

// Identical to Biclocked but displays a debug message
// Double phase Clocked entity 
// see Clock, BiClocked, Pll

#ifndef __BICLOCKEDD_H__
#define __BICLOCKEDD_H__

#include <sync/Clocked.h>

class BiclockedD : public Clocked {
public :
  BiclockedD(const char *n);
 void tick(int no);
 virtual void phase0(int no) = 0;
 virtual void phase1(int no) = 0;
};

#endif

#ifndef _SIM_PREDICATE_H_
#define _SIM_PREDICATE_H_

class  sim_predicate {
 protected:
  int tester;
 public:
  virtual int match(sim_event &ev)=0;
};

#endif

#ifndef __INSTR_ARRAY_HH_
#define __INSTR_ARRAY_HH_

// Author: F.Mallet
// Date: 7/11/2002
#include "HaseArray.h"
#include <vector>
class Label {
  std::string label;
  int pos;
 public:
  Label(const std::string& label, int pos) : label(label), pos(pos) { }
  bool equals(const char *l) const;
  int getPos() const { return pos; }
};

class HaseInstrArray : public HaseArray{
  std::vector<Label *> labels;
 protected:
  virtual bool treatLine(const std::string& line, int& pos);
 public:
  HaseInstrArray();
  HaseInstrArray(int size, char *name);

  int process(char *label, int offset = 0);
};
#endif

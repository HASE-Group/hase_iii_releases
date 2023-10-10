#ifndef __HASE_ARRAY_H__
#define __HASE_ARRAY_H__

// Author: F.Mallet
// Date: 31/10/2002
// --- Arrays which are generated for experiment (by JLinuxHase)
// --- will inherit this class or HaseInstrArray
#include <string>
//using namespace std;

class HaseArray {
  static int lastSection;
  int readLines(const std::string& filename);
 protected:
  int section, max;
  bool check(int index) const;
  std::string instName;
  virtual bool treatLine(const std::string& line, int& pos);
 public:
  HaseArray();
  HaseArray(int size, const std::string& name);
  int getSize() const;
  int readMemFile(const char *entity_name);
};
#endif

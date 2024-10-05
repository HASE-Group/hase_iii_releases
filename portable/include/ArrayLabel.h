#ifndef __ARRAY_LABEL_HH_
#define __ARRAY_LABEL_HH_

class ArrayLabel {
  char **labels;
  int maxLabel;

 public:
  ArrayLabel();
  int process(char *label, int offset = 0);
  void setLabels(char **val, int max);
  ~ArrayLabel();
};
#endif

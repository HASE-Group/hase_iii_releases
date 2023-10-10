#ifndef __MEMORY_CACHE_HH_
#define __MEMORY_CACHE_HH_

#include <vector>
//using namespace std;
#include "Type.h"
#include "Array.h"

class MemoryCache {
  static std::vector<MemoryCache*> list;
  char* filename;
  Array* a;
public:
  MemoryCache(const char* name, Array* array);
  ~MemoryCache();
  
  Array* shiftAndGetArray(); // shift the array to the next section and return the next section
  Array* getArray();
  int getSize();

  static MemoryCache* find(const char* name);
  static void add(MemoryCache* m);
  static void clear();
};

#endif

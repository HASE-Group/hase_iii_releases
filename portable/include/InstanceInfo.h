#pragma once
#include <vector>
#include <string>
//using namespace std;

class InstanceInfo {
  static std::vector<InstanceInfo *> list; 
  std::string name;
  char *fileName;
 public:
  const char* iName;
  const char* cName;
  const char* piName;
  const char* pcName;
  InstanceInfo(const std::string& Name, const char *iName, const char *cName, const char *piName=0, const char *pcName=0);
  void setFileName(const std::string& name);
  void setFileName(char *name);
  const char *getFileName() const;
  static void add(InstanceInfo *inf);
  static InstanceInfo& find(const char *name);
  static void clearDB();
  ~InstanceInfo();
};

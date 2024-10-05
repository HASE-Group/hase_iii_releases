#pragma once
// #include "Type.h" // include by Array
#include "Array.h"

void registering(const char *name, const char *iName, const char *cName, const char *piName=0, const char *pcName=0);

int readMemory(const char *entity_name, Array &a);
int readArray(const char *filename, Array *a);

void clearArrayDB();

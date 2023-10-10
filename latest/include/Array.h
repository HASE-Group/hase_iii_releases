#ifndef __ARRAY_HH_
#define __ARRAY_HH_

#include "Type.h"

class Array
{
	int noSec; // Section number (first = 0)
	int size, count;
	Type* m_pRef; // Type of all elements
	Type **tab;
	char **labels;
  
	Array *refFore, *refBack; // Allow sharing information withou copy

	Array *nextsection; // allow for multiple sections.
public:
	const char *name;
	bool hasLabel; // true if must parse labels
	// pre: data contains size not null Type or data == 0.
	// post: data are copied to tab
	Array(Array *other, int size = -1, bool copy = false); // pre: other != 0
	Array(const char *name, int size, Type& rRef, bool hasLabel = false, Type *data[] = 0, int nSec = -1);

	Array *createSection(int realSize = -1);
	Array *getNextSection();
	int getNoSection(); // Get the section number starting with 0

	int getSize();
	void setRealSize(int s); // Can only reduce the size
	const char *getName();
	Type *get(int i);
	char *toString(int i, char *buf, const Filter *f = 0);
	bool set(Type *val, int i = -1);
	bool set(char *val, int i = -1);
	bool addLabel(char *label, int i); // duplicate label memory
	void copyTo(Array &a); // only Min(a.size, this->size) element are copied 
	char **getAndRemoveLabels(); // will not delete the memory on destruction.
	~Array();
};

#endif

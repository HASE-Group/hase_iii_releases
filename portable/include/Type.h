#ifndef __TYPE_HH_
#define __TYPE_HH_

//#include <type_traits>

#ifdef _MSC_VER
typedef __int8				I8;
typedef __int16				I16;
typedef __int32				I32;
typedef __int64				I64;
typedef unsigned __int8		U8;
typedef unsigned __int16	U16;
typedef unsigned __int32	U32;
typedef unsigned __int64	U64;
typedef float				F32;
typedef double				F64;
typedef char			 	C8;
typedef wchar_t				CW;
#if defined(_M_AMD64) || defined(_M_X64 )
typedef unsigned __int64    UPTR;
typedef __int64             IPTR;
#else
typedef unsigned __int32	UPTR;
typedef __int32             IPTR;
#endif
#else
typedef __SIZE_TYPE__       size_t;
typedef char				I8;
typedef short				I16;
typedef int 				I32;
typedef long long       	I64;
typedef unsigned char		U8;
typedef unsigned short		U16;
typedef unsigned int		U32;
typedef unsigned long long  U64;
typedef float				F32;
typedef double				F64;
typedef char			 	C8;
typedef wchar_t			 	CW;
typedef __UINTPTR_TYPE__	UPTR;
typedef __INTPTR_TYPE__		IPTR;
#endif

#ifdef _NEW_CHECKS
static_assert(sizeof(F32) == 4, "Could not set F32 to size 4 bytes.");
static_assert(sizeof(F64) == 8, "Could not set F64 to size 8 bytes.");

static_assert(sizeof(U8) == 1, "Could not set U8 to size 1 byte.");
static_assert(sizeof(U16) == 2, "Could not set U8 to size 2 bytes.");
static_assert(sizeof(U32) == 4, "Could not set U8 to size 4 bytes.");

static_assert(sizeof(I8) == 1, "Could not set I8 to size 1 byte.");
static_assert(sizeof(I16) == 2, "Could not set I8 to size 2 bytes.");
static_assert(sizeof(I32) == 4, "Could not set I8 to size 4 bytes.");

static_assert(sizeof(C8) == 1, "Could not set C8 to size 1 byte.");

static_assert(sizeof(UPTR) == sizeof(void*), "Could not set UPTR to size of a pointer.");
static_assert(sizeof(IPTR) == sizeof(void*), "Could not set IPTR to size of a pointer.");
#endif


class Filter {
public:
  virtual char filter(char c) const = 0;
  virtual char filterBack(char c) const = 0;
};
class ReplaceFilter : public Filter {
  char old, nouveau;
public:
  ReplaceFilter(char old, char other) : old(old), nouveau(other) {}
  inline char filter(char c) const { return c==old?nouveau:c; }
  inline char filterBack(char c) const { return c==nouveau?old:c; }
};

class Type {
public:
	virtual ~Type(){};
	virtual Type *fromString(const char *v, int *len) = 0;
	virtual char *toString(char *v, const Filter *f = 0) const = 0;
	virtual Type *copy() = 0;
};




class Int : public Type {
  int a;
public:
  Int(int v);
  Type *fromString(const char *v, int *len);
  char *toString (char *v, const Filter *f = 0) const;
  int intValue();
  Type *copy();
};

class Long : public Type {
	I64 a;
public:
  Long(I64 v);
  Type *fromString(const char *v, int *len);
  char *toString (char *v, const Filter *f = 0) const;
  I64 longValue();
  Type *copy();
};

class UInt : public Type {
  unsigned int a;
public:
  UInt(unsigned int v);
  Type *fromString(const char *v, int *len);
  char *toString (char *v, const Filter *f = 0) const;
  unsigned int uintValue();
  Type *copy();
};

class Char : public Type {
  char c;
public:
  Char(char c);
  Type *fromString(const char *v, int *len);
  char *toString(char *v, const Filter *f = 0) const;
  Type *copy();
};

class Float : public Type {
  double d;
public:
  Float(double d);
  Type *fromString(const char *v, int *len);
  char *toString(char *v, const Filter *f = 0) const;
  double doubleValue();
  Type *copy();
};

class String : public Type {
  char * val;
  char sep;
public:
  String(const char *v, char sep = '\n');
  String(const char *v, char sep, int len); // max len character are copied.
  Type *fromString(const char *v, int *len);
  char *toString(char *v, const Filter *f = 0) const;
  const char * toString() const;
  Type *copy();
  ~String();
};

class Struct : public Type {
  Type **list;
  int nb, compulsory;
public:
  Struct(Type *l[], int nb, int compulsory = -1);
  Type *fromString(const char *v, int *len);
  char *toString(char *v, const Filter *f = 0) const;
  char *toString(int field, char *v, const Filter *f = 0) const;
  Type *copy();
  int getFieldNo() { return nb; }
  Type *getField(int no) const { return no<0||no>=nb?0:list[no]; }
  ~Struct();
};

class Enum : public Type {
protected:
  char **names;
  int num, count, current;
  bool caseSens;
public:
  Enum(int size, bool caseSens = true);
  virtual bool addField(const char *name);
  virtual Type *fromString(const char *v, int *len);
  virtual char *toString(char *v, const Filter *f = 0) const;
  const char *getName(int num = -1);
  int getNum() { return current; }
  virtual Type *copy();
  ~Enum();
};

class Instr : public Enum {
  Type **list;
public:
  Instr(int size);
  bool addField(const char *name);
  void addField(const char *name, Type *t);
  Type *fromString(const char *v, int *len);
  char *toString(char *v, const Filter *f = 0) const;
  const Type *getType(int num=-1);
  Type *copy();
  ~Instr();
};

class Coordinate : public Type {
  int *list;
  int dim;
  int static val;
public:
  Coordinate(int dim=2);
  Type *fromString(const char *v, int *len);
  char *toString(char *v, const Filter *f = 0) const;
  int getField(int pos) const;
  Type *copy();
  ~Coordinate();
};

#endif


#ifndef _TYPES_H_
#define _TYPES_H_

////////////////////////////////////////////////////////////////////////////////

typedef enum { FALSE=0, TRUE=1 } BOOLEAN;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

////////////////////////////////////////////////////////////////////////////////

typedef union MISCWORD {
   WORD w;
   struct {BYTE l,h;} b;
} MISCWORD;

typedef union MISCLONG {
   DWORD l;
   struct {WORD h,l;} w;
   BYTE b[4];
} MISCLONG;

////////////////////////////////////////////////////////////////////////////////

#define BITSET(name,bitn)  ((name)|=(1<<(bitn)))
#define BITCLR(name,bitn)  ((name)&=~(1<<(bitn)))
#define BITTST(name,bitn)  ((unsigned char) name & (1<<(bitn)))
#define BITNOT(name,bitn)  ((name)^=(1<<(bitn)))

////////////////////////////////////////////////////////////////////////////////

#endif


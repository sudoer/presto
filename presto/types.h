
#ifndef _TYPES_H_
#define _TYPES_H_

////////////////////////////////////////////////////////////////////////////////

#define NULL 0

////////////////////////////////////////////////////////////////////////////////

// INTEGER TYPES

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;
typedef signed char sint8;
typedef signed short sint16;
typedef signed long sint32;
typedef signed long long sint64;

////////////////////////////////////////////////////////////////////////////////

// BIT TYPES

typedef enum { FALSE=0, TRUE=1 } BOOLEAN;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

////////////////////////////////////////////////////////////////////////////////

// UNION BIT TYPES

typedef union MISCWORD {
   WORD w;
   struct {BYTE h,l;} b;
} MISCWORD;

typedef union MISCLONG {
   DWORD l;
   struct {WORD h,l;} w;
   BYTE b[4];
} MISCLONG;

////////////////////////////////////////////////////////////////////////////////

// BITWISE OPERATIONS

#define BITSET(name,bitn)  ((unsigned char)(name)|=(0x01<<(bitn)))
#define BITCLR(name,bitn)  ((unsigned char)(name)&=~(0x01<<(bitn)))
#define BITTST(name,bitn)  ((unsigned char)(name) & (0x01<<(bitn)))
#define BITNOT(name,bitn)  ((unsigned char)(name)^=(0x01<<(bitn)))

#define MASKSET(name,mask)  ((unsigned char)(name)|=(mask))
#define MASKCLR(name,mask)  ((unsigned char)(name)&=~(mask))
#define MASKTST(name,mask)  ((unsigned char)(name) & (mask))
#define MASKNOT(name,mask)  ((unsigned char)(name)^=(mask))

////////////////////////////////////////////////////////////////////////////////

// TO REDUCE THE NUMBER OF WARNINGS GENERATED

#define NOT_USED(p) if (&p) {}

////////////////////////////////////////////////////////////////////////////////

#endif

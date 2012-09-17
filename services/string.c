////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// These are common string functions.

// All functions that write to strings also take "max length" as a parameter.
// One safe way to keep from over-writing strings is to declare all strings
// using a constant, and then provide that constant to all function calls.
// Example:
//
//   #define MYSTRLEN  5
//   char mystring[MYSTRLEN+1];   // always add one for the terminating NULL
//   string_IntegerToString(123456, mystring, MYSTRLEN);
//
// This still produces a wrong result (the string is truncated).  But it does
// NOT over-write memory like the string functions (strcpy, sprintf, itoa) in
// the standard C library do.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "services/string.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// #define FLOAT


////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////
//   TYPES
////////////////////////////////////////////////////////////////////////////////

BOOLEAN string_IsSpace(char c) {
   if (c==' ') return TRUE;
   return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

BOOLEAN string_IsNumber(char c) {
   if ((c>='0')&&(c<='9')) return TRUE;
   if ((c=='-')||(c=='+')) return TRUE;
   return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

BOOLEAN string_IsDigit(char c) {
   if ((c>='0')&&(c<='9')) return TRUE;
   return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

BOOLEAN string_IsHexDigit(char c) {
   if ((c>='0')&&(c<='9')) return TRUE;
   if ((c>='A')&&(c<='F')) return TRUE;
   if ((c>='a')&&(c<='f')) return TRUE;
   return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//   FLOATING POINT
////////////////////////////////////////////////////////////////////////////////

#ifdef FLOAT
float string_StringToFloat(const char * string) {
   float val;
   float power;
   int8 posn;
   int8 sign;

   // skip white space
   for (posn=0;string[posn]==' ';posn++) {
   }

   // determine sign
   if (string[posn]=='-') {
      sign=-1;
   } else {
      sign=1;
   }

   // skip over sign digit
   if ((string[posn]=='-')||(string[posn]=='+')) {
      posn++;
   }

   // accumulate digits before the decimal point
   for (val=0.0;((string[posn]>='0')&&(string[posn]<='9'));posn++) {
      val=10.0*val+(string[posn]-'0');
   }

   // skip decimal point
   if (string[posn]=='.') {
      posn++;
   }

   // accumulate digits after the decimal (keeping track of power)
   for (power=1.0;((string[posn]>='0')&&(string[posn]<='9'));posn++) {
      val=10.0*val+(string[posn]-'0');
      power*=10.0;
   }

   return sign*val/power;
}
#endif // FLOAT

////////////////////////////////////////////////////////////////////////////////

#ifdef FLOAT
void string_FloatToString(float value, uint8 decimals, char * string, uint8 len) {
   uint16 power;
   uint8 total_numerical_digits;
   uint8 overall_size;
   uint8 dec_counter;
   uint16 long_value;
   uint16 multiplier;
   uint8 posn;
   uint8 sign;
   uint8 digit;

   // gotta start somewhere
   posn=0;

   // negative numbers
   sign=0;
   if (value<0.0) {
      if (posn<len) string[posn++]='-';
      value=0.0-value;
      sign=1;
   }

   // figure out multiplier to shift in decimal digits
   dec_counter=0;
   multiplier=1;
   while (dec_counter<decimals) {
      multiplier*=10;
      dec_counter++;
   }

   // build a long int with all digits
   long_value=(long)((float)multiplier*value);

   // see how big long int is (take log-base-10 of it)
   power=1;
   total_numerical_digits=1;
   while (((power*10)<=long_value)||(total_numerical_digits<=decimals)) {
      power*=10;
      total_numerical_digits++;
   }

   // figure out how long string should be
   overall_size=total_numerical_digits+sign+((decimals>0)?1:0);

   // go through and save each digit
   while (power>0) {
      digit=(char)(long_value/power);
      long_value-=(digit*power);
      if (posn<len) string[posn++]=(char)(digit+'0');
      // if this is the spot where the decimal should go, put it here
      if ((posn==(overall_size-decimals-1))&&(decimals>0)) {
         if (posn<len) string[posn++]='.';
      }
      power/=10;
   }

   // null terminate
   if (posn<len) string[posn]=0;
   string[len]=0;
}
#endif // FLOAT

////////////////////////////////////////////////////////////////////////////////
//   INTEGERS
////////////////////////////////////////////////////////////////////////////////

uint8 string_DigitToInteger(char digit) {
   if ((digit>='0')&&(digit<='9')) return digit-'0';
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

sint16 string_StringToInteger(const char * string) {
   uint8 posn;
   sint16 val;
   sint8 sign;

   // skip white space
   for (posn=0;string[posn]==' ';posn++) {
   }

   // determine sign
   if (string[posn]=='-') {
      sign=-1;
   } else {
      sign=1;
   }

   // skip over sign digit
   if ((string[posn]=='-')||(string[posn]=='+')) {
      posn++;
   }

   // accumulate digits before the decimal point
   for (val=0;((string[posn]>='0')&&(string[posn]<='9'));posn++) {
      val=10*val+(string[posn]-'0');
   }

   return sign*val;
}

////////////////////////////////////////////////////////////////////////////////

void string_IntegerToString(sint16 value, char * string, uint8 maxlen) {
   uint16 power;
   uint8 posn;
   uint8 sign;
   uint8 digit;

   // gotta start somewhere
   posn=0;

   // negative numbers
   sign=0;
   if (value<0) {
      if (posn<maxlen) string[posn++]='-';
      value=0-value;
      sign=1;
   }

   // see how big long int is (take log-base-10 of it)
   power=1;
   while ((power*10)<=value) power*=10;

   // go through and save each digit
   while (power>0) {
      digit=(char)(value/power);
      value-=(digit*power);
      if (posn<maxlen) string[posn++]=(char)(digit+'0');
      power/=10;
   }

   // null terminate
   if (posn<maxlen) string[posn]=0;
   string[maxlen]=0;
}

////////////////////////////////////////////////////////////////////////////////
//   HEX
////////////////////////////////////////////////////////////////////////////////

void string_IntegerToHex(uint16 value, char * string, uint8 len) {
   uint8 count;
   uint8 digit;

   // go through and save each digit
   for (count=len;count>0;count--) {
      digit=(uint8)value&0x0F;
      string[count-1]=(char)((digit<10)?(digit+'0'):(digit-10+'A'));
      value>>=4;
   }

   // null terminate
   string[len]=0;
}

////////////////////////////////////////////////////////////////////////////////

uint8 string_HexDigitToInteger(char digit) {
   if ((digit>='0')&&(digit<='9')) return digit-'0';
   if ((digit>='A')&&(digit<='F')) return digit-'A'+10;
   if ((digit>='a')&&(digit<='f')) return digit-'a'+10;
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

uint16 string_HexToInteger(char * string) {
   uint8 p=0;
   uint8 d;
   uint16 total=0;
   while (string_IsHexDigit(string[p])) {
      d=string_HexDigitToInteger(string[p]);
      if (d==STRING_INVALID_HEX_DIGIT) break;
      total=total*16+(uint16)d;
   }
   return total;
}

////////////////////////////////////////////////////////////////////////////////
//   COPYING
////////////////////////////////////////////////////////////////////////////////

// Copy Source to Destination.  The length of Destination is not to exceed Length.

void string_Copy( char * Destination, char * Source, uint8 Length ) {
// written by Alan Porter, 6 March 1995
   uint8 Counter=0;
   while ( ( Counter<Length ) && ( Source[Counter] != 0 ) ) {
      Destination[Counter]=Source[Counter];
      Counter++;
   }
   Destination[Counter]=0;
}

////////////////////////////////////////////////////////////////////////////////
//   SEARCHING
////////////////////////////////////////////////////////////////////////////////

// Return a pointer to the beginning of the next word in String.  Words
// are delimited by spaces.  If Length is specified, do not search past Length
// digits.  If no next word exists, return a pointer to the end of String.
// This makes it easy to iterate through words in a string like this:
//    while (next=NextWordInString(next),*next!=0) {
//       printf"product is %ld\n",atol(next));
//    }

char * string_NextWord( char * String ) {
// written by Alan Porter, 5 March 1996
   BOOLEAN LookingForSpaces=TRUE;
   int Counter=0;
   while (1) {
      if ( *(String+Counter) == 0 ) break;
      if (LookingForSpaces) {
         if ( string_IsSpace( *(String+Counter) ) ) LookingForSpaces=FALSE;
      } else {
         if ( !string_IsSpace( *(String+Counter) ) ) break;
      }
      Counter++;
   }
   return (String+Counter);
}

////////////////////////////////////////////////////////////////////////////////

char * string_SkipSpaces( char * String ) {
// written by Alan Porter, 5 March 1996
   int Counter=0;
   while (1) {
      if ( !string_IsSpace( *(String+Counter) ) ) break;
      Counter++;
   }
   return (String+Counter);
}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////



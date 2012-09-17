////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "utils\string.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////


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

#ifdef FLOAT
float string_StringToFloat(const char * string) {
   float val;
   float power;
   int8 posn;
   int8 sign;

   // skip white space
   for(posn=0;string[posn]==' ';posn++) {
   }

   // determine sign
   if(string[posn]=='-') {
      sign=-1;
   } else {
      sign=1;
   }

   // skip over sign digit
   if((string[posn]=='-')||(string[posn]=='+')) {
      posn++;
   }

   // accumulate digits before the decimal point
   for(val=0.0;((string[posn]>='0')&&(string[posn]<='9'));posn++) {
      val=10.0*val+(string[posn]-'0');
   }

   // skip decimal point
   if(string[posn]=='.') {
      posn++;
   }

   // accumulate digits after the decimal (keeping track of power)
   for(power=1.0;((string[posn]>='0')&&(string[posn]<='9'));posn++) {
      val=10.0*val+(string[posn]-'0');
      power*=10.0;
   }

   return sign*val/power;
}
#endif

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
   if(value<0.0) {
      if(posn<len) string[posn++]='-';
      value=0.0-value;
      sign=1;
   }

   // figure out multiplier to shift in decimal digits
   dec_counter=0;
   multiplier=1;
   while(dec_counter<decimals) {
      multiplier*=10;
      dec_counter++;
   }

   // build a long int with all digits
   long_value=(long)((float)multiplier*value);

   // see how big long int is (take log-base-10 of it)
   power=1;
   total_numerical_digits=1;
   while(((power*10)<=long_value)||(total_numerical_digits<=decimals)) {
      power*=10;
      total_numerical_digits++;
   }

   // figure out how long string should be
   overall_size=total_numerical_digits+sign+((decimals>0)?1:0);

   // go through and save each digit
   while(power>0) {
      digit=(char)(long_value/power);
      long_value-=(digit*power);
      if(posn<len) string[posn++]=(char)(digit+'0');
      // if this is the spot where the decimal should go, put it here
      if((posn==(overall_size-decimals-1))&&(decimals>0)) {
         if(posn<len) string[posn++]='.';
      }
      power/=10;
   }

   // null terminate
   if(posn<len) string[posn]=0;
   string[len]=0;
}
#endif

////////////////////////////////////////////////////////////////////////////////

sint16 string_StringToInteger(const char * string) {
   uint8 posn;
   sint16 val;
   sint8 sign;

   // skip white space
   for(posn=0;string[posn]==' ';posn++) {
   }

   // determine sign
   if(string[posn]=='-') {
      sign=-1;
   } else {
      sign=1;
   }

   // skip over sign digit
   if((string[posn]=='-')||(string[posn]=='+')) {
      posn++;
   }

   // accumulate digits before the decimal point
   for(val=0;((string[posn]>='0')&&(string[posn]<='9'));posn++) {
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
   if(value<0) {
      if(posn<maxlen) string[posn++]='-';
      value=0-value;
      sign=1;
   }

   // see how big long int is (take log-base-10 of it)
   power=1;
   while((power*10)<=value) power*=10;

   // go through and save each digit
   while(power>0) {
      digit=(char)(value/power);
      value-=(digit*power);
      if(posn<maxlen) string[posn++]=(char)(digit+'0');
      power/=10;
   }

   // null terminate
   if(posn<maxlen) string[posn]=0;
   string[maxlen]=0;
}

////////////////////////////////////////////////////////////////////////////////

void string_IntegerToHexString(uint16 value, char * string, uint8 len) {
   uint8 count;
   uint8 digit;

   // go through and save each digit
   for(count=len;count>0;count--) {
      digit=(uint8)value&0x0F;
      string[count-1]=(char)((digit<10)?(digit+'0'):(digit-10+'A'));
      value>>=4;
   }

   // null terminate
   string[len]=0;
}

////////////////////////////////////////////////////////////////////////////////

// Copy Source to Destination.  The length of Destination is not to exceed Length.

void string_Copy( char * Destination, char * Source, uint8 Length ) {
// written by Alan Porter, 6 March 1995
   uint8 Counter=0;
   while( ( Counter<Length ) && ( Source[Counter] != 0 ) ) {
      Destination[Counter]=Source[Counter];
      Counter++;
   }
   Destination[Counter]=0;
}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////



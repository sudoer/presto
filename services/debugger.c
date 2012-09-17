////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include <hc11.h>
#include "presto.h"
#include "types.h"
#include "services\debugger.h"
#include "utils\string.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

#define DEBUGGER_STACK_SIZE 256
#define CMDLINE_LEN 40

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

static BYTE debugger_stack[DEBUGGER_STACK_SIZE];
static PRESTO_TID_T debugger_tid;

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

static void newline(void);
static void prompt(void);
static void interpret_command(char * cmd);
static void debugger(void);

////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void debugger_init(void) {
   debugger_tid=presto_create_task(debugger, debugger_stack, DEBUGGER_STACK_SIZE, 1);
}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

static void prompt(void) {
   serial_send_string("> ");
}

////////////////////////////////////////////////////////////////////////////////

static void newline(void) {
   serial_send_string("\r\n");
}

////////////////////////////////////////////////////////////////////////////////

static void interpret_command(char * cmd) {
   serial_send_string(cmd);
   newline();

   switch(cmd[0]) {
      case 'd': {
         int x;
         char byte[4];
         for(x=0;x<128;x++) {
            string_IntegerToHexString(x,byte,2);
            serial_send_string(byte);
            serial_send_byte(' ');
            if((x%16)==15) newline();
         }
      } break;
      case 'h': {
         serial_send_string("help");
         newline();
         serial_send_string("d = dump");
         newline();
         serial_send_string("h = help");
         newline();
      } break;
   }

}

////////////////////////////////////////////////////////////////////////////////

static void debugger(void) {
   static char cmdline[CMDLINE_LEN+1];
   PRESTO_MAIL_T wake_up;
   BYTE in;
   uint8 posn=0;
   prompt();
   while(1) {
      presto_timer(debugger_tid,100,wake_up);
      presto_sleep();
      presto_get_message(&wake_up);
      while(serial_recv(&in)) {
         if((in=='\r')||(in=='\n')) {
            cmdline[posn]=0;
            posn=0;
            newline();
            interpret_command(cmdline);
            prompt();
         } else if(in==0x08) {
            if(posn>0) {
               posn--;
               serial_send_byte(0x08);
               serial_send_byte(' ');
               serial_send_byte(0x08);
            }
         } else if((in>=32)&&(in<=127)) {
            if(posn<CMDLINE_LEN) {
               cmdline[posn++]=(char)in;
               serial_send_byte(in);
            } else {
               serial_send_byte('!');
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////



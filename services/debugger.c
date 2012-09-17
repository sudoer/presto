////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "hc11regs.h"
#include "presto.h"
#include "types.h"
#include "services\debugger.h"
#include "services\inputs.h"
#include "services\motors.h"
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

static void prompt(void);
static void interpret_command(char * cmd);
static void debugger(void);

// common feedback functions
static void feedback_motor(uint8 motor);

////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void debugger_init(void) {
   debugger_tid=presto_create_task(debugger, debugger_stack, DEBUGGER_STACK_SIZE, 1);
}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

static void feedback_analog(uint8 input) {
   char num[3];
   uint8 value=input_sample_analog(input);
   serial_send_string("analog ");
   string_IntegerToString(input,num,2);
   serial_send_string(num);
   serial_send_string(" value ");
   string_IntegerToString(value,num,2);
   serial_send_string(num);
   serial_send_string("\r\n");
}

////////////////////////////////////////////////////////////////////////////////

static void feedback_digital(uint8 input) {
   char num[3];
   uint8 value=input_sample_digital(input);
   serial_send_string("digital ");
   string_IntegerToString(input,num,2);
   serial_send_string(num);
   serial_send_string(" value ");
   string_IntegerToString(value,num,2);
   serial_send_string(num);
   serial_send_string("\r\n");
}

////////////////////////////////////////////////////////////////////////////////

static void feedback_motor(uint8 motor) {
   char num[3];
   sint8 speed=motor_get_speed(motor);
   serial_send_string("motor ");
   string_IntegerToString(motor,num,2);
   serial_send_string(num);
   serial_send_string(" speed ");
   string_IntegerToString(speed,num,2);
   serial_send_string(num);
   serial_send_string("\r\n");
}

////////////////////////////////////////////////////////////////////////////////

static void interpret_command(char * cmd) {
   char * ptr;
   ptr=string_SkipSpaces(cmd);
   switch(*ptr) {
      ////////////////////////////////////////
      case 'd': {
         int x;
         char byte[4];
         for(x=0;x<128;x++) {
            string_IntegerToHex(x,byte,2);
            serial_send_string(byte);
            serial_send_byte(' ');
            if((x%16)==15) serial_send_string("\r\n");
         }
      } break;
      ////////////////////////////////////////
      case 'a': {
         BOOLEAN help=FALSE;
         ptr=string_NextWord(ptr);
         switch(*ptr) {
            case 0:
            case '?': {
               help=TRUE;
            } break;
            default: {
               if(string_IsDigit(*ptr)) {
                  // one particular input
                  uint8 input=string_StringToInteger(ptr);
                  feedback_analog(input);
               } else {
                  // 2nd argument is nonsense
                  help=TRUE;
               } // if digit
            } break;
         } // switch
         if(help) {
            serial_send_string("a ? = analog input help\r\n");
            serial_send_string("a 1 = read analog input 1\r\n");
         }
      } break;
      ////////////////////////////////////////
      case 'b': {
         BOOLEAN help=FALSE;
         ptr=string_NextWord(ptr);
         switch(*ptr) {
            case 0:
            case '?': {
               help=TRUE;
            } break;
            default: {
               if(string_IsDigit(*ptr)) {
                  // one particular input
                  uint8 input=string_StringToInteger(ptr);
                  feedback_digital(input);
               } else {
                  // 2nd argument is nonsense
                  help=TRUE;
               } // if digit
            } break;
         } // switch
         if(help) {
            serial_send_string("b ? = binary input help\r\n");
            serial_send_string("b 8 = read binary input 8\r\n");
         }
      } break;
      ////////////////////////////////////////
      case 'm': {
         BOOLEAN help=FALSE;
         ptr=string_NextWord(ptr);
         switch(*ptr) {
            case 'x': {
               // kill all motors
               uint8 motor;
               for(motor=0;motor<MOTORS_NUM_MOTORS;motor++) {
                  motor_set_speed(motor,0);
                  feedback_motor(motor);
               }
            } break;
            case 0:
            case '?': {
               help=TRUE;
            } break;
            default: {
               if(string_IsDigit(*ptr)) {
                  // one particular motor
                  uint8 motor=string_StringToInteger(ptr);
                  ptr=string_NextWord(ptr);
                  if(string_IsNumber(*ptr)) {
                     // set motor speed
                     motor_set_speed(motor,string_StringToInteger(ptr));
                     feedback_motor(motor);
                  } else if(*ptr==0) {
                     feedback_motor(motor);
                  } else {
                     // 3rd argument is nonsense
                     help=TRUE;
                  }
               } else {
                  // 2nd argument is nonsense
                  help=TRUE;
               } // if digit
            } break;
         } // switch
         if(help) {
            serial_send_string("m ?    = motor help\r\n");
            serial_send_string("m 1    = shows speed of motor 1\r\n");
            serial_send_string("m 2 5  = sets motor 2 to forward speed 5\r\n");
            serial_send_string("m 3 -2 = sets motor 3 to backward speed 2\r\n");
            serial_send_string("m x    = stops all motors\r\n");
         }
      } break;
      ////////////////////////////////////////
      case '?': {
         serial_send_string("a = analog input\r\n");
         serial_send_string("d = dump (memory)\r\n");
         serial_send_string("m = motor\r\n");
         serial_send_string("? = help\r\n");
      } break;
      ////////////////////////////////////////
      case 0: {
        // no input, the guy's just pressing ENTER
      } break;
      ////////////////////////////////////////
      default: {
         serial_send_string("unknown command, ? for help\r\n");
      } break;
   } // switch
}

////////////////////////////////////////////////////////////////////////////////

static void prompt(void) {
   serial_send_string("> ");
}

////////////////////////////////////////////////////////////////////////////////

static void debugger(void) {
   static char cmdline[CMDLINE_LEN+1];
   PRESTO_MAIL_T wake_up;
   BYTE in;
   uint8 posn=0;
   prompt();
   while(1) {

      if(input_stop_button()) motor_set_speed(0,-6);
      if(input_start_button()) motor_set_speed(0,6);

      presto_timer(debugger_tid,100,wake_up);
      presto_sleep();
      presto_get_message(&wake_up);
      while(serial_recv(&in)) {
         if((in=='\r')||(in=='\n')) {
            cmdline[posn]=0;
            posn=0;
            serial_send_string("\r\n");
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



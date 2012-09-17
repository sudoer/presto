
#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_

////////////////////////////////////////////////////////////////////////////////

#define ERROR_FIRST_KERNEL    0x01
#define ERROR_FIRST_CPU       0x20
#define ERROR_FIRST_APP       0x40
#define ERROR_MAX_LIMIT       0xFFFF

////////////////////////////////////////////////////////////////////////////////

typedef enum {

   ERROR_KERNEL = ERROR_FIRST_KERNEL-1,
   #include "kernel/error_kernel.h"

   ERROR_CPU = ERROR_FIRST_CPU-1,
   #include "error_cpu.h"

   ERROR_APP = ERROR_FIRST_APP-1,
   #include "error_app.h"

   ERROR_MAX = ERROR_MAX_LIMIT,

} error_number_e;

////////////////////////////////////////////////////////////////////////////////

#endif // _ERROR_CODES_H_


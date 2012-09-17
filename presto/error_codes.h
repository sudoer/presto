
#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_

////////////////////////////////////////////////////////////////////////////////

#define ERROR_FIRST_KERNEL    0x01
#define ERROR_FIRST_CPU       0x10
#define ERROR_FIRST_APP       0x80

////////////////////////////////////////////////////////////////////////////////

typedef enum {

   ERROR_KERNEL = ERROR_FIRST_KERNEL-1,
   #include "kernel/error_kernel.h"

   ERROR_CPU = ERROR_FIRST_CPU-1,
   #include "error_cpu.h"

   ERROR_APP = ERROR_FIRST_APP-1,
   #include "error_app.h"

} error_number_e;

////////////////////////////////////////////////////////////////////////////////

#endif // _ERROR_CODES_H_


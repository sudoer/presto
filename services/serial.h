
#ifndef _SERIAL_H_
#define _SERIAL_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"

////////////////////////////////////////////////////////////////////////////////

extern void serial_init(PRESTO_TASKID_T task, PRESTO_TRIGGER_T tx_alert, PRESTO_TRIGGER_T rx_alert);
extern void serial_send_byte(BYTE send);
extern void serial_send_string(char * send);
extern BOOLEAN serial_recv(BYTE * r);
extern int serial_recv_string(BYTE * recv, uint8 maxlen);

////////////////////////////////////////////////////////////////////////////////

#endif


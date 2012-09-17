////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "cpu/hc11regs.h"
#include "cpu/locks.h"
#include "cpu/intvect.h"
#include "kernel/kernel.h"
#include "services/serial.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

#define TX_BUFFER_SIZE 300
#define RX_BUFFER_SIZE 200

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef unsigned short cq_size_t;

typedef struct {
   BYTE * buffer;
   BYTE * end;
   BYTE * head;
   BYTE * tail;
   cq_size_t max_size;
   cq_size_t current_size;
} circ_queue;

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

static circ_queue com1_rx_queue;
static circ_queue com1_tx_queue;
static BYTE com1_rx_buffer[RX_BUFFER_SIZE];
static BYTE com1_tx_buffer[TX_BUFFER_SIZE];

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

static void serial_isr(void) __attribute__((interrupt));
static void clear_buffers(void);

// circular queue stuff
static cq_size_t cq_put_byte(circ_queue * queue, BYTE b);
static cq_size_t cq_get_byte(circ_queue * queue, BYTE * b);
static void cq_init(circ_queue * queue, BYTE * buffer, cq_size_t max_size);

static PRESTO_TID_T     serialuser_tid;
static PRESTO_TRIGGER_T serialuser_trigger;

////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void serial_init(PRESTO_TID_T task, PRESTO_TRIGGER_T alert) {
   KERNEL_LOCK_T lock;
   presto_lock_save(lock);
   // 1 start, 8 data, 1 stop bits
   // WAKE mode = idle line
   SCCR1 = 0x00;
   // transmit interrupt enable, receive interrupt enable
   // transmit enable, receive enable
   SCCR2= SCCR2_TIE|SCCR2_RIE|SCCR2_TE|SCCR2_RE;
   // For 8 MHz system clock, and therefore 2 MHz E-clock)
   // BAUD=BAUD_SCP1|BAUD_SCP0|BAUD_SCR1|BAUD_SCR0; // 1200 baud
   // BAUD=BAUD_SCP1|BAUD_SCP0|BAUD_SCR1; // 2400 baud
   // BAUD=BAUD_SCP1|BAUD_SCP0|BAUD_SCR0; // 4800 baud
   // BAUD=BAUD_SCP1|BAUD_SCP0; // 9600 baud
   BAUD=BAUD_SCP1|BAUD_SCP0; // 9600 baud
   // INITIALIZE SERIAL PORT SOFTWARE
   clear_buffers();
   // remember who called us, and their alert trigger
   serialuser_tid=task;
   serialuser_trigger=alert;
   // set serial port interrupt service routine
   set_interrupt(INTR_SCI, serial_isr);
   presto_unlock_restore(lock);
}

////////////////////////////////////////////////////////////////////////////////

static void clear_buffers(void) {
   cq_init(&com1_rx_queue, com1_rx_buffer, RX_BUFFER_SIZE);
   cq_init(&com1_tx_queue, com1_tx_buffer, TX_BUFFER_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
//   SENDING
////////////////////////////////////////////////////////////////////////////////

void serial_send_byte(BYTE send) {
   // queue up one byte to send
   cq_put_byte(&com1_tx_queue,send);
   // enable the TX interrupt
   MASKSET(SCCR2,SCCR2_TIE);
}

////////////////////////////////////////////////////////////////////////////////

void serial_send_string(char * send) {
   // queue up the bytes to send
   while (*send) {
      cq_put_byte(&com1_tx_queue,*send);
      send++;
   }
   // enable the TX interrupt
   MASKSET(SCCR2,SCCR2_TIE);
}

////////////////////////////////////////////////////////////////////////////////
//   RECEIVING
////////////////////////////////////////////////////////////////////////////////

BOOLEAN serial_recv(BYTE * r) {
   BOOLEAN b;
   // read one byte from the serial RX queue
   b=cq_get_byte(&com1_rx_queue, r);
   // enable the RX interrupt
   MASKSET(SCCR2,SCCR2_RIE);
   return b;
}

////////////////////////////////////////////////////////////////////////////////

int serial_recv_string(BYTE * recv, uint8 maxlen) {
   int count=0;
   BYTE * ptr=recv;
   // read the serial RX queue until it is empty or we are full
   while (cq_get_byte(&com1_rx_queue,ptr++)&&(count<maxlen)) count++;
   // enable the RX interrupt
   MASKSET(SCCR2,SCCR2_RIE);
   return count;
}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////
//   INTERRUPT SERVICE ROUTINE
////////////////////////////////////////////////////////////////////////////////

static void serial_isr(void) {
   BYTE b;
   BOOLEAN received_something=FALSE;

   // TRANSMIT

   while (SCSR&SCSR_TDRE) {
      // TX data register is empty... feed it
      if (cq_get_byte(&com1_tx_queue,&b)) {
         // there is data in the TX queue
         SCDR=b;
      } else {
         // TX queue empty, turn off TX interrupt
         MASKCLR(SCCR2,SCCR2_TIE);
         break; // from while loop
      }
   }

   // RECEIVE

   while (SCSR&SCSR_RDRF) {
      // RX data register is full... read it
      cq_put_byte(&com1_rx_queue,SCDR);
      received_something=TRUE;
   }

   // NOTIFY RECEIVER

   if (received_something) {
      presto_trigger_send(serialuser_tid,serialuser_trigger);
      // will do a task switch if necessary
   }
}

////////////////////////////////////////////////////////////////////////////////
//   CIRCULAR QUEUE STUFF
////////////////////////////////////////////////////////////////////////////////

static void cq_init(circ_queue * queue, BYTE * buffer, cq_size_t max_size) {
   KERNEL_LOCK_T lock;
   presto_lock_save(lock);
   queue->buffer=buffer;
   queue->end=buffer+max_size-1;
   queue->head=buffer;
   queue->tail=buffer;
   queue->max_size=max_size;
   queue->current_size=0;
   presto_unlock_restore(lock);
}

////////////////////////////////////////////////////////////////////////////////

// returns the number of bytes retrieved from the queue (0 or 1)

static cq_size_t cq_get_byte(circ_queue * queue, BYTE * b) {
   KERNEL_LOCK_T lock;
   cq_size_t bytes=0;

   presto_lock_save(lock);
   // If the buffer is not empty
   if (queue->tail != queue->head) {
      // return the contents at tail pointer
      *b = *queue->tail;
      // Increment the tail pointer.
      queue->tail++;
      // If the tail points past the circular queue, then
      if (queue->tail > queue->end) {
         // point the tail at the beginning of the circular queue.
         queue->tail = queue->buffer;
      }
      bytes++;
   }
   presto_unlock_restore(lock);
   return bytes;
}

////////////////////////////////////////////////////////////////////////////////

// returns 0 if no errors, else the number of overwritten bytes (one)

static cq_size_t cq_put_byte(circ_queue * queue, BYTE b) {
   KERNEL_LOCK_T lock;
   cq_size_t overwrite=0;

   presto_lock_save(lock);
   // Store byte at head pointer
   *(queue->head) = b;

   // Increment the head pointer
   queue->head++;

   // If the head points past the queue
   if (queue->head > queue->end) {
      // point the head at the beginning of the circular queue.
      queue->head = queue->buffer;
   }

   // If the head points at the tail
   if (queue->head == queue->tail) {
      // increment the tail pointer
      queue->tail++;
      overwrite++;
      // If the tail points past the queue
      if (queue->tail > queue->end) {
         // point the tail at the beginning of the circular queue.
         queue->tail = queue->buffer;
      }
   }
   presto_unlock_restore(lock);
   return overwrite;
}

////////////////////////////////////////////////////////////////////////////////


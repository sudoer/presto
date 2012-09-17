////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// This serial driver is built as a service.  That is, it is not a part of
// the presto kernel itself, but it uses presto functions just like an
// application would.  To start using the serial port, serial_init() must
// be called.  The task ID and a trigger flag that you specify will be used
// to notify the task when data is received.  If you do not want to be
// notified, then just use a trigger flag of 0x00.  Typically, you would
// simply have a task that looks like this:

//    serial_init(task_tid,SERIAL_FLAG);
//    while(1) {
//       wait(SERIAL_FLAG);
//       serial_recv_string(mybuffer,MYBUFFERSIZE);
//       /* act on received data here */
//    }


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "hc11_regs.h"
#include "locks.h"
#include "vectors.h"
#include "presto.h"
#include "services/serial.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

#define TX_BUFFER_SIZE    250
#define TX_RESTART_LEVEL   16
#define RX_BUFFER_SIZE     16

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
static void queue_up_or_block(BYTE send);

// circular queue stuff
static cq_size_t cq_used(circ_queue * queue);
static cq_size_t cq_put_byte(circ_queue * queue, BYTE b);
static cq_size_t cq_get_byte(circ_queue * queue, BYTE * b);
static void cq_init(circ_queue * queue, BYTE * buffer, cq_size_t max_size);

static PRESTO_TASKID_T  serialuser_tid;
static PRESTO_TRIGGER_T serialuser_tx_trigger;
static PRESTO_TRIGGER_T serialuser_rx_trigger;

////////////////////////////////////////////////////////////////////////////////
//   INITIALIZATION
////////////////////////////////////////////////////////////////////////////////

void serial_init(PRESTO_TASKID_T task, PRESTO_TRIGGER_T tx_alert, PRESTO_TRIGGER_T rx_alert) {
   // 1 start, 8 data, 1 stop bits
   // WAKE mode = idle line
   SCCR1 = 0x00;
   // For 8 MHz system clock, and therefore 2 MHz E-clock)
   // BAUD=BAUD_SCP1|BAUD_SCP0|BAUD_SCR1|BAUD_SCR0; // 1200 baud
   // BAUD=BAUD_SCP1|BAUD_SCP0|BAUD_SCR1; // 2400 baud
   // BAUD=BAUD_SCP1|BAUD_SCP0|BAUD_SCR0; // 4800 baud
   // BAUD=BAUD_SCP1|BAUD_SCP0; // 9600 baud
   BAUD=BAUD_SCP1|BAUD_SCP0; // 9600 baud
   // INITIALIZE SERIAL PORT SOFTWARE
   clear_buffers();
   // remember who called us, and their rx_alert trigger
   serialuser_tid=task;
   serialuser_tx_trigger=tx_alert;
   serialuser_rx_trigger=rx_alert;
   // set serial port interrupt service routine
   set_interrupt(INTR_SCI, serial_isr);
   // transmit interrupt enable, receive interrupt enable
   // transmit enable, receive enable
   SCCR2= SCCR2_TIE|SCCR2_RIE|SCCR2_TE|SCCR2_RE;
}

////////////////////////////////////////////////////////////////////////////////

static void clear_buffers(void) {
   cq_init(&com1_rx_queue, com1_rx_buffer, RX_BUFFER_SIZE);
   cq_init(&com1_tx_queue, com1_tx_buffer, TX_BUFFER_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
//   SENDING
////////////////////////////////////////////////////////////////////////////////

static void queue_up_or_block(BYTE send) {
   cq_size_t b;
   // queue up one byte to send
   do {
      b=cq_put_byte(&com1_tx_queue,send);
      if (b==0) {
         // enable the TX interrupt
         MASKSET(SCCR2,SCCR2_TIE);
         presto_trigger_clear(serialuser_tx_trigger);
         presto_wait(serialuser_tx_trigger);
      }
   } while (b<1);
}

////////////////////////////////////////////////////////////////////////////////

void serial_send_byte(BYTE send) {
   queue_up_or_block(send);
   // enable the TX interrupt
   MASKSET(SCCR2,SCCR2_TIE);
}

////////////////////////////////////////////////////////////////////////////////

void serial_send_string(char * send) {
   // queue up the bytes to send
   while (*send) {
      queue_up_or_block(*send);
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
//   INTERRUPT SERVICE ROUTINE
////////////////////////////////////////////////////////////////////////////////

static void serial_isr(void) {
   BYTE b;
   BOOLEAN sent_something=FALSE;
   BOOLEAN received_something=FALSE;

   // TRANSMIT

   while (SCSR&SCSR_TDRE) {
      // TX data register is empty... feed it
      if (cq_get_byte(&com1_tx_queue,&b)) {
         // there is data in the TX queue
         SCDR=b;
         sent_something=TRUE;
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

   // ASSERT TRIGGERS IF NECESSARY

   if((sent_something)&&(cq_used(&com1_tx_queue)<TX_RESTART_LEVEL)) {
      presto_trigger_send(serialuser_tid,serialuser_tx_trigger);
      // will do a task switch if necessary
   }

   if (received_something) {
      presto_trigger_send(serialuser_tid,serialuser_rx_trigger);
      // will do a task switch if necessary
   }
}

////////////////////////////////////////////////////////////////////////////////
//   CIRCULAR QUEUE STUFF
////////////////////////////////////////////////////////////////////////////////

static void cq_init(circ_queue * queue, BYTE * buffer, cq_size_t max_size) {
   queue->buffer=buffer;
   queue->end=buffer+max_size-1;
   queue->head=buffer;
   queue->tail=buffer;
   queue->max_size=max_size;
   queue->current_size=0;
}

////////////////////////////////////////////////////////////////////////////////

static cq_size_t cq_used(circ_queue * queue) {
   return queue->current_size;
}

////////////////////////////////////////////////////////////////////////////////

// returns the number of bytes retrieved from the queue (0 or 1)

static cq_size_t cq_get_byte(circ_queue * queue, BYTE * b) {
   CPU_LOCK_T lock;

   // If the queue is empty, forget it.
   if (queue->current_size == 0) return 0;

   cpu_lock_save(lock);

   // Return the contents at tail pointer.
   *b = *queue->tail;

   // Increment the tail pointer.
   queue->tail++;

   // If the tail points past the circular queue, then
   if (queue->tail > queue->end) {
      // point the tail at the beginning of the circular queue.
      queue->tail = queue->buffer;
   }

   // Keep track of size.
   queue->current_size--;

   cpu_unlock_restore(lock);
   return 1;
}

////////////////////////////////////////////////////////////////////////////////

// returns the number of bytes written to the queue (0 or 1)

static cq_size_t cq_put_byte(circ_queue * queue, BYTE b) {
   CPU_LOCK_T lock;

   // If the queue is full, forget it.
   if (queue->current_size >= queue->max_size) return 0;

   cpu_lock_save(lock);

   // Store byte at head pointer
   *(queue->head) = b;

   // Increment the head pointer
   queue->head++;

   // If the head points past the queue
   if (queue->head > queue->end) {
      // point the head at the beginning of the circular queue.
      queue->head = queue->buffer;
   }

   // Keep track of size.
   queue->current_size++;

   cpu_unlock_restore(lock);
   return 1;
}

////////////////////////////////////////////////////////////////////////////////


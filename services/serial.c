
#include <hc11.h>
#include "hc11regs.h"
#include "types.h"
#include "services\serial.h"

////////////////////////////////////////////////////////////////////////////////

#pragma interrupt_handler presto_serial_isr

////////////////////////////////////////////////////////////////////////////////

// CONSTANTS

#define TX_BUFFER_SIZE 500
#define RX_BUFFER_SIZE 20

////////////////////////////////////////////////////////////////////////////////

// TYPE DEFINITIONS

typedef struct {
   BYTE * buffer;
   BYTE * head;
   BYTE * tail;
   uint8 max_size;
   uint8 current_size;
} circ_queue;

////////////////////////////////////////////////////////////////////////////////

// LOCAL VARIABLES

static circ_queue com1_rx_queue;
static circ_queue com1_tx_queue;
static BYTE com1_rx_buffer[RX_BUFFER_SIZE];
static BYTE com1_tx_buffer[TX_BUFFER_SIZE];

////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES

static void clear_buffers(void);

// circular queue stuff
static uint8 cq_put_byte(circ_queue * queue, BYTE b);
static uint8 cq_get_byte(circ_queue * queue, BYTE * b);
static void cq_init(circ_queue * queue, BYTE * buffer, uint8 max_size);

////////////////////////////////////////////////////////////////////////////////

void serial_init(uint16 baud) {

   // INITIALIZE SERIAL PORT HARDWARE

   INTR_OFF();

   // 1 start, 8 data, 1 stop bits
   // WAKE mode = idle line
   SCCR1 = SCCR1_M;

   // transmit interrupt enable, receive interrupt enable
   // transmit enable, receive enable
   // SCCR2= /*SCCR2_TIE|SCCR2_RIE|*/ SCCR2_TE|SCCR2_RE;
   SCCR2= SCCR2_TIE|SCCR2_RIE|SCCR2_TE|SCCR2_RE;

   // 2400 baud for 2 MHz clock
   BAUD=BAUD_SCP1|BAUD_SCP0;

   // INITIALIZE SERIAL PORT SOFTWARE

   clear_buffers();

   INTR_ON();

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
   // disable TX interrupt
   MASKCLR(SCCR2,SCCR2_TIE);
   // queue up one byte to send
   cq_put_byte(&com1_tx_queue,send);
   // re-enable the TX interrupt
   MASKSET(SCCR2,SCCR2_TIE);
   return;
}

////////////////////////////////////////////////////////////////////////////////

void serial_send_string(char * send) {
   // disable TX interrupt
   MASKCLR(SCCR2,SCCR2_TIE);
   // queue up the bytes to send
   while(*send) {
      cq_put_byte(&com1_tx_queue,*send);
      send++;
   }
   // re-enable the TX interrupt
   MASKSET(SCCR2,SCCR2_TIE);
   return;
}

////////////////////////////////////////////////////////////////////////////////
//   RECEIVING
////////////////////////////////////////////////////////////////////////////////

BOOLEAN serial_recv(BYTE * r) {
   BOOLEAN b;
   // disable RX interrupt
   MASKCLR(SCCR2,SCCR2_RIE);
   // read one byte from the serial RX queue
   b=cq_get_byte(&com1_rx_queue, r);
   // re-enable the RX interrupt
   MASKSET(SCCR2,SCCR2_RIE);
   return b;
}

////////////////////////////////////////////////////////////////////////////////

int serial_recv_string(BYTE * recv, uint8 maxlen) {
   int count=0;
   BYTE * ptr=recv;
   // disable RX interrupt
   MASKCLR(SCCR2,SCCR2_RIE);
   // read the serial RX queue until it is empty or we are full
   while(cq_get_byte(&com1_rx_queue,ptr++)&&(count<maxlen)) count++;
   // re-enable the RX interrupt
   MASKSET(SCCR2,SCCR2_RIE);
   return count;
}

////////////////////////////////////////////////////////////////////////////////
//   INTERRUPT SERVICE ROUTINE
////////////////////////////////////////////////////////////////////////////////

void presto_serial_isr(void) {
   BYTE b;
   INTR_OFF();
   if(SCSR&SCSR_TDRE) {
      // TX data register is empty... feed it
      if(cq_get_byte(&com1_tx_queue,&b)) {
         // there is data in the TX queue
         SCDR=b;
      } else {
         // TX queue empty, turn off TX interrupt
         MASKCLR(SCCR2,SCCR2_TIE);
      }
   }
   if(SCSR&SCSR_RDRF) {
      // RX data register is full... read it
      cq_put_byte(&com1_rx_queue,SCDR);
   }
   INTR_ON();
}

////////////////////////////////////////////////////////////////////////////////
//   CIRCULAR QUEUE STUFF
////////////////////////////////////////////////////////////////////////////////

static void cq_init(circ_queue * queue, BYTE * buffer, uint8 max_size) {
   queue->buffer=buffer;
   queue->head=buffer;
   queue->tail=buffer;
   queue->max_size=max_size;
   queue->current_size=0;
}

////////////////////////////////////////////////////////////////////////////////

// returns the number of bytes retrieved from the queue

static uint8 cq_get_byte(circ_queue * queue, BYTE * b) {

   // If the buffer is not empty
   if (queue->tail != queue->head) {
      // return the contents at tail pointer
      *b = *queue->tail;
      // Increment the tail pointer.
      queue->tail++;
      // If the tail points past the circular queue, then
      if (queue->tail > (queue->buffer + queue->max_size - 1)) {
         // point the tail at the beginning of the circular queue.
         queue->tail = queue->buffer;
      }
      return 1;
   } else {
      // the TX circular queue is empty
      return 0;
   }
}

////////////////////////////////////////////////////////////////////////////////

// returns 0 if no errors, else the number of overwritten bytes (one)

static uint8 cq_put_byte(circ_queue * queue, BYTE b) {
   uint8 overwrite=0;

   // Store byte at head pointer
   *(queue->head) = b;

   // Increment the head pointer
   queue->head++;

   // If the head points past the queue
   if (queue->head > (queue->buffer + queue->max_size - 1)) {
      // point the head at the beginning of the circular queue.
      queue->head = queue->buffer;
   }

   // If the head points at the tail
   if (queue->head == queue->tail) {
      // increment the tail pointer
      queue->tail++;
      overwrite++;
      // If the tail points past the queue
      if (queue->tail > (queue->buffer + queue->max_size - 1)) {
         // point the tail at the beginning of the circular queue.
         queue->tail = queue->buffer;
      }
   }
   return overwrite;
}

////////////////////////////////////////////////////////////////////////////////


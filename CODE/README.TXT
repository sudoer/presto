
Command-line args for compiler and linker documented.
More intelligent segment layout for linker.
Empty space left from FFC0-FFFF for INTVECT when using simulator.
LCD works on pins SCL=D2,SDA=D5.
Tasks are added to queue in priority order.
Multiple timers and messages can be sent.
Messages and timers cause tasks to become ready.
Timers and messages can be received.
Message/Timers use a structure for message payload.
Minor improvements to LCD.

Five real tasks:
 - one blinks motor0 lights every 250 ms
 - one blinks motor1 lights every 200 ms
 - one blinks motor2 lights every 1000 ms, sends message to LCD task
 - one blinks motor3 lights every 1250 ms
 - LCD task waits for messages, prints message contents to LCD

There is some sort of race condition that occasionally causes a
lock-up.  Change the timing of task Two to 500 and 500, change
task One to 150 and 150.  You'll see the lock-up within a few
seconds.

Source code is divided into three sections: kernel, services, app.
Build script is much more robust, and does not require "newer.exe".
All object files, list files, map files are put in OBJ directory.

------------------------------------------

Hook up LCD like this on handyboard
  5G
  CD
  ++

------------------------------------------

On LCD itself:
1=CLOCK
2=GROUND
3=+5V
4=contrast (-2V)
5=+5V
6=DATA

------------------------------------------

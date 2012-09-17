                              1 
                              2 .area DATA
   0000                       3    _presto_asm_save_sp:
   A6A2 00 01                 4    .word 1
                              5 .area IDATA
   A6A4 00 00                 6    .word 0    ; _presto_asm_save_sp
                              7 .area CODE
                              8 
                              9 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                             10 
   0010                      11 CCR_INTERRUPT=0x10
   1008                      12 ISR_LED_PORT=0x1008       ; PORTD
   0010                      13 ISR_LED_MASK=0x10         ; PIN4
                             14 
                             15 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                             16 
   0000                      17 _presto_system_isr::
                             18 ; uses global presto_asm_old_sp_p
                             19 ; uses global presto_asm_new_sp
                             20 ; uses global presto_asm_swap
                             21 
                             22    ; turn off interrupts
   C000 0F                   23    sei
                             24    ; LED on
                             25    ; ldaa  ISR_LED_PORT
                             26    ; anda  ~#ISR_LED_MASK
                             27    ; staa  ISR_LED_PORT
                             28    ; increment clock, check if we need to pre-empt
   C001 BD 02 FB             29    jsr   _presto_service_timer_interrupt
                             30    ; check to see if we need to switch tasks
   C004 7D 98 00             31    tst   _presto_asm_swap
   C007 27 08                32    beq   psi_rti
                             33    ; toggle the speaker
                             34    ; ldaa  $1000
                             35    ; eora  #$08
                             36    ; staa  $1000
                             37   ; swap the stack pointers
   C009 FE 27 30             38    ldx   _presto_asm_old_sp_p
   C00C AF 00                39    sts   0,x
   C00E BE 27 2E             40    lds   _presto_asm_new_sp
   0011                      41 psi_rti:
                             42    ; clear interrupt mask bit (enable ints) in the CC register on the stack
   C011 32                   43    pula
   C012 B4 FF EF             44    anda  ~#CCR_INTERRUPT
   C015 36                   45    psha
                             46    ; LED off
                             47    ; ldaa  ISR_LED_PORT
                             48    ; oraa  ISR_LED_MASK
                             49    ; staa  ISR_LED_PORT
                             50    ; "run" new task
   C016 3B                   51    rti
                             52 
                             53 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                             54 
   0017                      55 _presto_switch_tasks::
                             56 ; uses global presto_asm_old_sp_p
                             57 ; uses global presto_asm_new_sp
                             58 
                             59    ; LED on
                             60    ; ldaa  ISR_LED_PORT
                             61    ; anda  ~#ISR_LED_MASK
                             62    ; staa  ISR_LED_PORT
                             63    ; save the registers (in the same order that an interrupt does)
   C017 18 3C                64    pshy
   C019 3C                   65    pshx
   C01A 36                   66    psha
   C01B 37                   67    pshb
   C01C 07                   68    tpa
   C01D B4 FF EF             69    anda  ~#CCR_INTERRUPT     ; enable interrupts in pushed CC register
   C020 36                   70    psha
                             71    ; swap the stack pointers
   C021 FE 27 30             72    ldx   _presto_asm_old_sp_p
   C024 AF 00                73    sts   0,x
                             74    ; fall through to the "half-function" below
   0026                      75 _presto_start_task_switching::
   C026 BE 27 2E             76    lds   _presto_asm_new_sp
                             77    ; restore the registers
   C029 32                   78    pula
   C02A 8A 10                79    oraa  #CCR_INTERRUPT      ; do not enable interrupts in the CC register... yet
   C02C 06                   80    tap
   C02D 33                   81    pulb
   C02E 32                   82    pula
   C02F 38                   83    pulx
   C030 18 38                84    puly
                             85    ; NOW we can re-enable interrupts, because we are done with the stack
   C032 0E                   86    cli
                             87    ; LED off
                             88    ; ldaa  ISR_LED_PORT
                             89    ; oraa  ISR_LED_MASK
                             90    ; staa  ISR_LED_PORT
                             91    ; "run" new task
   C033 39                   92    rts
                             93 
                             94 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                             95 
   0034                      96 _presto_setup_new_task::
                             97 ; uses global presto_asm_new_sp
                             98 
                             99    ; store our own SP so we can work on the new task
   C034 BF A6 A2            100    sts _presto_asm_save_sp
                            101    ; load empty SP from task so we can initialize it
   C037 BE 27 2E            102    lds _presto_asm_new_sp
                            103    ; make presto_kill_self as the "return pc" of a new task,
                            104    ; so if it ever returns, it will reclaim the TCB, etc
   C03A CC 02 F1            105    ldd   #_presto_kill_self
   C03D 37                  106    pshb
   C03E 36                  107    psha
                            108    ; push the actual function call on the stack
   C03F FC 27 32            109    ldd   _presto_asm_new_fn
   C042 37                  110    pshb
   C043 36                  111    psha
                            112    ; push any old stinkin' registers onto the stack
                            113    ; they'll be pulled off when we start running
                            114    ; we push in interrupt-stack order
   C044 18 3C               115    pshy
   C046 3C                  116    pshx
   C047 36                  117    psha
   C048 37                  118    pshb
   C049 07                  119    tpa
   C04A B4 FF EF            120    anda  ~#CCR_INTERRUPT     ; enable interrupts in pushed CC
   C04D 36                  121    psha
                            122    ; save task SP in TCB
   C04E BF 27 2E            123    sts   _presto_asm_new_sp
                            124    ; re-load our own SP so we can return
   C051 BE A6 A2            125    lds   _presto_asm_save_sp
   C054 39                  126    rts
                            127 
                            128 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
                            129 
                            130 

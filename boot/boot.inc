SEG_BOOT        EQU     0x07C0  ;512 = 0x200
SEG_SETUP       EQU     0x07E0
SEG_KERNEL      EQU     0x1000

FD_SETUP        EQU     0x02    ;sector 2
FD_SETUP_SIZE   EQU     0x05    ;5 sectors

FD_KERNEL       EQU     0x07    ;sector 7
FD_KERNEL_SIZE  EQU     0x0C    ;12 sectors

TASK_COUNT      EQU     0x10
INTR_COUNT      EQU     0x100

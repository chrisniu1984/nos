%include "head.inc"

extern main

[BITS 16]
jmp    0x1000:_start_16

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_GDT:
    dq  0x0000000000000000                                  ; 0x00,空描述符              
    m4Code  BASEADDR,         0xFFFFF, G_1B, DPL_0, CODE_ER ; 0x08,code
    m4Data  BASEADDR,         0xFFFFF, G_1B, DPL_0, DATA_RW ; 0x10,data
    m4Data  0x000B8000,       0xFFFFF, G_1B, DPL_0, DATA_RW ; 0x18,vga text mode
    times   256 dq 0x0000000000000000                       ; LDT
    times   256 dq 0x0000000000000000                       ; TSS

GdtPtr: dw  $ - _GDT - 1
        dd  0

_IDT:   times   256 dq 0x0000000000000000 ; 未初始化的中断描述符表

IdtPtr: dw  $ - _IDT - 1
        dd  0

[BITS 16]
_start_16:
    mov     ax,cs
    mov     ds,ax
    mov     es,ax
    mov     ss,ax
    mov     sp,0x400

    cli
    call    setup_pic
    call    setup_gdt
    call    setup_idt

    ; 打开地址线A20
    in      al, 92h
    or      al, 00000010b
    out     92h, al

    ; 准备切换到保护模式
    mov     eax, cr0
    or      eax, 0x00000001
    mov     cr0, eax

    ; 真正进入保护模式
    jmp     0x08:_start_32

; 设置8259A
setup_pic:
    ; ICW1
    mov     al, 0x11        ; initialization sequence
    out     0x20,al         ; send it to 8259A-1
    dd      0x00eb,0x00eb   ; jmp $+2, jmp $+2
    out     0xA0,al         ; and to 8259A-2
    dd      0x00eb,0x00eb

    ; ICW2
    mov     al,0x20         ; start of hardware int's (0x20)
    out     0x21,al
    dd      0x00eb,0x00eb
    mov     al,0x28         ; start of hardware int's 2 (0x28)
    out     0xA1,al
    dd      0x00eb,0x00eb

    ; ICW3
    mov     al,0x04         ; 8259-1 is master
    out     0x21,al
    dd      0x00eb,0x00eb
    mov     al,0x02         ; 8259-2 is slave
    out     0xA1,al
    dd      0x00eb,0x00eb

    ; ICW4
    mov     al,0x01         ; 8086 mode for both
    out     0x21,al
    dd      0x00eb,0x00eb
    out     0xA1,al
    dd      0x00eb,0x00eb


    mov     al,0xFF         ; mask off all interrupts for now
    out     0x21,al
    dd      0x00eb,0x00eb
    out     0xA1,al
    ret

; 设置gdt
setup_gdt:
    xor     eax, eax
    mov     ax, ds
    shl     eax, 4
    add     eax, _GDT
    mov     dword [GdtPtr+2], eax
    lgdt    [GdtPtr]
    ret

; 设置idt
setup_idt:
    xor     eax, eax
    mov     ax, ds
    shl     eax, 4
    add     eax, _IDT
    mov     dword [IdtPtr+2], eax
    lidt    [IdtPtr]
    ret

[BITS 32]
_start_32:
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     ss, ax
    mov     esp, 0x40000

    mov     ax, 0x18
    mov     gs, ax

    jmp     main


;
; setup.s
;
; setup被boot加载到物理地址0x7E00处。
;
; setup的作用是设置GDT,IDT，并把系统切换到保护模式。
; 最后跳转到入处于保护模式的kernel（0x10000处）开始执行c代码。

%include "setup.inc"

jmp    _start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_GDT:
    dq  0x0000000000000000  ; 空描述符
    m4Code  BASEADDR,         0xFFFFF, G_1B, DPL_0, CODE_ER
    m4Data  BASEADDR,         0xFFFFF, G_1B, DPL_0, DATA_RW
    m4Data  0x000B8000,       0xFFFFF, G_1B, DPL_0, DATA_RW
GdtPtr: dw  $ - _GDT - 1
        dd    0

_IDT:   times   256 dq 0x0000000000000000 ; 未初始化的中断描述符表
IdtPtr: dw      $ - _IDT - 1
        dd    0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
msg    db    "Now, I'm in Setup !"
len    equ    $-msg
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

BITS 16
_start:
    mov     ax,cs
    mov     ds,ax
    mov     es,ax
    mov     ss,ax
    mov     sp,0x400

    ;读取当前光标位置
    mov     ah,0x03
    xor     bh,bh       ;bh = page
    int     0x10

    ;移动到下行行首
    inc     dh          ;dh = row
    xor     dl,dl       ;dl = column

    ;显示字符串
    mov     ax,cs
    mov     es,ax       ;es = seg
    mov     bp,msg      ;bp = offset
    mov     cx,len      ;cx = len
    mov     bx,0x0004   ;bh = page, bl = attribute
    mov     ax,0x1301
    int     0x10

    call    setup_gdt
    call    setup_idt

    ; 关中断
    cli

    ; 打开地址线A20
    in      al, 92h
    or      al, 00000010b
    out     92h, al

    ; 准备切换到保护模式
    mov     eax, cr0
    or      eax, 0x00000001
    mov     cr0, eax

    ; 真正进入保护模式
    jmp     8 :0

    ; never be here
    jmp     $

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

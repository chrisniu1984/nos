;
; boot.s (nasm)
;
; boot被bios加载到物理地址0x07C00处，boot的大小为512字节。
; boot需要加载kernel到0x0000处。

%include "boot.inc"

;系统只是保证加载首扇区到物理地址的0x07C00处并将跳转到此物理地址执行，
;但是不保证cs和offset是否为我们想要的，现在我们通过跳转把段寄存器设置
;为我们需要的值 0x07C0。
jmp    SEG_BOOT:main

main:
    ;加载kenerl到0x0000:0000
    mov     ax,SEG_KERNEL
    mov     es,ax
    mov     bx,0x0000           ;bx = offset
    mov     ch,0x00             ;ch = track
    mov     dx,0x0000           ;dh = head, dl = drive
    mov     cl,FDSEC_KERNEL     ;cl = sector
    mov     al,FDSEC_KERNEL_NUM ;al = number of sectors
    mov     ah,0x02
    int     0x13

    jmp     SEG_KERNEL:0x0000
    jmp     $

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; boot flag
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    times 510-($-$$) db 0
    dw 0xAA55

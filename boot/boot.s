;
; boot.s
;
; boot被bios加载到物理地址0x07C00处，boot的大小为512字节。
; boot的作用是加载setup到合适的位置,即加载到0x07E00处。
; boot还需要加载kernel到合适的位置,即加载到0x10000处。

%include "boot.inc"

;系统只是保证加载首扇区到物理地址的0x07C00处并将跳转到此物理地址执行，
;但是不保证cs和offset是否为我们想要的，现在我们通过跳转把段寄存器设置
;为我们需要的值 0x07C0。
jmp    SEG_BOOT:main

msg    db    "Loading Boot Stage2..."
len    equ    $-msg

main:
    ;读取当前光标位置
    mov    ah,0x03
    xor    bh,bh                ;bh = page
    int    0x10

    ;移动到下行行首
    inc    dh
    xor    dl,dl

    ;显示字符串
    mov     ax,cs
    mov     es,ax               ;es = seg
    mov     bp,msg              ;bp = offset
    mov     cx,len              ;cx = len
    mov     bx,0x0003           ;bh = page, bl = attribute
    mov     ax,0x1301
    int     0x10

    ;加载setup到0x07E0:0000
    mov     ax,SEG_SETUP
    mov     es,ax               ;es = seg
    mov     bx,0x0000           ;bx = offset
    mov     ch,0x00             ;ch = track
    mov     dx,0x0000           ;dh = head, dl = drive
    mov     cl,FD_SETUP         ;cl = sector
    mov     al,FD_SETUP_SIZE    ;al = number of sectors
    mov     ah,0x02
    int     0x13

    ;加载kenerl到0x1000:0000
    mov     ax,SEG_KERNEL
    mov     es,ax
    mov     bx,0x0000           ;bx = offset
    mov     ch,0x00             ;ch = track
    mov     dx,0x0000           ;dh = head, dl = drive
    mov     cl,FD_KERNEL        ;cl = sector
    mov     al,FD_KERNEL_SIZE   ;al = number of sectors
    mov     ah,0x02
    int     0x13

    ; 读出成功的话跳转到boot2继续执行
    jmp     SEG_SETUP:0x0000
    jmp     $

    ;如果失败，重置硬盘再次加载boot2
    ;mov    ah,0x00
    ;int    0x13
    ;jmp    boot2_load

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; boot flag
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    times 510-($-$$) db 0
    dw 0xAA55

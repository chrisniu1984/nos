;
; heas.s (nasm)
;
; 将地址指针保存到全局变量，这只栈指针，跳到main函数
;

GLOBAL _start
GLOBAL _GDT, _GDT_LIMIT, _IDT, _IDT_LIMIT

extern main

[BITS 32]
_start:
    xor    eax, eax
    mov    ax, 0x10     ; DS
    mov    ds, ax
    mov    es, ax
    mov    fs, ax
    mov    ss, ax
    mov    esp, 0x2000
    mov    ax, 0x18
    mov    gs, ax       ; Video

    sgdt   [_TMP]       ; _GDT
    mov    ax, [_TMP]
    mov    [_GDT_LIMIT], ax;
    mov    eax, [_TMP+2]
    mov    [_GDT], eax;

    sidt   [_TMP]       ; _IDT
    mov    ax, [_TMP]
    mov    [_IDT_LIMIT], ax;
    mov    eax, [_TMP+2]
    mov    [_IDT], eax;

    jmp    main

_TMP        times 48 db 0

_GDT        dd 0x00000000
_GDT_LIMIT  dw 0x0000

_IDT        dd 0x00000000
_IDT_LIMIT  dw 0x0000


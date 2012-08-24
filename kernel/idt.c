
#include <types.h>
#include <vga.h>
#include <idt.h>

struct idt_reg_t {
    __u16 len;
    __u32 addr;
}__attribute__ ((packed));

struct idt_gate_t {
    __u16   offset_l;
    __u16   selector;

    __u16   resv:8;
    __u16   type:4;
    __u16   s:1;
    __u16   dpl:2;
    __u16   p:1;

    __u16   offset_h;
}__attribute__ ((packed));

static struct idt_reg_t m_idr;
static struct idt_gate_t *m_idt = NULL;

void idt_trap_resv(void)
{
    vga_string("idt_trap_resv\n\0");
}

void idt_int_timer(void)
{
    vga_string("idt_int_timer\n\0");
    __asm__("sti":::);
}

void idt_int_keyboard(void)
{
    vga_string("idt_keyboard\n\0");
    __asm__("sti":::);
}

void idt_set_trap(int n, int dpl, void *addr)
{
    struct idt_gate_t *gate = m_idt+n;

    gate->offset_l = (unsigned short) (unsigned long) addr;
    gate->selector = 0x0008;
    gate->resv = 0x00;
    gate->type = 0x0F;
    gate->s = 0;
    gate->dpl = dpl;
    gate->p = 1;
    gate->offset_h = (unsigned short) ((unsigned long)(addr)>>16);
}

void idt_set_int(int n, int dpl, void *addr)
{
    struct idt_gate_t *gate = m_idt + n;

    gate->offset_l = (unsigned short) (unsigned long) addr;
    gate->selector = 0x0008;
    gate->resv = 0x00;
    gate->type = 0x0E;
    gate->s = 0;
    gate->dpl = dpl;
    gate->p = 1;
    gate->offset_h = (unsigned short) ((unsigned long)(addr)>>16);
}

void idt_init()
{
    int i= 0;

    // 获得idt地址指针
    __asm__("sidt %0;":"=m"(m_idr)::);
    m_idt = (struct idt_gate_t *) (m_idr.addr-0x10000); // 从物理地址减去段基址

    idt_set_trap(0, 0, idt_trap_resv);   // Divide-by-zero    fault
    idt_set_trap(1, 0, idt_trap_resv);   // Debug exception    trap or fault
    idt_set_trap(2, 0, idt_trap_resv);   // Non-Maskable Interrupt (NMI)    trap
    idt_set_trap(3, 0, idt_trap_resv);   // Breakpoint (INT 3)    trap
    idt_set_trap(4, 0, idt_trap_resv);   // Overflow (INTO with EFlags[OF] set)    trap
    idt_set_trap(5, 0, idt_trap_resv);   // Bound exception (BOUND on out-of-bounds access)    trap
    idt_set_trap(6, 0, idt_trap_resv);   // Invalid Opcode    trap
    idt_set_trap(7, 0, idt_trap_resv);   // FPU not available    trap
    idt_set_trap(8, 0, idt_trap_resv);   // Double Fault    abort
    idt_set_trap(9, 0, idt_trap_resv);   // Coprocessor Segment Overrun    abort
    idt_set_trap(10, 0, idt_trap_resv);  // Invalid TSS    fault
    idt_set_trap(11, 0, idt_trap_resv);  // Segment not present    fault
    idt_set_trap(12, 0, idt_trap_resv);  // Stack exception    fault
    idt_set_trap(13, 0, idt_trap_resv);  // General Protection    fault or trap
    idt_set_trap(14, 0, idt_trap_resv);  // Page fault    fault
    idt_set_trap(15, 0, idt_trap_resv);  // Reserved
    idt_set_trap(16, 0, idt_trap_resv);  // Floating-point error    fault
    idt_set_trap(17, 0, idt_trap_resv);  // Alignment Check    fault
    idt_set_trap(18, 0, idt_trap_resv);  // Machine Check    abort
    for (i=19; i<=31; i++) {
        idt_set_trap(i, 0, idt_trap_resv); //19-31    Reserved By Intel
    }

    idt_set_int(32, 0, idt_int_timer); // timer
    idt_set_int(33, 0, idt_int_keyboard); // keyboard

    __asm__("sti":::);
}


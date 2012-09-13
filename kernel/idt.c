#include <types.h>
#include <vga.h>
#include <io.h>
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
#define CLK_8253    (1193187)
#define INIT_8253   (CLK_8253/HZ)

int idt_trap_resv(void)
{
    //vga_string("idt_trap_resv\n\0");
    return 0;
}

int idt_int_timer(void)
{
    static unsigned int cnt = 0;
    static unsigned char a = '0';

    if (cnt==0) {
        vga_put_xy(a, 0, 0);

        a += 1;
        if (a>'9') {
            a = '0';
        }
    }
    cnt = (cnt+1) % HZ;

    sti();
    eoi();

    return 0;
}

int idt_int_keyboard(void)
{
    vga_string("idt_keyboard\n\0");
    eoi();
    sti();
    return 0;
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

    // timer 8253 PIT
	outb(0x43, 0x36);		/* binary, mode 3, LSB/MSB, ch 0 */
	outb(0x40, INIT_8253&0xFF);	/* LSB */
	outb(0x40, INIT_8253>>8);	/* MSB */
    idt_set_int(32, 0, idt_int_timer);
    outb(0x21, inb(0x21)&0xFE);

    // keyboard
    idt_set_int(33, 0, idt_int_keyboard);
    outb(0x21, inb(0x21)&0xFD);

    // 开中断
    sti();
}


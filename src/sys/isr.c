#include <stdint.h>
#include <inc/devices/video/vbe.h>
#include <inc/sys/idt.h>
#include <inc/sys/ports.h>

extern void isr_stub(void);
extern void isr_keyb(void);
extern void isr_exception(void);
extern void isr_irq_master(void);
extern void isr_irq_slave(void);

void OsSys_ISR_Keyboard(void) {
    char key = OsSys_Ports_Inb(0x60);
    OsDriver_Video_VBE_Puts("\nKey received! ", 0x07DA63);
    OsDriver_Video_VBE_Putc(key, 0xFFFFFF);
}

void OsSys_ISR_Exception(void) {
    OsDriver_Video_VBE_Puts("\nException!", 0xFF0000);
    for (;;) {}
}

void OsSys_ISR_Init(void) {
    for (int i = 0; i < 32; i++) {
        OsSys_IDT_Set_Gate(i, (uint64_t) isr_exception);
    }

    OsSys_IDT_Set_Gate(31, (uint64_t) isr_irq_master);
    OsSys_IDT_Set_Gate(32, (uint64_t) isr_keyb);
    for (int i = 33; i < 33 + 6; i++) {
        OsSys_IDT_Set_Gate(i, (uint64_t) isr_irq_master);
    }
    for (int i = 39; i < 15 + 32 + 1; i++) {
        OsSys_IDT_Set_Gate(i, (uint64_t) isr_irq_slave);
    }
    for (int i = 15 + 32; i < 256; i++) {
        OsSys_IDT_Set_Gate(i, (uint64_t) isr_stub);
    }
}

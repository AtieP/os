#include <stdint.h>
#include <inc/devices/video/vbe.h>
#include <inc/sys/idt.h>
#include <inc/sys/pic.h>
#include <inc/sys/ports.h>

extern void isr_stub(void);
extern void isr_timer(void);
extern void isr_keyb(void);
extern void isr_exception(void);
extern void isr_irq_master(void);
extern void isr_irq_slave(void);

static const uint8_t ascii_nomod[] = {
    '\0', '\e', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' '
};

void OsSys_ISR_Stub(void) {
    OsDriver_Video_VBE_Puts("\n[ISR] Stub", 0xE5E5E5);
}

void OsSys_ISR_Timer(void) {
    OsSys_PIC_Master_EOI();
}

void OsSys_ISR_Keyboard(void) {
    uint8_t key = OsSys_Ports_Inb(0x60);
    if (key > 0x57) {
        OsSys_PIC_Master_EOI();
        return;
    }
    OsDriver_Video_VBE_Puts("\n[ISR] Key received: ", 0x07DA63);
    OsDriver_Video_VBE_Putc(ascii_nomod[key], 0x07DA63);
    OsSys_PIC_Master_EOI();
}

void OsSys_ISR_Exception(void) {
    OsDriver_Video_VBE_Puts("\n[ISR] Exception!", 0xFF0000);
    for (;;) {}
}

void OsSys_ISR_Init(void) {
    for (int i = 0; i < 0x21; i++) {
        OsSys_IDT_Set_Gate(i, (uint64_t) isr_exception);
    }
    OsSys_IDT_Set_Gate(0x20, (uint64_t) isr_timer);
    OsSys_IDT_Set_Gate(0x21, (uint64_t) isr_keyb);
    for (int i = 0x22; i < 0x28; i++) {
        OsSys_IDT_Set_Gate(i, (uint64_t) isr_irq_master);
    }
    for (int i = 0x28; i < 0x2F; i++) {
        OsSys_IDT_Set_Gate(i, (uint64_t) isr_irq_slave);
    }
    for (int i = 0x30; i < 256; i++) {
        OsSys_IDT_Set_Gate(i, (uint64_t) isr_stub);
    }
}

#include <stdint.h>
#include <inc/sys/idt.h>

static idt_gate_t idt_gates[256];
static idt_register_t idt_register;

void OsSys_IDT_Set_Gate(uint8_t number, uint64_t handler) {
	idt_gates[number].offset_low = (uint16_t) (handler); // Lower 16 bits
	idt_gates[number].offset_middle = (uint16_t) (handler >> 16); // Middle 16 bits
	idt_gates[number].offset_high = (uint32_t) (handler >> 32); // High 32 bits
	idt_gates[number].selector = 0x08;
	idt_gates[number].ist = 0;
	idt_gates[number].type = 0b10001110; // Interrupt gate
	idt_gates[number].null = 0;
}

void OsSys_IDT_Set_IST(uint8_t number, uint8_t ist_index) {
	idt_gates[number].ist = (ist_index & 0b0111);
}

void OsSys_IDT_Init(void) {
	idt_register.base = (uint64_t) idt_gates;
	idt_register.limit = sizeof(idt_register_t) * 256 - 1;
	asm volatile("lidtq (%0)" :: "r"(&idt_register));
}

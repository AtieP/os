#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>

typedef struct {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t type;
	uint16_t offset_middle;
	uint32_t offset_high;
	uint32_t null;
} __attribute__((packed)) idt_gate_t;

typedef struct {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) idt_register_t;

void OsSys_IDT_Set_Gate(uint8_t number, uint64_t handler);
void OsSys_IDT_Set_IST(uint8_t number, uint8_t ist_index);
void OsSys_IDT_Init(void);

#endif
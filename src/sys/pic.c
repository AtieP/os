#include <inc/sys/pic.h>
#include <inc/sys/ports.h>

#define io_wait() OsSys_Ports_Outb(0x80, 0x00);

void OsSys_PIC_Master_EOI(void) {
	OsSys_Ports_Outb(0x20, 0x20);
}

void OsSys_PIC_Slave_EOI(void) {
	OsSys_Ports_Outb(0xA0, 0x20);
	OsSys_Ports_Outb(0x20, 0x20);
}

void OsSys_PIC_Init(void) {
	uint8_t master_mask, slave_mask;

	master_mask = OsSys_Ports_Inb(PIC_MASTER_DATA);
	slave_mask = OsSys_Ports_Inb(PIC_SLAVE_COMMAND);

	// Restart the PIC
	OsSys_Ports_Outb(PIC_MASTER_COMMAND, 0x11);
	io_wait();
	OsSys_Ports_Outb(PIC_SLAVE_COMMAND, 0x11);
	io_wait();

	// Set offsets
	// IRQ 0 = 0x20
	OsSys_Ports_Outb(PIC_MASTER_DATA, 0x20);
	io_wait();
	OsSys_Ports_Outb(PIC_SLAVE_DATA, 0x28);
	io_wait();

	// Set up cascading
	OsSys_Ports_Outb(PIC_MASTER_DATA, 0x04);
	io_wait();
	OsSys_Ports_Outb(PIC_SLAVE_DATA, 0x02);

	OsSys_Ports_Outb(PIC_MASTER_DATA, 0x01);
	io_wait();
	OsSys_Ports_Outb(PIC_SLAVE_DATA, 0x01);
	io_wait();

	// Set some masks ;)
	OsSys_Ports_Outb(PIC_MASTER_DATA, 0xFC);
	OsSys_Ports_Outb(PIC_SLAVE_DATA, 0xFF);
}
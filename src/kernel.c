#include <inc/boot/stivale.h>
#include <inc/devices/video/vbe.h>
#include <inc/devices/video/vga.h>
#include <inc/misc/font.h>
#include <inc/sys/idt.h>
#include <inc/sys/isr.h>
#include <inc/sys/pic.h>
#include <inc/sys/ports.h>

void kmain(struct stivale_struct *bootloader_info) {
	if (!bootloader_info->framebuffer_addr) {
		// VGA mode
		OsDriver_Video_VGA_Init();
		OsDriver_Video_VGA_Puts("Error: you should be using VBE graphics.", BLACK, LIGHT_RED);
		return;
	}
	OsDriver_Video_VBE_Init(
		(uint32_t *) bootloader_info->framebuffer_addr,
		bootloader_info->framebuffer_width,
		bootloader_info->framebuffer_height,
		bootloader_info->framebuffer_pitch / 4,
		bootloader_info->framebuffer_bpp,
		font,
		font_width,
		font_height
	);
	OsDriver_Video_VBE_Puts("Hello", 0xFFFFF);
	OsSys_PIC_Init();
	OsSys_ISR_Init();
	OsSys_IDT_Init();
	asm volatile("sti");
	OsDriver_Video_VBE_Puts("\nI SURVIVED LMAOOO", 0xFFFFF);
	while (1) {}
}

#include <stdint.h>
#include <inc/devices/video/vga.h>
#include <inc/sys/ports.h>

static volatile uint16_t *vga_buffer = (uint16_t *) VGA_ADDRESS;
static uint8_t vga_cursor_x;
static uint8_t vga_cursor_y;

static int vga_mode_enabled = 0; // bool

void OsDriver_Video_VGA_Init(void) {
	vga_mode_enabled = 1;
	OsSys_Ports_Outb(0x3D4, 0x0A);
	OsSys_Ports_Outb(0x3D5, 0x20);

	OsDriver_Video_VGA_Clear_screen(BLACK, WHITE);
}

void OsDriver_Video_VGA_Clear_screen(VgaColors bg_color, VgaColors fg_color) {
	if (!vga_mode_enabled) {
		return;
	}

	vga_cursor_x = 0;
	vga_cursor_y = 0;
	for (int i = 0; i < VGA_BUFFER_SIZE; i++) {
		vga_buffer[i] = 0x00 | (uint16_t)((uint8_t) fg_color | (uint8_t) bg_color << 4) << 8;
	}
}

void OsDriver_Video_VGA_Putc(const char ch, VgaColors bg_color, VgaColors fg_color) {
	if (!vga_mode_enabled) {
		return;
	}

	uint16_t index = (uint16_t) (vga_cursor_y * 80 + vga_cursor_x);
	vga_buffer[index] = (uint16_t) ch | (uint16_t)((uint8_t) fg_color | (uint8_t) bg_color << 4) << 8;
	if (++vga_cursor_x == 81) {
		vga_cursor_x = 0;
		if (++vga_cursor_y == 25) {
			OsDriver_Video_VGA_Scroll();
		}
	}
}

void OsDriver_Video_VGA_Puts(const char *string, VgaColors bg_color, VgaColors fg_color) {
	if (!vga_mode_enabled) {
		return;
	}

	while (*string) {
		OsDriver_Video_VGA_Putc(*string, bg_color, fg_color);
		string++;
	}
}

void OsDriver_Video_VGA_Scroll(void) {
	if (!vga_mode_enabled) {
		return;
	}

	int a = 0;
	int b = 80;
	for (int i = 0; i < 80 * 24 / 2; i++) {
		vga_buffer[a] = vga_buffer[b];
		a++;
		b++;
	}
	vga_cursor_y--;
}

void OsDriver_Video_VGA_Exit(void) {
	vga_mode_enabled = 0;
}

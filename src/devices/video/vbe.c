#include <stdint.h>
#include <inc/devices/video/vbe.h>

static fb_t framebuffer;

static int vbe_cursor_x;
static int vbe_cursor_y;
static int vbe_enabled = 0;

const fb_t OsDriver_Video_VBE_Init(
	uint32_t *address,
	uint16_t width,
	uint16_t height,
	uint16_t pitch,
	uint16_t bpp,
	unsigned char *font_address,
	unsigned char font_width,
	unsigned char font_height
) {
	framebuffer.address = address;
	framebuffer.width = width;
	framebuffer.height = height;
	framebuffer.pitch = pitch;
	framebuffer.bpp = bpp;
	framebuffer.font_address = font_address;
	framebuffer.font_width = font_width;
	framebuffer.font_height = font_height;
	framebuffer.max_chars_width = width / font_width;
	framebuffer.max_chars_height = height / font_height;
	return framebuffer;
}

void OsDriver_Video_VBE_Putc(const char ch, uint32_t color) {
	if (ch == '\n') {
		vbe_cursor_x = 0;
		vbe_cursor_y++;
	} else {
		uint8_t *a = framebuffer.font_address + (uint16_t) ch * ((framebuffer.font_width / 8) * framebuffer.font_height);
		for (int i = 0; i < framebuffer.font_height; i++) {
			for (int j = 0, bit = framebuffer.font_width; j < framebuffer.font_width; j++) {
				if (a[i] & (1 << --bit)) {
					framebuffer.address[(i + (vbe_cursor_y * framebuffer.font_height)) * framebuffer.pitch + j + (vbe_cursor_x * framebuffer.font_width)] = color;
				}
			}
		}
		vbe_cursor_x++;
	}
	if (vbe_cursor_x == framebuffer.max_chars_width) {
		vbe_cursor_x = 0;
		vbe_cursor_y++;
	}
	if (vbe_cursor_y == framebuffer.max_chars_height) {
		OsDriver_Video_VBE_Scroll();
		vbe_cursor_y--;
	}
}

void OsDriver_Video_VBE_Puts(const char *string, uint32_t color) {
	while (*string) {
		OsDriver_Video_VBE_Putc(*string, color);
		string++;
	}
}

void OsDriver_Video_VBE_Scroll(void) {
	uint32_t row_size = framebuffer.pitch * framebuffer.font_height;
	uint32_t screen_size = framebuffer.pitch * framebuffer.max_chars_height * framebuffer.font_height;
	// The actual move
	for (int i = 0; i < screen_size - row_size; i++) {
		framebuffer.address[i] = framebuffer.address[i + row_size];
	}
	// The bottom
	for (int i = screen_size - row_size; i < screen_size; i++) {
		framebuffer.address[i] = 0;
	}
}

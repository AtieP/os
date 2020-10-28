#ifndef __VBE_H__
#define __VBE_H__

#include <stdint.h>

typedef struct {
	volatile uint32_t *address;
	uint16_t width;
	uint16_t height;
	uint16_t pitch;
	uint16_t bpp;
	unsigned char *font_address;
	unsigned char font_width;
	unsigned char font_height;
	unsigned char max_chars_width;
	unsigned char max_chars_height;
} fb_t;

const fb_t OsDriver_Video_VBE_Init(
	uint32_t *address,
	uint16_t width,
	uint16_t height,
	uint16_t pitch,
	uint16_t bpp,
	unsigned char *font_address,
	unsigned char font_width,
	unsigned char font_height
);
void OsDriver_Video_VBE_Putc(const char ch, uint32_t color);
void OsDriver_Video_VBE_Puts(const char *string, uint32_t color);
void OsDriver_Video_VBE_Scroll(void);

#endif

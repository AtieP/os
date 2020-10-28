#ifndef __VGA_H__
#define __VGA_H__

#define VGA_ADDRESS 0xb8000
#define VGA_BUFFER_SIZE 2000

typedef enum {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHT_GRAY,
	DARK_GRAY,
	LIGHT_BLUE,
	LIGHT_GREEN,
	LIGHT_CYAN,
	LIGHT_RED,
	LIGHT_MAGENTA,
	YELLOW,
	WHITE
} VgaColors;

void OsDriver_Video_VGA_Init(void);
void OsDriver_Video_VGA_Clear_screen(VgaColors bg_color, VgaColors fg_color);
void OsDriver_Video_VGA_Putc(const char ch, VgaColors bg_color, VgaColors fg_color);
void OsDriver_Video_VGA_Puts(const char *string, VgaColors bg_color, VgaColors fg_color);
void OsDriver_Video_VGA_Scroll(void);
void OsDriver_Video_VGA_Exit(void);

#endif

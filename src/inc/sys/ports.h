#ifndef __PORTS_H__
#define __PORTS_H__

#include <stdint.h>

static inline uint8_t OsSys_Ports_Inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %%dx, %%al" : "=a" (ret) : "d" (port));
    return ret;
}

static inline void OsSys_Ports_Outb(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

#endif
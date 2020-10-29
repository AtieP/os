#ifndef __PIC_H__
#define __PIC_H__

#define PIC_MASTER 0x20
#define PIC_SLAVE 0xA0
#define PIC_MASTER_COMMAND 0x20
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_DATA 0xA1

void OsSys_PIC_Master_EOI(void);
void OsSys_PIC_Slave_EOI(void);
void OsSys_PIC_Init(void);

#endif

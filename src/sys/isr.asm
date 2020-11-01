global isr_stub
global isr_timer
global isr_keyb
global isr_exception
global isr_irq_master
global isr_irq_slave

extern OsSys_ISR_Stub
extern OsSys_ISR_Timer
extern OsSys_ISR_Keyboard
extern OsSys_ISR_Exception

isr_stub:
	call OsSys_ISR_Stub
	iretq

isr_timer:
    call OsSys_ISR_Timer
	iretq

isr_keyb:
	call OsSys_ISR_Keyboard
	iretq

isr_irq_master:
	mov al, 0x20
	out 0x20, al
	iretq

isr_irq_slave:
	mov al, 0x20
	out 0xA0, al
	out 0x20, al
	iretq

isr_exception:
	call OsSys_ISR_Exception
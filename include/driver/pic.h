#ifndef INCLUDE_DRIVER_PIC_H_
#define INCLUDE_DRIVER_PIC_H_

#include <arch.h>

// ����8259AоƬ
void init_interrupt_chip(void);

// ���� 8259A оƬ
void clear_interrupt_chip(uint32_t intr_no);

#endif  // INCLUDE_DRIVER_PIC_H_

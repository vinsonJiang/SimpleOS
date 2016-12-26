#ifndef INCLUDE_DRIVER_PIC_H_
#define INCLUDE_DRIVER_PIC_H_

#include <arch.h>

// …Ë÷√8259A–æ∆¨
void init_interrupt_chip(void);

// ÷ÿ…Ë 8259A –æ∆¨
void clear_interrupt_chip(uint32_t intr_no);

#endif  // INCLUDE_DRIVER_PIC_H_

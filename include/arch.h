#ifndef ARCH_H_
#define ARCH_H_

#include <mm/gdt.h>
#include <intr/intr.h>
#include <driver/pic.h>
//#include <driver/clock.h>
#include <driver/console.h>
//#include <task/task.h>

// �ܹ���صĳ�ʼ��
void arch_init(void);

#endif  // ARCH_H_

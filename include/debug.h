
#ifndef INCLUDE_DEBUG_H_
#define INCLUDE_DEBUG_H_

#include <stdarg.h>
#include <elf.h>
#include <error.h>
#include <driver/console.h>

// �ں˴�������ڴ��е���ʼ�ͽ���λ�ã������ӽű��ж���
extern uint8_t kern_init_text_start[];
extern uint8_t kern_init_text_end[];
extern uint8_t kern_init_data_start[];
extern uint8_t kern_init_data_end[];
extern uint8_t kern_text_start[];
extern uint8_t kern_text_end[];
extern uint8_t kern_data_start[];
extern uint8_t kern_data_end[];

#define assert(x, info)                                         \
        do {                                                    \
                if (!(x)) {                                     \
                        panic(info);                            \
                }                                               \
        } while (0)

// ��ʼ�� Debug ��Ϣ
void debug_init(void);

// ��� BIOS �ṩ�������ڴ沼��
void show_memory_map(void);

// ����ں˵�ǰռ�ݵ��ڴ��ַ
void show_kernel_memory_map(void);

// ��ӡ��ǰ�ĺ�������ջ��Ϣ
void panic(const char *msg);

// ��ӡ��ǰ�Ķδ���ֵ
void print_cur_status(void);

// �ں˵Ĵ�ӡ����
void printk(const char *format, ...);

// �ں˵Ĵ�ӡ����(����ɫ)
void cprintk(real_color_t back, real_color_t fore, const char *format, ...);

#endif  // INCLUDE_DEBUG_H_

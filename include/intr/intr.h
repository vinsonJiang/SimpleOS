#ifndef INCLUDE_INTR_INTR_H_
#define INCLUDE_INTR_INTR_H_

//#ifndef IDT_H_
//#define IDT_H_

#include <types.h>

// �жϱ���ļĴ�������
typedef
struct pt_regs_t {

	// ���ڱ����û������ݶ�������
	uint16_t ds;
	uint16_t padding1;

	// �� edi �� eax �� pusha ָ��ѹջ
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t oesp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	// �жϺ�(�ں˴�������ѹջ)
	uint32_t int_no;

	// �������(���жϴ��������жϻ���CPUѹջ)
	uint32_t err_code;

	// �����ɴ������Զ�ѹջ
	uint32_t eip;
	uint16_t cs;
	uint16_t padding2;
	uint32_t eflags;

	// �����������Ȩ�����л�CPU��ѹջ
	uint32_t esp;
	uint16_t ss;
	uint16_t padding3;
} pt_regs_t;

// �����жϴ�����ָ��
typedef void(*interrupt_handler_t)(pt_regs_t *);

// �����жϴ�����
void isr_handler(pt_regs_t *regs);

// ע��һ���жϴ�����
void register_interrupt_handler(uint8_t n, interrupt_handler_t h);

// �жϺŶ���
#define INT_DIVIDE_ERROR         0
#define INT_DEBUG                1
#define INT_NMI                  2
#define INT_BREAKPOINT           3
#define INT_OVERFLOW             4
#define INT_BOUND                5
#define INT_INVALID_OPCODE       6
#define INT_DEVICE_NOT_AVAIL     7
#define INT_DOUBLE_FAULT         8
#define INT_COPROCESSOR          9
#define INT_INVALID_TSS         10
#define INT_SEGMENT             11
#define INT_STACK_FAULT         12
#define INT_GENERAL_PROTECT     13
#define INT_PAGE_FAULT          14

#define INT_X87_FPU             16
#define INT_ALIGNMENT           17
#define INT_MACHINE_CHECK       18
#define INT_SIMD_FLOAT          19
#define INT_VIRTUAL_EXCE        20

// �����жϴ����� 0-19 ���� CPU ���쳣�ж�
// ISR:�жϷ������(interrupt service routine)
void isr0();            // 0 #DE �� 0 �쳣 
void isr1();            // 1 #DB �����쳣 
void isr2();            // 2 NMI 
void isr3();            // 3 BP �ϵ��쳣 
void isr4();            // 4 #OF ��� 
void isr5();            // 5 #BR ����������ó����߽� 
void isr6();            // 6 #UD ��Ч��δ����Ĳ����� 
void isr7();            // 7 #NM �豸������(����ѧЭ������) 
void isr8();            // 8 #DF ˫�ع���(�д������) 
void isr9();            // 9 Э��������β��� 
void isr10();           // 10 #TS ��ЧTSS(�д������) 
void isr11();           // 11 #NP �β�����(�д������) 
void isr12();           // 12 #SS ջ����(�д������) 
void isr13();           // 13 #GP ���汣��(�д������) 
void isr14();           // 14 #PF ҳ����(�д������) 
void isr15();           // 15 CPU ���� 
void isr16();           // 16 #MF ���㴦��Ԫ���� 
void isr17();           // 17 #AC ������ 
void isr18();           // 18 #MC ������� 
void isr19();           // 19 #XM SIMD(��ָ�������)�����쳣

						// 20-31 Intel ����
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// 32��255 �û��Զ����쳣
void isr128();

// IRQ ������
void irq_handler(pt_regs_t *regs);

// ����IRQ
#define  IRQ0     32    // ����ϵͳ��ʱ��
#define  IRQ1     33    // ����
#define  IRQ2     34    // �� IRQ9 ��ӣ�MPU-401 MD ʹ��
#define  IRQ3     35    // �����豸
#define  IRQ4     36    // �����豸
#define  IRQ5     37    // ��������ʹ��
#define  IRQ6     38    // �����������ʹ��
#define  IRQ7     39    // ��ӡ���������ʹ��
#define  IRQ8     40    // ��ʱʱ��
#define  IRQ9     41    // �� IRQ2 ��ӣ����趨������Ӳ��
#define  IRQ10    42    // ��������ʹ��
#define  IRQ11    43    // ���� AGP �Կ�ʹ��
#define  IRQ12    44    // �� PS/2 ��꣬Ҳ���趨������Ӳ��
#define  IRQ13    45    // Э������ʹ��
#define  IRQ14    46    // IDE0 �������ʹ��
#define  IRQ15    47    // IDE1 �������ʹ��

// ���� IRQ ����
// IRQ:�ж�����(Interrupt Request)
void irq0();            // ����ϵͳ��ʱ��
void irq1();            // ����
void irq2();            // �� IRQ9 ��ӣ�MPU-401 MD ʹ��
void irq3();            // �����豸
void irq4();            // �����豸
void irq5();            // ��������ʹ��
void irq6();            // �����������ʹ��
void irq7();            // ��ӡ���������ʹ��
void irq8();            // ��ʱʱ��
void irq9();            // �� IRQ2 ��ӣ����趨������Ӳ��
void irq10();           // ��������ʹ��
void irq11();           // ���� AGP �Կ�ʹ��
void irq12();           // �� PS/2 ��꣬Ҳ���趨������Ӳ��
void irq13();           // Э������ʹ��
void irq14();           // IDE0 �������ʹ��
void irq15();           // IDE1 �������ʹ��

						// ��ʼ���ж���������
void idt_init(void);


#endif // !INCLUDE_INTR_INTR_H_

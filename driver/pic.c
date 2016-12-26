#include <common.h>
#include <driver/pic.h>

#define IO_PIC1   (0x20)	  // Master (IRQs 0-7)
#define IO_PIC2   (0xA0)	  // Slave  (IRQs 8-15)

#define IO_PIC1C  (IO_PIC1+1)
#define IO_PIC2C  (IO_PIC2+1)

// ���� 8259A оƬ
void init_interrupt_chip(void)
{
	// ����ӳ�� IRQ ��
	// ��Ƭ������ Intel 8259A оƬ
	// ��Ƭ�˿� 0x20 0x21
	// ��Ƭ�˿� 0xA0 0xA1

	// ��ʼ����Ƭ����Ƭ
	// 0001 0001
	outb(IO_PIC1, 0x11);
	outb(IO_PIC2, 0x11);

	// ������Ƭ IRQ �� 0x20(32) ���жϿ�ʼ
	outb(IO_PIC1C, 0x20);

	// ���ô�Ƭ IRQ �� 0x28(40) ���жϿ�ʼ
	outb(IO_PIC2C, 0x28);

	// ������Ƭ IR2 �������Ӵ�Ƭ
	outb(IO_PIC1C, 0x04);

	// ���ߴ�Ƭ������ź���Ƭ IR2 ������
	outb(IO_PIC2C, 0x02);

	// ������Ƭ�ʹ�Ƭ���� 8086 �ķ�ʽ����
	outb(IO_PIC1C, 0x01);
	outb(IO_PIC2C, 0x01);

	// ��������Ƭ�����ж�
	outb(IO_PIC1C, 0x0);
	outb(IO_PIC2C, 0x0);
}

// ���� 8259A оƬ
void clear_interrupt_chip(uint32_t intr_no)
{
	// �����жϽ����źŸ� PICs
	// �������ǵ����ã��� 32 ���ж���Ϊ�û��Զ����ж�
	// ��Ϊ��Ƭ�� Intel 8259A оƬֻ�ܴ��� 8 ���ж�
	// �ʴ��ڵ��� 40 ���жϺ����ɴ�Ƭ�����
	if (intr_no >= 40) {
		// ���������źŸ���Ƭ
		outb(IO_PIC2, 0x20);
	}
	// ���������źŸ���Ƭ
	outb(IO_PIC1, 0x20);
}


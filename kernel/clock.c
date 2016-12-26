#include <debug.h>
#include <common.h>
#include <arch.h>
#include <sched.h>

#include <clock.h>

#define FREQUENCY       100

// ��ʱ�ж��� 8253/8254 оƬ�� IRQ0 �ṩ
#define IO_TIMER        0x040               // 8253 Timer #1

// ����Ƶ��Ϊ 1193180��frequency ��ÿ���жϴ���
#define TIMER_FREQ      1193180

// Intel 8253/8254 PITоƬ I/O�˿ڵ�ַ��Χ��40h~43h
#define TIMER_MODE      (IO_TIMER + 3)         // timer mode port

#define TIMER_SEL0      0x00                    // select counter 0
#define TIMER_RATEGEN   0x04                    // mode 2
#define TIMER_CLK       0x06                    // mode 3
#define TIMER_16BIT     0x30                    // r/w counter 16 bits, LSB first

void clock_init(void)
{
	// ע��ʱ����صĴ�����
	register_interrupt_handler(IRQ0, clock_callback);

	uint32_t divisor = TIMER_FREQ / FREQUENCY;

	outb(TIMER_MODE, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);

	// ��ֵ��ֽں͸��ֽ�
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t hign = (uint8_t)((divisor >> 8) & 0xFF);

	// �ֱ�д����ֽں͸��ֽ�
	outb(IO_TIMER, low);
	outb(IO_TIMER, hign);
}


#include <debug.h>
#include <arch.h>
#include <mm/mm.h>
#include <common.h>
#include <kio.h>
#include <device.h>
#include <driver/console.h>
#include <fs/fs.h>

//�ܹ���ʼ��
void arch_init(void)
{
	gdt_init();//ȫ�����������ʼ��
	idt_init();//�ж����������ʼ��
	//clock_init();//ʱ�ӳ�ʼ��
	console_init();//����̨��ʼ��
}

//�ں˳�ʼ��
int kern_init()
{
	//�ں˳�ʼ��
	debug_init();//���Գ�ʼ��
	arch_init();//�ܹ���ʼ��
	mm_init();//�ڴ��ʼ��
	device_init();//�豸��ʼ�����ַ��豸�����豸��
	fs_init();//�ļ�ϵͳ��ʼ��

	enable_intr();//���ж�


	printk("kernel in memory start:0x%08X  \n", kern_start);
	printk("kernel in memory end:0x%08X  \n", kern_end);
	printk("kernel in memory used:%d kb  \n\n", (kern_end - kern_start + 1023) / 1024);

	show_memory_map();

	//printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);

	uint32_t allc_addr = NULL;
	cprintk(rc_black, rc_light_brown, "Test Physical Memory Alloc :\n");
	allc_addr = alloc_page;
	cprintk(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = alloc_page;
	cprintk(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = alloc_page;
	cprintk(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = alloc_page;
	cprintk(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);


	uint8_t ch = 0;
	while (true) {
	    ch = getchar();
		putchar(ch, rc_black, rc_green);
	}

}
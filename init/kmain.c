#include <debug.h>
#include <arch.h>
#include <mm/mm.h>
#include <common.h>
#include <kio.h>
#include <device.h>
#include <driver/console.h>
#include <fs/fs.h>

//架构初始化
void arch_init(void)
{
	gdt_init();//全局描述符表初始化
	idt_init();//中断描述符表初始化
	//clock_init();//时钟初始化
	console_init();//控制台初始化
}

//内核初始化
int kern_init()
{
	//内核初始化
	debug_init();//调试初始化
	arch_init();//架构初始化
	mm_init();//内存初始化
	device_init();//设备初始化（字符设备、块设备）
	fs_init();//文件系统初始化

	enable_intr();//开中断


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
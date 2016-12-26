#include <device.h>
#include <driver/char_dev.h>

// 外部设备初始化
void device_init(void)
{
	char_dev_init();//字符设备初始化
	block_dev_init();//块设备初始化
}


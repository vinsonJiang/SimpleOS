#include <device.h>
#include <driver/char_dev.h>

// �ⲿ�豸��ʼ��
void device_init(void)
{
	char_dev_init();//�ַ��豸��ʼ��
	block_dev_init();//���豸��ʼ��
}


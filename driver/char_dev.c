/*
�ַ��豸����
2016/12/22
Vinson
*/

#include <debug.h>
#include <driver/char_dev.h>
#include <lib/string.h>

// ȫ���ַ��豸����
char_dev_t *char_devs;

// �ַ��豸��ʼ��
void char_dev_init(void)
{
	char_dev_t *kb_dev = &kboard_dev;
	kb_dev->ops.init();
	add_char_dev(kb_dev);

	cprintk(rc_black, rc_magenta, "Init %s device ...\n\n", kb_dev->ops.get_desc());
}

// �ں�ע���ַ��豸
int add_char_dev(char_dev_t *cdev)
{
	char_dev_t *p = char_devs;
	while (p) {
		if (strcmp(p->name, cdev->name) == 0) {
			return -1;
		}
		p = p->next;
	}

	cdev->next = char_devs;
	char_devs = cdev;

	return 0;
}


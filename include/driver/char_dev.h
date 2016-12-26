#ifndef INCLUDE_DRIVER_CHAR_DEV_H_
#define INCLUDE_DRIVER_CHAR_DEV_H_

#include <types.h>

// �ַ��豸�ӿ�
typedef
struct char_dev {
	const char *name;                               // �豸����
	bool is_readable;                               // �豸�Ƿ�ɶ�
	bool is_writeable;                              // �豸�Ƿ��д
	struct char_ops {                               // �豸����
		int(*init)(void);                      // �豸��ʼ��
		bool(*device_valid)(void); 	        // �豸�Ƿ����
		const char *(*get_desc)(void);          // ��ȡ�豸����
		int(*read)(void *, uint32_t);          // �豸��ȡ
		int(*write)(const void *, uint32_t);   // �豸д��
		int(*ioctl)(int, int);                 // �豸ѡ������
	} ops;
	struct char_dev *next;                          // �ַ��豸��
} char_dev_t;

// ȫ���ַ��豸����
extern char_dev_t *char_devs;

// �ַ��豸��ʼ��
void char_dev_init(void);

// �ں�ע���ַ��豸
int add_char_dev(char_dev_t *cdev);

// Keyboard �豸�ṹ
extern char_dev_t kboard_dev;

#endif  // INCLUDE_DRIVER_CHAR_DEV_H_

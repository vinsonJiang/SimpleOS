
#ifndef INCLUDE_BLOCK_DEV_H_
#define INCLUDE_BLOCK_DEV_H_

#include <types.h>

typedef enum io_type {
	IO_READ = 0,
	IO_WRITE = 1
} io_type_t;

typedef struct io_request {
	io_type_t io_type;       // IO��������
	uint32_t secno;          // ��ʼλ��
	uint32_t nsecs;          // ��д����
	void *buffer;            // ��д������
	uint32_t bsize;          // �������ߴ�
} io_request_t;

// ���豸�ӿ�
typedef struct block_dev {
	const char *name;               // �豸����
	uint32_t block_size;            // ��λ���С
	struct block_ops {                              // �豸����
		int(*init)(void);                      // �豸��ʼ��
		bool(*device_valid)(void); 	        // �豸�Ƿ����
		const char *(*get_desc)(void);          // ��ȡ�豸����
		int(*get_nr_block)(void);              // ����豸Ĭ�Ͽ�����
		int(*request)(io_request_t *);         // �豸��������
		int(*ioctl)(int, int);                 // �豸ѡ������
	} ops;
	struct block_dev *next;                         // ���豸��
} block_dev_t;

// ȫ�ֿ��豸����
extern block_dev_t *block_devs;

// ���豸��ʼ��
void block_dev_init(void);

// �ں�ע����豸
int add_block_dev(block_dev_t *bdev);

// IDE �豸�ṹ
extern block_dev_t ide_main_dev;

#endif  // INCLUDE_BLOCK_DEV_H_

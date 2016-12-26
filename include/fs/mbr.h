
#ifndef INCLUDE_MBR_H_
#define INCLUDE_MBR_H_

#include <types.h>
#include <driver/block_dev.h>

#define SECTION_SIZE      512 	  // ������С
#define MBR_CODE_LENGTH   446     // MBR���볤��
#define PARTITION_SIZE    16      // ���������С
#define PARTITION_COUNT   4       // ��������Ŀ����

typedef
struct partition_info_t {
	uint8_t active_flag;       // ��������(0x0��ʾ�ǻ,0x80��ʾ�)
	uint8_t start_chs[3];      // ��ʼ��ͷ��+������+����š���ͷ��1�ֽ�;������2�ֽڵ�6λ;�����2�ֽڸ�2λ+3�ֽ�
	uint8_t partition_type;    // ��������
	uint8_t end_chs[3];        // ������ͷ��,����ͬ��ʼ��ͷ��
	uint32_t start_sector;     // �߼���ʼ������
	uint32_t nr_sectors;       // ��ռ��������
} __attribute__((packed)) partition_info_t;

typedef
struct mbr_info_t
{
	uint8_t  mbr_code[MBR_CODE_LENGTH];         // ����������
	partition_info_t part[PARTITION_COUNT];     // 4 ������������Ϣ
	uint16_t magic_num;                         // ħ�� 0xAA55
} __attribute__((packed)) mbr_info_t;

// MBR��Ϣ
extern mbr_info_t mbr_info;

// ��ȡ������Ϣ
int read_mbr_info(block_dev_t *bdev);

// ���������Ϣ
void show_partition_info(void);

#endif  // INCLUDE_MBR_H_

#ifndef FS_FS_SFS_H
#define FS_FS_SFS_H

#include <types.h>

struct sfs_super_block {
	uint16_t s_type;                // �ļ�ϵͳ����
	uint32_t s_inode_count;         // inode����
	uint32_t s_block_count;         // block����
	uint32_t s_block_size;          // block��С
	uint32_t s_max_file;            // �ļ����ߴ�
	uint32_t s_root_inode;          // ��Ŀ¼ inode ��

	uint32_t in_secs;               // inode ��ռ������
	uint32_t bl_secs;               // block ��ռ������
	uint32_t im_start;              // inode map ��ʼ
	uint32_t im_secs;               // inode map ��ռ������
	uint32_t bm_start;              // block map ��ʼ��ַ
	uint32_t bm_secs;               // block map ��ռ������
	uint32_t in_start;              // inode ��ʼλ��
	uint32_t bl_start;              // block ��ʼλ��
};

#define P_BLOCK_MAP_SIZE     32            // һ�� block ����
#define PP_BLOCK_MAP_SIZE    32            // ���� block ����

struct sfs_inode {
	uint32_t i_type;                // inode ����
	uint32_t i_ino;                 // �����ڵ��
	time_t i_atime;                 // �ļ����һ�η���ʱ��
	time_t i_mtime;                 // �ļ����һ���޸�ʱ��
	time_t i_ctime;                 // �ļ��״δ���ʱ��
	uint32_t i_size;                // �ļ��ֽ���
	uint32_t i_blocks;              // �ļ�ʹ��block��
	uint32_t i_bytes;               // �ļ����һ��block���ֽ���

	uint32_t p_block_map[P_BLOCK_MAP_SIZE];          // һ�� block ָ��
	uint32_t pp_block_map[PP_BLOCK_MAP_SIZE];        // ���� block ָ��
};

// ��ļ���
#define SFS_MAX_FILE_NAME   123

// Ŀ¼����inode�洢��Ŀ¼���ݿ� sizeof = 128
struct sfs_dirent {
	char d_name[SFS_MAX_FILE_NAME + 1];   // Ŀ¼������
	uint32_t inode;                     // ��Ӧ�� inode ��
};

void sfs_init(void);

#endif  // FS_FS_SFS_H

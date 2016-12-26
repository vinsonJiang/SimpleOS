#ifndef INCLUDE_FS_FS_H_
#define INCLUDE_FS_FS_H_

#include <types.h>
#include <spinlock.h>
#include <driver/block_dev.h>

#define RAMFS_T        0xA0
#define SFS_T          0xAA

// ǰ������
struct super_block;
struct super_ops;
struct inode;
struct inode_ops;
struct dentry;
struct dentry_ops;
struct file;
struct file_ops;

struct filesystem {
	const char *name;                                         // �ļ�ϵͳ����
	uint8_t type;                                             // �ļ�ϵͳ����
	struct super_block *(*read_super)(struct super_block *);  // ��ȡsuper_block
	struct list_head fs_supers;                               // super_blockָ��
	struct filesystem *next;                                  // ��һ���ļ�ϵͳ
};

struct super_block {
	uint16_t s_type;                // �ļ�ϵͳ����
	uint32_t s_inode_count;         // inode����
	uint32_t s_block_count;         // block����
	uint32_t s_block_size;          // block��С
	uint32_t s_max_file;            // �ļ����ߴ�
	uint32_t s_root_inode;          // �� inode

	void *impl_sb;                  // ָ������ļ�ϵͳ�� super_block
	struct list_head s_list;        // super_blockָ��
	block_dev_t *bdev;              // ��Ӧ�Ŀ��豸ָ��
	struct dentry *s_root;          // ��dentry
	struct super_block_ops *s_ops;  // super_block����
};

struct super_block_ops {
	struct inode *(*alloc_inode)(struct super_block *);     // ��ȡinode
	void(*destroy_inode)(struct inode *);                  // �ͷ�inode
	void(*write_super)(struct super_block *);              // д��super_block
	int(*sync_fs)(struct super_block *);                   // ͬ���ļ�ϵͳ
};

#define S_DIR    0x1    // inode Ŀ¼����
#define S_FILE   0x2    // inode �ļ�����

struct inode {
	uint32_t i_type;                // inode ����
	uint32_t i_ino;                 // �����ڵ��
	time_t i_atime;                 // �ļ����һ�η���ʱ��
	time_t i_mtime;                 // �ļ����һ���޸�ʱ��
	time_t i_ctime;                 // �ļ��״δ���ʱ��
	uint32_t i_size;                // �ļ��ֽ���
	uint32_t i_blocks;              // �ļ�ʹ��block��
	uint32_t i_bytes;               // �ļ����һ��block���ֽ���

	void *impl_in;                  // ָ������ļ�ϵͳ�� inode
	spinlock_t i_lock;              // inode������
	atomic_t i_count;               // �����ڵ����ü���
	struct super_block *i_sb;       // super_blcokָ��
	struct list_head i_list;        // inode ��
};

// inode��ز���
struct inode_ops {
	int(*create)(struct inode *, struct dentry *);         // �����ļ�
	int(*rm)(struct inode *, struct dentry *);             // ɾ���ļ�
	int(*mkdir)(struct inode *, struct dentry *);          // ����Ŀ¼
	int(*rmdir)(struct inode *, struct dentry *);          // ɾ��Ŀ¼
	int(*rename)(struct inode *, struct dentry *,          // ������
	struct inode *, struct dentry *);
};

// ��ļ���
#define MAX_FILE_NAME   123

// Ŀ¼����inode�洢��Ŀ¼���ݿ� sizeof = 128
struct dirent {
	char d_name[MAX_FILE_NAME + 1];   // Ŀ¼������
	uint32_t inode;                 // ��Ӧ�� inode
};

struct dentry {
	atomic_t d_count;               // ���ü���
	spinlock_t d_lock;              // dentry���������
	char d_name[MAX_FILE_NAME + 1];   // Ŀ¼������
	uint32_t d_status;              // Ŀ¼��״̬
	struct dentry *d_parent;        // ��Ŀ¼ָ��
	struct list_head d_subdirs;     // ��Ŀ¼����ͷ
	struct list_head d_child;       // ���ӵ���dentry��d_subdirs
	bool is_mounted;                // �Ƿ񱻹����豸
	struct super_block *d_sb;       // Ŀ¼���Ӧ��super_blcok
	struct inode *d_inode;          // ���ӵ�Ŀ¼���Ӧ��inode
	struct dentry_ops *d_ops;       // dentry��ز���
};

struct dentry_ops {
	int(*del)(struct dentry *);
};

// �·������(���ļ���)
#define MAX_PATH    1024

struct file {
	atomic_t f_count;               // ���ü���
	spinlock_t f_lock;              // file���������
	char f_path[MAX_PATH + 1];        // �ļ�·��
	struct dentry *f_dentry;        // ���ļ���ص�dentry
	uint32_t f_openflag;            // ���ļ�ʱ�ı��
	uint32_t f_pos;                 // �ļ�������ƫ��
	struct file_ops *f_ops;         // �ļ�����
};

struct file_ops {
	int(*read)(struct file *, char *, uint32_t);
	int(*write)(struct file *, const char *, uint32_t);
	int(*open)(struct inode *, struct file *);
	int(*fsync)(struct file *);
	int(*close)(struct inode *, struct file *);
};

// �������򿪵��ļ�����
#define MAX_OPEN_FILE   64

// ����PCB�������ļ�ϵͳ�Ľṹ
struct file_struct {
	spinlock_t fs_lock;                     // ͬ���޸ı�����
	struct vfsmount *vfsmount;              // �ļ�ϵͳ���ṹ
	struct file *file_array[MAX_OPEN_FILE]; // ���̴򿪵��ļ�����
};

struct vfsmount {
	const char *mnt_devname;         // ���ص��豸����
	struct super_block *mnt_sb;      // ���ص� super_block
	struct dentry *mnt_root;         // ���صĸ�Ŀ¼ dentry
	struct dentry *mnt_mountpoint;   // ���ص� dentry
	struct vfsmount *mnt_parent;     // ��vfsmountָ��
};

// ȫ�ֵ��ļ�ϵͳָ��
extern struct filesystem *file_systems;

// ���ļ�ϵͳ
extern struct filesystem fs_ramfs;

// �ļ�ϵͳ��ʼ��
void fs_init(void);

// �ں�ע���ļ�ϵͳ
int add_filesystem(struct filesystem *fs);

// ��ȡ super_block �ṹ
struct super_block *alloc_super_block(void);

// ��ȡ super_pos �ṹ
struct super_block_ops *alloc_super_block_ops(void);

// ��ȡ inode �ṹ
struct inode *alloc_inode(void);

// ��ȡ inode_ops �ṹ
struct inode_ops *alloc_inode_ops(void);

// ��ȡ dentry �ṹ
struct dentry *alloc_dentry(void);

// ��ȡ dentry_ops �ṹ
struct dentry_ops *alloc_dentry_ops(void);

// ��ȡ file �ṹ
struct file *alloc_file(void);

// ��ȡ file_ops �ṹ
struct file_ops *alloc_file_ops(void);

// �ͷ� super_block �ṹ
void free_super_block(struct super_block *sb);

// �ͷ� super_block_ops �ṹ
void free_super_block_ops(struct super_ops *sb_ops);

// �ͷ� inode �ṹ
void free_inode(struct inode *inode);

// �ͷ� inode_ops �ṹ
void free_inode_ops(struct inode_ops *inode_ops);

// �ͷ� dentry �ṹ
void free_dentry(struct dentry *dentry);

// �ͷ� dentry_ops �ṹ
void free_dentry_ops(struct dentry_ops *dentry_ops);

// �ͷ� file �ṹ
void free_file(struct file *file);

// �ͷ� file_ops �ṹ
void free_file_ops(struct file_ops *file_ops);

// ���ļ�
int vfs_open(const char *filename, uint32_t openflag);

// �ر��ļ�
int vfs_close(int fd);

// ��ȡ�ļ�
int vfs_read(int fd, char *buff, size_t size);

// д���ļ�
int vfs_write(int fd, const char *buff, size_t size);

#endif  // INCLUDE_FS_FS_H_

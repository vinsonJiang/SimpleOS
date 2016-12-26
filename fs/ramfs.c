/*
	���ļ�ϵͳ
*/

#include <fs/fs.h>

// ��ȡsuper_block
static struct super_block *ramfs_read_super(struct super_block *sb);

// ramfs ����
struct filesystem fs_ramfs = {
	.name = "ramfs",
	.type = RAMFS_T,
	.read_super = ramfs_read_super
};

// ���� inode
static struct inode *ramfs_alloc_inode(struct super_block *sb);

// �ͷ�inode
static void ramfs_destroy_inode(struct inode *inode);

// �豸д�س�����
static void ramfs_write_super(struct super_block *sb);

// ͬ���ļ�ϵͳ�޸�
static int ramfs_sync_fs(struct super_block *sb);

// super_block_ops ����
static struct super_block_ops ramfs_sb_ops = {
	.alloc_inode = ramfs_alloc_inode,
	.destroy_inode = ramfs_destroy_inode,
	.write_super = ramfs_write_super,
	.sync_fs = ramfs_sync_fs,
};

// ��ȡsuper_block
static struct super_block *ramfs_read_super(struct super_block *sb)
{
	sb->s_type = RAMFS_T;
	sb->s_ops = &ramfs_sb_ops;

	return sb;
}

// ���� inode
static struct inode *ramfs_alloc_inode(__UNUSED__ struct super_block *sb)
{
	return alloc_inode();
}

// �ͷ�inode
static void ramfs_destroy_inode(struct inode *inode)
{
	free_inode(inode);
}

// �豸д�س�����
static void ramfs_write_super(__UNUSED__ struct super_block *sb)
{
	// do nothing ...
}

// ͬ���ļ�ϵͳ�޸�
static int ramfs_sync_fs(__UNUSED__ struct super_block *sb)
{
	// do nothing ...
	return 0;
}


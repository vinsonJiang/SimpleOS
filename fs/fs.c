#include <fs/sfs.h>
#include <fs/vfs.h>

// �ļ�ϵͳ��ʼ��
void fs_init(void)
{
	vfs_init();
	sfs_init();
}

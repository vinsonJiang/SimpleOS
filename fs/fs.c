#include <fs/sfs.h>
#include <fs/vfs.h>

// 文件系统初始化
void fs_init(void)
{
	vfs_init();
	sfs_init();
}

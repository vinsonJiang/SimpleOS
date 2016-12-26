#include <types.h>
#include <common.h>
#include <debug.h>

#include <driver/block_dev.h>

#define SECTSIZE  512 // Ĭ��������С

/**
* bit 7 = 1  ������æ         * bit 6 = 1  ����������
* bit 5 = 1  �豸����         * bit 4      N/A
* bit 3 = 1  ��������������   * bit 2 = 1  �����ѱ���У��
* bit 1        N/A            * bit 0 = 1  ��һ������ִ��ʧ��
*/
#define IDE_BSY                 0x80            // IDE������æ 
#define IDE_DRDY                0x40            // IDE����������
#define IDE_DF                  0x20            // IDE����������
#define IDE_ERR                 0x01            // ��һ������ʧ��

#define IDE_CMD_READ            0x20            // IDE����������
#define IDE_CMD_WRITE           0x30            // IDEд��������
#define IDE_CMD_IDENTIFY        0xEC            // IDEʶ������

// IDE�豸�˿���ʼ�˿ڶ���
#define IOBASE                  0x1F0             // ��IDE�豸��ʼ�����˿�
#define IOCTRL                  0x3F4             // ��IDE������ʼ���ƶ˿�

// IDE�豸���ƶ˿�ƫ����
#define ISA_DATA                0x00            // IDE���ݶ˿�ƫ��
#define ISA_ERROR               0x01            // IDE����˿�ƫ��
#define ISA_PRECOMP             0x01            
#define ISA_CTRL                0x02            // IDE���ƶ˿�ƫ��
#define ISA_SECCNT              0x02
#define ISA_SECTOR              0x03
#define ISA_CYL_LO              0x04
#define ISA_CYL_HI              0x05
#define ISA_SDH                 0x06            // IDEѡ��˿�ƫ��
#define ISA_COMMAND             0x07            // IDE����˿�ƫ��
#define ISA_STATUS              0x07            // IDE״̬�˿�ƫ��

// IDE�豸����ֵ
#define MAX_NSECS               128             // IDE�豸������������
#define MAX_DISK_NSECS          0x10000000      // IDE�豸���������

// IDE�豸�����Ϣ�ڶ�ȡ����Ϣ���е�ƫ��
#define IDE_IDENT_SECTORS       20
#define IDE_IDENT_MODEL         54
#define IDE_IDENT_CAPABILITIES  98
#define IDE_IDENT_CMDSETS       164
#define IDE_IDENT_MAX_LBA       120
#define IDE_IDENT_MAX_LBA_EXT   200

#define IDE_DESC_LEN            40              // IDE�豸������Ϣ�ߴ�

// IDE�豸��Ϣ
static struct ide_device {
	uint8_t valid;                   // �Ƿ����
	uint32_t sets;                   // ����֧��
	uint32_t size;                   // ��������
	char desc[IDE_DESC_LEN + 1];       // IDE�豸����
} ide_device;

// ��ʼ��IDE�豸
static int ide_init(void);

// ���IDE�豸�Ƿ����
static bool ide_device_valid(void);

// ��ȡIDE�豸����
static const char *ide_get_desc(void);

// ����豸Ĭ�Ͽ�����
static int ide_get_nr_block(void);

// �豸��������
static int ide_request(io_request_t *req);

// ��ȡIDE�豸��������
static int ide_read_secs(uint32_t secno, void *dst, uint32_t nsecs);

// д��IDE�豸��������
static int ide_write_secs(uint32_t secno, const void *src, uint32_t nsecs);

// IDE�豸ѡ������
static int ide_ioctl(int op, int flag);

// IDE �豸�ṹ
block_dev_t ide_main_dev = {
	.name = "IDE_MAIN",
	.block_size = SECTSIZE,
	.ops = {
		.init = &ide_init,
		.device_valid = &ide_device_valid,
		.get_desc = &ide_get_desc,
		.get_nr_block = &ide_get_nr_block,
		.request = &ide_request,
		.ioctl = ide_ioctl
	}
};

// �ȴ�IDE�豸����
static int32_t ide_wait_ready(uint16_t iobase, bool check_error)
{
	int r = 0;
	while ((r = inb(iobase + ISA_STATUS)) & IDE_BSY) {
		// Waiting ... Do nothing ...
	}
	if (check_error && (r & (IDE_DF | IDE_ERR)) != 0) {
		return -1;
	}

	return 0;
}

// ��ȡIDE�豸����
static const char *ide_get_desc(void)
{
	return (const char *)(ide_device.desc);
}

// ��ʼ��IDE�豸
static int ide_init(void)
{
	ide_wait_ready(IOBASE, 0);

	// 1: ѡ��Ҫ�������豸
	outb(IOBASE + ISA_SDH, 0xE0);
	ide_wait_ready(IOBASE, 0);

	// 2: ����IDE��Ϣ��ȡ����
	outb(IOBASE + ISA_COMMAND, IDE_CMD_IDENTIFY);
	ide_wait_ready(IOBASE, 0);

	// 3: ����豸�Ƿ����
	if (inb(IOBASE + ISA_STATUS) == 0 || ide_wait_ready(IOBASE, 1) != 0) {
		return -1;
	}

	ide_device.valid = 1;

	// ��ȡIDE�豸��Ϣ
	uint32_t buffer[128];
	insl(IOBASE + ISA_DATA, buffer, sizeof(buffer) / sizeof(uint32_t));

	uint8_t *ident = (uint8_t *)buffer;
	uint32_t cmdsets = *(uint32_t *)(ident + IDE_IDENT_CMDSETS);
	uint32_t sectors;

	// ����豸ʹ��48-bits����28-bits��ַ
	if (cmdsets & (1 << 26)) {
		sectors = *(uint32_t *)(ident + IDE_IDENT_MAX_LBA_EXT);
	}
	else {
		sectors = *(uint32_t *)(ident + IDE_IDENT_MAX_LBA);
	}
	ide_device.sets = cmdsets;
	ide_device.size = sectors;

	char *desc = ide_device.desc;
	char *data = (char *)((uint32_t)ident + IDE_IDENT_MODEL);

	// �����豸������Ϣ
	int i, length = IDE_DESC_LEN;
	for (i = 0; i < length; i += 2) {
		desc[i] = data[i + 1];
		desc[i + 1] = data[i];
	}
	do {
		desc[i] = '\0';
	} while (i-- > 0 && desc[i] == ' ');

	return 0;
}

// ���ָ��IDE�豸�Ƿ����
static bool ide_device_valid(void)
{
	return ide_device.valid == 1;
}

// ����豸Ĭ�Ͽ�����
static int ide_get_nr_block(void)
{
	if (ide_device_valid()) {
		return ide_device.size;
	}

	return 0;
}

// �豸��������
static int ide_request(io_request_t *req)
{
	if (req->io_type == IO_READ) {
		if (req->bsize < SECTSIZE * req->nsecs) {
			return -1;
		}
		return ide_read_secs(req->secno, req->buffer, req->nsecs);
	}
	else if (req->io_type == IO_WRITE) {
		if (req->bsize < SECTSIZE * req->nsecs) {
			return -1;
		}
		return ide_write_secs(req->secno, req->buffer, req->nsecs);
	}

	return -1;
}

// ��ȡָ��IDE�豸��������
static int ide_read_secs(uint32_t secno, void *dst, uint32_t nsecs)
{
	assert(nsecs <= MAX_NSECS && ide_device.valid == 1, "nsecs or ide error!");
	assert(secno < MAX_DISK_NSECS && secno + nsecs <= MAX_DISK_NSECS, "secno error!");

	ide_wait_ready(IOBASE, 0);

	outb(IOCTRL + ISA_CTRL, 0);
	outb(IOBASE + ISA_SECCNT, nsecs);
	outb(IOBASE + ISA_SECTOR, secno & 0xFF);
	outb(IOBASE + ISA_CYL_LO, (secno >> 8) & 0xFF);
	outb(IOBASE + ISA_CYL_HI, (secno >> 16) & 0xFF);
	outb(IOBASE + ISA_SDH, 0xE0 | ((secno >> 24) & 0xF));
	outb(IOBASE + ISA_COMMAND, IDE_CMD_READ);

	int ret = 0;
	for (; nsecs > 0; nsecs--, dst += SECTSIZE) {
		if ((ret = ide_wait_ready(IOBASE, 1)) != 0) {
			return ret;
		}
		insl(IOBASE, dst, SECTSIZE / sizeof(uint32_t));
	}

	return ret;
}

// д��ָ��IDE�豸��������
static int ide_write_secs(uint32_t secno, const void *src, uint32_t nsecs)
{
	assert(nsecs <= MAX_NSECS && ide_device.valid == 1, "nsecs or ide error");
	assert(secno < MAX_DISK_NSECS && secno + nsecs <= MAX_DISK_NSECS, "secno error!");

	ide_wait_ready(IOBASE, 0);

	outb(IOCTRL + ISA_CTRL, 0);
	outb(IOBASE + ISA_SECCNT, nsecs);
	outb(IOBASE + ISA_SECTOR, secno & 0xFF);
	outb(IOBASE + ISA_CYL_LO, (secno >> 8) & 0xFF);
	outb(IOBASE + ISA_CYL_HI, (secno >> 16) & 0xFF);
	outb(IOBASE + ISA_SDH, 0xE0 | ((secno >> 24) & 0xF));
	outb(IOBASE + ISA_COMMAND, IDE_CMD_WRITE);

	int ret = 0;
	for (; nsecs > 0; nsecs--, src += SECTSIZE) {
		if ((ret = ide_wait_ready(IOBASE, 1)) != 0) {
			return ret;
		}
		outsl(IOBASE, src, SECTSIZE / sizeof(uint32_t));
	}

	return ret;
}

// IDE�豸ѡ������
static int ide_ioctl(int op, int flag)
{
	if (op != 0 && flag != 0) {
		return -1;
	}

	return 0;
}


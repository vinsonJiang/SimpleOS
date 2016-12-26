#include <common.h>
#include <intr/intr.h>
#include <driver/char_dev.h>

/*
* ���̵�ǰ״̬��Ϣ(���λ�����ü�Ϊ��Ч)
*
* 0: control
* 1: alt
* 2: alt-gr
* 3: left shift
* 4: right shift
* 5: caps_lock
* 6: scroll_lock
* 7: num_lock
*/
#define CONTROL         0x1
#define ALT             0x2
#define ALTGR           0x4
#define LSHIFT          0x8
#define RSHIFT          0x10
#define CAPSLOCK        0x20
#define SCROLLLOCK      0x40
#define NUMLOCK         0x80

// 8 λ�ļ���ɨ����Ľ�ͨ��ʹ��ǰ7λ
// �����λ�� 1 �������Ӧ�ĶϿ���
// �ú���Ժͻ�ȡ��ɨ���������ж�һ�����ǰ��»���̧��
#define RELEASED_MASK 0x80

typedef uint8_t key_status_t;

typedef
struct keymap {
	uint8_t scancodes[128];         // ����ɨ�����ӳ��
	uint8_t capslock_scancodes[128];
	uint8_t shift_scancodes[128];
	uint8_t control_map[8];
	key_status_t controls;          // ���̵Ŀ���״̬��Ϣ
} keymap_t;

static keymap_t us_keymap = {
	//normal keys
	{
		/* first row - indices 0 to 14 */
		0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
		/* second row - indices 15 to 28 */
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', //Enter key
																			/* 29 = Control, 30 - 41: third row */
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	/* fourth row, indices 42 to 54, zeroes are shift-keys*/
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*',
	/* Special keys */
	0,    // ALT - 56
	' ', // Space - 57
	0,    // Caps lock - 58
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
	0, // Num lock - 69
	0, // Scroll lock - 70
	0, // Home - 71
	72, // Up arrow - 72  TODO
	0, // Page up - 73
	'-',
	0, // Left arrow - 75
	0,
	0, // Right arrow -77
	'+',
	0, // End - 79
	80, // Dowm arrow - 80  TODO
	0, // Page down - 81
	0, // Insert - 82
	0, // Delete - 83
	0, 0, 0,
	0, // F11 - 87
	0, // F12 - 88
	0, // All others undefined
	},
	// caps
	{
		/* first row - indices 0 to 14 */
		0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
		/* second row - indices 15 to 28 */
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	/* 29 = Control, 30 - 41: third row */
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
	/* fourth row, indices 42 to 54, zeroes are shift-keys*/
	0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*',
	/* Special keys */
	0,   // ALT - 56
	' ', // Space - 57
	0,   // Caps lock - 58
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
	0, // Num lock - 69
	0, // Scroll lock - 70
	0, // Home - 71
	0, // Up arrow - 72
	0, // Page up - 73
	'-',
	0, // Left arrow - 75
	0,
	0, // Right arrow -77
	'+',
	0, // End - 79
	0, // Dowm arrow - 80
	0, // Page down - 81
	0, // Insert - 82
	0, // Delete - 83
	0, 0, 0,
	0, // F11 - 87
	0, // F12 - 88
	0, // All others undefined
	},
	// shift
	{
		/* first row - indices 0 to 14 */
		0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
		/* second row - indices 15 to 28 */
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	/* 29 = Control, 30 - 41: third row */
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	/* fourth row, indices 42 to 54, zeroes are shift-keys*/
	0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
	/* Special keys */
	0,   // ALT - 56
	' ', // Space - 57
	0,   // Caps lock - 58
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
	0, // Num lock - 69
	0, // Scroll lock - 70
	0, // Home - 71
	0, // Up arrow - 72
	0, // Page up - 73
	'-',
	0, // Left arrow - 75
	0,
	0, // Right arrow -77
	'+',
	0, // End - 79
	0, // Dowm arrow - 80
	0, // Page down - 81
	0, // Insert - 82
	0, // Delete - 83
	0, 0, 0,
	0, // F11 - 87
	0, // F12 - 88
	0, // All others undefined
	},
	// control_map
	{
		29, // Ctrl
		56, // Alt
	0,  // AltGr
	42, // left Shift
	54, // right Shift
	58, // Caps lock
	70, // Scroll lock
	69  // Num lock
	},
	// ���̵Ŀ��Ƽ���Ϣ��ʼ��Ϊ 0 
	0
};

// �豸��ʼ��
static int kb_init(void);

// �豸�Ƿ����
static bool kb_device_valid(void);

// ��ȡ�豸����
static const char *kb_get_desc(void);

// �豸��ȡ
static int kb_read(void *dec, uint32_t len);

// �豸д��
static int kb_write(const void *src, uint32_t len);

// �豸����
static int kb_ioctl(int op, int flag);

// �����жϴ�����
static void keyboard_handler(__UNUSED__ pt_regs_t *regs);

// Keyboard �豸�ṹ
char_dev_t kboard_dev = {
	.name = "Keyboard",
	.is_readable = true,
	.is_writeable = false,
	.ops = {
	.init = kb_init,
	.device_valid = kb_device_valid,
	.get_desc = kb_get_desc,
	.read = kb_read,
	.write = kb_write,
	.ioctl = kb_ioctl
}
};

#define KBUFFER_LEN     1024

// Keyboard ������Ϣ
static struct kboard_device {
	bool is_valid;                   // �豸�Ƿ����
	keymap_t *curr_layout;           // ��ǰ���ַ���
	uint8_t kbuffer[KBUFFER_LEN];    // ��������Ļ���������
	uint32_t kfront;                 // ������ж�ͷ
	uint32_t krear;                  // ������ж�β
} kb_device;

// �豸��ʼ��
static int kb_init(void)
{
	// ����US�ַ���
	kb_device.curr_layout = &us_keymap;

	// �����豸����
	kb_device.is_valid = true;

	// ע������жϴ�����
	register_interrupt_handler(IRQ1, &keyboard_handler);

	return 0;
}

// �豸�Ƿ����
static bool kb_device_valid(void)
{
	return kb_device.is_valid;
}

// ��ȡ�豸����
static const char *kb_get_desc(void)
{
	return "Keyboard";
}

static uint8_t kb_getchar(void)
{
	// ��������������ȡ�������򷵻� 0
	if (kb_device.kfront != kb_device.krear) {
		char ch = kb_device.kbuffer[kb_device.kfront];
		kb_device.kfront = (kb_device.kfront + 1) % KBUFFER_LEN;
		return ch;
	}
	else {
		return 0;
	}
}

// �豸��ȡ
static int kb_read(void *dec, uint32_t len)
{
	uint8_t *buffer = (uint8_t *)dec;
	uint8_t ch = 0;
	uint32_t i = 0;
	while (i < len) {
		if ((ch = kb_getchar()) != 0) {
			buffer[i] = ch;
			i++;
		}
		else {
			break;
		}
	}

	return i;
}

// �豸д��
static int kb_write(__UNUSED__ const void *src, __UNUSED__ uint32_t len)
{
	return -1;
}

// �����жϴ�����
static void keyboard_handler(__UNUSED__ pt_regs_t *regs)
{
	// �Ӽ��̶˿ڶ��밴�µļ�
	uint8_t scancode = inb(0x60);

	keymap_t *layout = kb_device.curr_layout;

	// �����ж��ǰ��»���̧��
	if (scancode & RELEASED_MASK) {
		uint32_t i;
		// ����ֻ���ǰ 5 �����Ƽ�����Ϊǰ��λ Ctrl Alt Shift �ɿ��󲻱���״̬
		// ������Щ�����ɿ������������±��
		for (i = 0; i < 5; i++) {
			if (layout->control_map[i] == (scancode & ~RELEASED_MASK)) {
				layout->controls &= ~(1 << i);
				return;
			}
		}
		// ����������
	}
	else {
		uint32_t i;
		// ��һ����������λ
		for (i = 0; i < 8; i++) {
			// �����ǰ���ǿ��Ƽ��������ؿ���λ�� 1
			// ������иñ�־λ�������ؿ���λ�� 0
			if (layout->control_map[i] == scancode) {
				if (layout->controls & 1 << i) {
					layout->controls &= ~(1 << i);
				}
				else {
					layout->controls |= (1 << i);
				}
				return;
			}
		}
		uint8_t *scancodes = layout->scancodes;

		// �����ʱ������ shift �����л��� shift ɨ����
		if ((layout->controls & (LSHIFT | RSHIFT)) && !(layout->controls & CONTROL)) {
			scancodes = layout->shift_scancodes;
		}
		// �����ʱ���ڴ�д����״̬���л�����д������ɨ����
		if ((layout->controls & (CAPSLOCK)) && !(layout->controls & CONTROL)) {
			scancodes = layout->capslock_scancodes;
		}
		// ������в������ַ���ӣ�������
		if (kb_device.kfront != (kb_device.krear + 1) % KBUFFER_LEN) {
			kb_device.kbuffer[kb_device.krear] = scancodes[scancode];
			kb_device.krear = (kb_device.krear + 1) % KBUFFER_LEN;
		}
	}
}


// �豸����
static int kb_ioctl(int op, int flag)
{
	if (op != 0 && flag != 0) {
		return -1;
	}

	return 0;
}

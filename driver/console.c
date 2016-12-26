#include <common.h>
#include <debug.h>
//#include <sync.h>
#include <mm/mm.h>

#include <driver/console.h>

/*
* VGA(Video Graphics Array����Ƶͼ������)��ʹ��ģ���źŵ�һ����Ƶ�����׼���ں˿���ͨ������������Ļ���ַ�����ͼ�ε���ʾ��
* ��Ĭ�ϵ��ı�ģʽ(Text-Mode)�£�VGA������������һ���ڴ�(0x8b000~0x8bfa0)��Ϊ��Ļ���ַ���ʾ�Ļ�������
* ��Ҫ�ı���Ļ���ַ�����ʾ��ֻ��Ҫ�޸�����ڴ�ͺ��ˡ�
*
*/

// VGA ����ʾ���������� 0xB8000
static uint16_t *video_memory = (uint16_t *)(0xB8000 + PAGE_OFFSET);

// ��Ļ"���"������
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
// ��Ļ��ʾ��ʼ��
void console_init(void)
{
	console_clear();
	cprintk(rc_black, rc_green, "Hello, Vinson!\n\n\n");
}

// �ƶ����
static void move_cursor()
{
	// ��Ļ�� 80 �ֽڿ�
	uint16_t cursorLocation = cursor_y * 80 + cursor_x;

	// VGA �ڲ��ļĴ������300�������Ȼ�޷�һһӳ�䵽I/O�˿ڵĵ�ַ�ռ䡣
	// �Դ� VGA �������Ľ�������ǣ���һ���˿���Ϊ�ڲ��Ĵ�����������0x3D4��
	// ��ͨ�� 0x3D5 �˿���������Ӧ�Ĵ�����ֵ��
	// �������õ��������ڲ��Ĵ����ı��Ϊ14��15���ֱ��ʾ���λ�õĸ�8λ���8λ��

	outb(0x3D4, 14);                  	// ���� VGA ����Ҫ���ù��ĸ��ֽ�
	outb(0x3D5, cursorLocation >> 8); 	// ���͸� 8 λ
	outb(0x3D4, 15);                  	// ���� VGA ����Ҫ���ù��ĵ��ֽ�
	outb(0x3D5, cursorLocation);     	// ���͵� 8 λ
}

// ��Ļ��������
static void scroll()
{
	// attribute_byte �������һ���ڵװ��ֵ�������ʽ
	uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
	uint16_t blank = 0x20 | (attribute_byte << 8);  // space �� 0x20

													// cursor_y �� 25 ��ʱ�򣬾͸û�����
	if (cursor_y >= 25) {
		// �������е���ʾ���ݸ��Ƶ���һ�У���һ����Զ��ʧ��...
		int i;
		for (i = 0 * 80; i < 24 * 80; i++) {
			video_memory[i] = video_memory[i + 80];
		}

		// ����һ�������������ո񣬲���ʾ�κ��ַ�
		for (i = 24 * 80; i < 25 * 80; i++) {
			video_memory[i] = blank;
		}

		// �����ƶ���һ�У����� cursor_y ������ 24
		cursor_y = 24;
	}
}

// ��������
void console_clear()
{
	uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
	uint16_t blank = 0x20 | (attribute_byte << 8);

	int i;
	for (i = 0; i < 80 * 25; i++) {
		video_memory[i] = blank;
	}

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

// ��Ļ���һ���ַ�(����ɫ)
void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	uint8_t back_color = (uint8_t)back;
	uint8_t fore_color = (uint8_t)fore;

	uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
	uint16_t attribute = attribute_byte << 8;

	// 0x08 �� �˸�� �� ASCII ��
	// 0x09 �� tab �� �� ASCII ��
	if (c == 0x08 && cursor_x) {
		cursor_x--;
	}
	else if (c == 0x09) {
		cursor_x = (cursor_x + 8) & ~(8 - 1);
	}
	else if (c == '\r') {
		cursor_x = 0;
	}
	else if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
	}
	else if (c >= ' ') {
		video_memory[cursor_y * 80 + cursor_x] = c | attribute;
		cursor_x++;
	}

	// ÿ 80 ���ַ�һ�У���80�ͱ��뻻����
	if (cursor_x >= 80) {
		cursor_x = 0;
		cursor_y++;
	}

	// �����Ҫ�Ļ�������Ļ��ʾ
	scroll();

	// �ƶ�Ӳ����������
	move_cursor();
}

// ��Ļ��ӡһ���� \0 ��β���ַ���(Ĭ�Ϻڵװ���)
void console_write(char *cstr)
{
	while (*cstr) {
		console_putc_color(*cstr++, rc_black, rc_white);
	}
}

// ��Ļ��ӡһ���� \0 ��β���ַ���(����ɫ)
void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
	while (*cstr) {
		console_putc_color(*cstr++, back, fore);
	}
}

// ��Ļ���һ��ʮ�����Ƶ�������
void console_write_hex(uint32_t n, real_color_t back, real_color_t fore)
{
	int tmp;
	char noZeroes = 1;

	console_write_color("0x", back, fore);

	int i;
	for (i = 28; i >= 0; i -= 4) {
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0) {
			continue;
		}
		noZeroes = 0;
		if (tmp >= 0xA) {
			console_putc_color(tmp - 0xA + 'a', back, fore);
		}
		else {
			console_putc_color(tmp + '0', back, fore);
		}
	}
}

// ��Ļ���һ��ʮ���Ƶ�������
void console_write_dec(uint32_t n, real_color_t back, real_color_t fore)
{
	if (n == 0) {
		console_putc_color('0', back, fore);
		return;
	}

	uint32_t acc = n;
	char c[32];
	int i = 0;
	while (acc > 0) {
		c[i] = '0' + acc % 10;
		acc /= 10;
		i++;
	}
	c[i] = 0;

	char c2[32];
	c2[i--] = 0;

	int j = 0;
	while (i >= 0) {
		c2[i--] = c[j++];
	}

	console_write_color(c2, back, fore);
}


//�����
void putchar(char ch, real_color_t back, real_color_t fore)
{
	console_putc_color(ch, back, fore);
}
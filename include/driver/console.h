#ifndef INCLUDE_DRIVER_CONSOLE_H_
#define INCLUDE_DRIVER_CONSOLE_H_

#include <types.h>

typedef
enum real_color {
	rc_black = 0,
	rc_blue = 1,
	rc_green = 2,
	rc_cyan = 3,
	rc_red = 4,
	rc_magenta = 5,
	rc_brown = 6,
	rc_light_grey = 7,
	rc_dark_grey = 8,
	rc_light_blue = 9,
	rc_light_green = 10,
	rc_light_cyan = 11,
	rc_light_red = 12,
	rc_light_magenta = 13,
	rc_light_brown = 14,
	rc_white = 15
} real_color_t;

// ��Ļ��ʾ��ʼ��
void console_init(void);

// ��������
void console_clear(void);

// ��Ļ��ӡһ���� \0 ��β���ַ���(Ĭ�Ϻڵװ���)
void console_write(char *cstr);

// ��Ļ��ӡһ���� \0 ��β���ַ���(����ɫ)
void console_write_color(char *cstr, real_color_t back, real_color_t fore);

//// ��Ļ��ʾ�����ƶ�n��
//void console_view_up(uint16_t offset);
//
//// ��Ļ��ʾ�����ƶ�n��
//void console_view_down(uint16_t offset);

void putchar(char ch, real_color_t back, real_color_t fore);

#endif  // INCLUDE_DRIVER_CONSOLE_H_

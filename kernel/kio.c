#include <common.h>
#include <driver/char_dev.h>
#include <kio.h>

// ¶ÁÈ¡Ò»¸ö×Ö·û
char getchar(void)
{
	char ch;
	char_dev_t *kb_dev = &kboard_dev;

	if (!kb_dev->ops.device_valid()) {
		return 0;
	}

	while (kb_dev->ops.read(&ch, 1) == 1) {
		cpu_hlt();
		return ch;
	}

	return 0;
}

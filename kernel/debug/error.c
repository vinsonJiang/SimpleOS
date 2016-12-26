#include <error.h>

// 内核错误的字符串定义
static const char * const error_string[MAXERROR + 1] = {
	[0]                     NULL,
	[E_UNSPECIFIED]         "unspecified error",
	[E_BAD_PROC]            "bad process",
	[E_INVAL]               "invalid parameter",
	[E_NO_MEM]              "out of memory",
	[E_NO_FREE_PROC]        "out of processes",
	[E_FAULT]               "segmentation fault",
};

// 获得错误号对应的字符串
const char *strerr(int errno)
{
	if (errno < MAXERROR) {
		return error_string[errno];
	}

	return (const char *)("unknown error");
}


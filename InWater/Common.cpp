#include "Lib.h"
#include <cstdio>

const TCHAR *putf(const TCHAR *format, ...)
{
	va_list	args;
	static TCHAR str[256];
	va_start(args, format);
	vsprintf_s(str, format, args);
	va_end(args);

	return str;
}

RECT Rect(int x, int y, int width, int height)
{
	RECT r = { x, y, width, height };
	return r;
}

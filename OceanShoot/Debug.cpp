/*
*	デバッグ用の関数
*
*	2015/05/17		Y.Ozawa
*/

#include "Lib.h"
#include <cstdio>

//	デバッグ用のアラートを出力する
void DebugAlert(TCHAR *format, ...)
{
	va_list args;
	TCHAR str[256];
	va_start(args, format);
	vsprintf_s(str, format, args);
	va_end(args);

	OutputDebugString(str);
	MessageBox(nullptr, str, "Error", MB_ICONSTOP);
}

//	デバッグ用のログを出力する
void DebugLog(TCHAR *format, ...)
{
	va_list args;
	TCHAR str[256];
	va_start(args, format);
	vsprintf_s(str, format, args);
	va_end(args);

	OutputDebugString(str);
	printf(str);
}




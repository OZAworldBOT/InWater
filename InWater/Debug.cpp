/*
*	�f�o�b�O�p�̊֐�
*
*	2015/05/17		Y.Ozawa
*/

#include "Lib.h"
#include <cstdio>

//	�f�o�b�O�p�̃A���[�g���o�͂���
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

//	�f�o�b�O�p�̃��O���o�͂���
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




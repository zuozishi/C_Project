#include "stdafx.h"
#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "Converter.h"

void TcharToChar(const TCHAR *tchar, char *_char)
{
	int length;
	//获取字节长度   
	length = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, length, NULL, NULL);
}

void CharToTchar(const char *_char, TCHAR *tchar)
{
	int length;
	length = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, length);
}

int TcharToInt(const TCHAR *tchar)
{
	int length;
	char str[50];
	//获取字节长度   
	length = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, str, length, NULL, NULL);
	int res = 0;
	for (int i = 0; i < strlen(str); i++)
	{
		res += (str[i] - '0')*pow(10.0, strlen(str) - 1 - i);
	}
	return res;
}

void NumToTCHAR(int num, TCHAR *tchar)
{
	char str[50];
	sprintf(str, "%d", num);
	CharToTchar(str, tchar);
}

void NumToTCHAR(double num, TCHAR *tchar)
{
	char str[50];
	sprintf(str, "%f", num);
	CharToTchar(str, tchar);
}

void NumToChar(int num, char * tchar)
{
	sprintf(tchar, "%d", num);
}

void NumToChar(double num, char * tchar)
{
	sprintf(tchar, "%f", num);
}

int TCHARToInt(const TCHAR *tchar)
{
	char str[50];
	int num;
	TcharToChar(tchar, str);
	sscanf(str, "%d", &num);
	return num;
}

double TCHARToDouble(const TCHAR *tchar)
{
	char str[50];
	TcharToChar(tchar, str);
	return atof(str);
}

void SystemTimeToTchar(const SYSTEMTIME * time, TCHAR * tchar)
{
	char str[50],year[50],month[50],day[50],*spit="-";
	for (int i = 0; i < 50; i++)
	{
		str[i] = '\0';
	}
	NumToChar((int)time->wYear, year);
	NumToChar((int)time->wMonth, month);
	NumToChar((int)time->wDay, day);
	int week = (int)time->wDayOfWeek;
	strcat(str, year);
	strcat(str, spit);
	strcat(str, month);
	strcat(str, spit);
	strcat(str, day);
	switch (week)
	{
	case 0:
		strcat(str, " 星期日");
		break;
	case 1:
		strcat(str, " 星期一");
		break;
	case 2:
		strcat(str, " 星期二");
		break;
	case 3:
		strcat(str, " 星期三");
		break;
	case 4:
		strcat(str, " 星期四");
		break;
	case 5:
		strcat(str, " 星期五");
		break;
	case 6:
		strcat(str, " 星期六");
		break;
	default:
		break;
	}
	CharToTchar(str, tchar);
}

void SystemTimeToChar(const SYSTEMTIME * time, char * str)
{
	char year[50], month[50], day[50], *spit = "-";
	for (int i = 0; i < 50; i++)
	{
		str[i] = '\0';
	}
	NumToChar((int)time->wYear, year);
	NumToChar((int)time->wMonth, month);
	NumToChar((int)time->wDay, day);
	int week = (int)time->wDayOfWeek;
	strcat(str, year);
	strcat(str, spit);
	strcat(str, month);
	strcat(str, spit);
	strcat(str, day);
	switch (week)
	{
	case 0:
		strcat(str, " 星期日");
		break;
	case 1:
		strcat(str, " 星期一");
		break;
	case 2:
		strcat(str, " 星期二");
		break;
	case 3:
		strcat(str, " 星期三");
		break;
	case 4:
		strcat(str, " 星期四");
		break;
	case 5:
		strcat(str, " 星期五");
		break;
	case 6:
		strcat(str, " 星期六");
		break;
	default:
		break;
	}
}

#pragma once

void TcharToChar(const TCHAR *tchar, char *_char);
void CharToTchar(const char *_char, TCHAR *tchar);
int TcharToInt(const TCHAR *tchar);
void NumToTCHAR(int num, TCHAR *tchar);
void NumToTCHAR(double num, TCHAR *tchar);
void NumToChar(int num, char *tchar);
void NumToChar(double num, char *tchar);
int TCHARToInt(const TCHAR *tchar);
double TCHARToDouble(const TCHAR *tchar);
void SystemTimeToTchar(const SYSTEMTIME* time, TCHAR *tchar);
void SystemTimeToChar(const SYSTEMTIME* time, char *str);
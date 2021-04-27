#pragma once


unsigned char* StrToLwrExt(unsigned char* pString);
int StrnCiCmp(const char* s1, const char* s2, size_t ztCount);
int StrCiCmp(const char* s1, const char* s2);
char* StrCiStr(const char* s1, const char* s2);
unsigned char* StrToUprExt(unsigned char* pString);

#ifdef _WIN32
string cp1251_to_utf8(const char* str);
#endif

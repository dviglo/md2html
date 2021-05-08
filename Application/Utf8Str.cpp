// https://stackoverflow.com/questions/36897781/how-to-uppercase-lowercase-utf-8-characters-in-c
// Модифицировано, чтобы компилировалось C++ компилятором

#include <stdlib.h>
#include <cstring>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

unsigned char* StrToLwrExt(unsigned char* pString)
{
    if (pString && *pString) {
        unsigned char* p = pString;
        unsigned char* pExtChar = 0;
        while (*p) {
            if ((*p >= 0x41) && (*p <= 0x5a)) // US ASCII
                (*p) += 0x20;
            else if (*p > 0xc0) {
                pExtChar = p;
                p++;
                switch (*pExtChar) {
                case 0xc3: // Latin 1
                    if ((*p >= 0x80)
                        && (*p <= 0x9e)
                        && (*p != 0x97))
                        (*p) += 0x20; // US ASCII shift
                    break;
                case 0xc4: // Latin Exteneded
                    if ((*p >= 0x80)
                        && (*p <= 0xb7)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0xb9)
                        && (*p <= 0xbe)
                        && (*p % 2)) // Odd
                        (*p)++; // Next char is lwr
                    else if (*p == 0xbf) {
                        *pExtChar = 0xc5;
                        (*p) = 0x80;
                    }
                    break;
                case 0xc5: // Latin Exteneded
                    if ((*p >= 0x80)
                        && (*p <= 0x88)
                        && (*p % 2)) // Odd
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0x8a)
                        && (*p <= 0xb7)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0xb9)
                        && (*p <= 0xbe)
                        && (*p % 2)) // Odd
                        (*p)++; // Next char is lwr
                    break;
                case 0xc6: // Latin Exteneded
                    switch (*p) {
                    case 0x82:
                    case 0x84:
                    case 0x87:
                    case 0x8b:
                    case 0x91:
                    case 0x98:
                    case 0xa0:
                    case 0xa2:
                    case 0xa4:
                    case 0xa7:
                    case 0xac:
                    case 0xaf:
                    case 0xb3:
                    case 0xb5:
                    case 0xb8:
                    case 0xbc:
                        (*p)++; // Next char is lwr
                        break;
                    default:
                        break;
                    }
                    break;
                case 0xc7: // Latin Exteneded
                    if (*p == 0x84)
                        (*p) = 0x86;
                    else if (*p == 0x85)
                        (*p)++; // Next char is lwr
                    else if (*p == 0x87)
                        (*p) = 0x89;
                    else if (*p == 0x88)
                        (*p)++; // Next char is lwr
                    else if (*p == 0x8a)
                        (*p) = 0x8c;
                    else if (*p == 0x8b)
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0x8d)
                        && (*p <= 0x9c)
                        && (*p % 2)) // Odd
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0x9e)
                        && (*p <= 0xaf)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    else if (*p == 0xb1)
                        (*p) = 0xb3;
                    else if (*p == 0xb2)
                        (*p)++; // Next char is lwr
                    else if (*p == 0xb4)
                        (*p)++; // Next char is lwr
                    else if (*p == 0xb8)
                        (*p)++; // Next char is lwr
                    else if (*p == 0xba)
                        (*p)++; // Next char is lwr
                    else if (*p == 0xbc)
                        (*p)++; // Next char is lwr
                    else if (*p == 0xbe)
                        (*p)++; // Next char is lwr
                    break;
                case 0xc8: // Latin Exteneded
                    if ((*p >= 0x80)
                        && (*p <= 0x9f)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0xa2)
                        && (*p <= 0xb3)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    else if (*p == 0xbb)
                        (*p)++; // Next char is lwr
                    break;
                case 0xcd: // Greek & Coptic
                    switch (*p) {
                    case 0xb0:
                    case 0xb2:
                    case 0xb6:
                        (*p)++; // Next char is lwr
                        break;
                    default:
                        if (*p == 0xbf) {
                            *pExtChar = 0xcf;
                            (*p) = 0xb3;
                        }
                        break;
                    }
                    break;
                case 0xce: // Greek & Coptic
                    if (*p == 0x86)
                        (*p) = 0xac;
                    else if (*p == 0x88)
                        (*p) = 0xad;
                    else if (*p == 0x89)
                        (*p) = 0xae;
                    else if (*p == 0x8a)
                        (*p) = 0xaf;
                    else if (*p == 0x8c) {
                        *pExtChar = 0xcf;
                        (*p) = 0x8c;
                    }
                    else if (*p == 0x8e) {
                        *pExtChar = 0xcf;
                        (*p) = 0x8d;
                    }
                    else if (*p == 0x8f) {
                        *pExtChar = 0xcf;
                        (*p) = 0x8e;
                    }
                    else if ((*p >= 0x91)
                        && (*p <= 0x9f))
                        (*p) += 0x20; // US ASCII shift
                    else if ((*p >= 0xa0)
                        && (*p <= 0xab)
                        && (*p != 0xa2)) {
                        *pExtChar = 0xcf;
                        (*p) -= 0x20;
                    }
                    break;
                case 0xcf: // Greek & Coptic
                    if (*p == 0x8f)
                        (*p) = 0xb4;
                    else if (*p == 0x91)
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0x98)
                        && (*p <= 0xaf)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    else if (*p == 0xb4)
                        (*p) = 0x91;
                    else if (*p == 0xb7)
                        (*p)++; // Next char is lwr
                    else if (*p == 0xb9)
                        (*p) = 0xb2;
                    else if (*p == 0xbb)
                        (*p)++; // Next char is lwr
                    else if (*p == 0xbd) {
                        *pExtChar = 0xcd;
                        (*p) = 0xbb;
                    }
                    else if (*p == 0xbe) {
                        *pExtChar = 0xcd;
                        (*p) = 0xbc;
                    }
                    else if (*p == 0xbf) {
                        *pExtChar = 0xcd;
                        (*p) = 0xbd;
                    }

                    break;
                case 0xd0: // Cyrillic
                    if ((*p >= 0x80)
                        && (*p <= 0x8f)) {
                        *pExtChar = 0xd1;
                        (*p) += 0x10;
                    }
                    else if ((*p >= 0x90)
                        && (*p <= 0x9f))
                        (*p) += 0x20; // US ASCII shift
                    else if ((*p >= 0xa0)
                        && (*p <= 0xaf)) {
                        *pExtChar = 0xd1;
                        (*p) -= 0x20;
                    }
                    break;
                case 0xd1: // Cyrillic supplement
                    if ((*p >= 0xa0)
                        && (*p <= 0xbf)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    break;
                case 0xd2: // Cyrillic supplement
                    if (*p == 0x80)
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0x8a)
                        && (*p <= 0xbf)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    break;
                case 0xd3: // Cyrillic supplement
                    if ((*p >= 0x81)
                        && (*p <= 0x8e)
                        && (*p % 2)) // Odd
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0x90)
                        && (*p <= 0xbf)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    break;
                case 0xd4: // Cyrillic supplement & Armenian
                    if ((*p >= 0x80)
                        && (*p <= 0xaf)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    else if ((*p >= 0xb1)
                        && (*p <= 0xbf)) {
                        *pExtChar = 0xd5;
                        (*p) -= 0x10;
                    }
                    break;
                case 0xd5: // Armenian
                    if ((*p >= 0x80)
                        && (*p <= 0x96)
                        && (!(*p % 2))) // Even
                        (*p)++; // Next char is lwr
                    break;
                case 0xe1: // Three byte code
                    pExtChar = p;
                    p++;
                    switch (*pExtChar) {
                    case 0x82: // Georgian
                        if ((*p >= 0xa0)
                            && (*p <= 0xbf)) {
                            *pExtChar = 0x83;
                            (*p) -= 0x10;
                        }
                        break;
                    case 0x83: // Georgian
                        if ((*p >= 0x80)
                            && ((*p <= 0x85)
                                || (*p == 0x87))
                            || (*p == 0x8d))
                            (*p) += 0x30;
                        break;
                    case 0xb8: // Latin extened
                        if ((*p >= 0x80)
                            && (*p <= 0xbf)
                            && (!(*p % 2))) // Even
                            (*p)++; // Next char is lwr
                        break;
                    case 0xb9: // Latin extened
                        if ((*p >= 0x80)
                            && (*p <= 0xbf)
                            && (!(*p % 2))) // Even
                            (*p)++; // Next char is lwr
                        break;
                    case 0xba: // Latin extened
                        if ((*p >= 0x80)
                            && (*p <= 0x94)
                            && (!(*p % 2))) // Even
                            (*p)++; // Next char is lwr
                        else if ((*p >= 0x9e)
                            && (*p <= 0xbf)
                            && (!(*p % 2))) // Even
                            (*p)++; // Next char is lwr
                        break;
                    case 0xbb: // Latin extened
                        if ((*p >= 0x80)
                            && (*p <= 0xbf)
                            && (!(*p % 2))) // Even
                            (*p)++; // Next char is lwr
                        break;
                    case 0xbc: // Greek extened
                        if ((*p >= 0x88)
                            && (*p <= 0x8f))
                            (*p) -= 0x08;
                        else if ((*p >= 0x98)
                            && (*p <= 0x9f))
                            (*p) -= 0x08;
                        else if ((*p >= 0xa8)
                            && (*p <= 0xaf))
                            (*p) -= 0x08;
                        else if ((*p >= 0xb8)
                            && (*p <= 0x8f))
                            (*p) -= 0x08;
                        break;
                    case 0xbd: // Greek extened
                        if ((*p >= 0x88)
                            && (*p <= 0x8d))
                            (*p) -= 0x08;
                        else if ((*p >= 0x98)
                            && (*p <= 0x9f))
                            (*p) -= 0x08;
                        else if ((*p >= 0xa8)
                            && (*p <= 0xaf))
                            (*p) -= 0x08;
                        else if ((*p >= 0xb8)
                            && (*p <= 0x8f))
                            (*p) -= 0x08;
                        break;
                    case 0xbe: // Greek extened
                        if ((*p >= 0x88)
                            && (*p <= 0x8f))
                            (*p) -= 0x08;
                        else if ((*p >= 0x98)
                            && (*p <= 0x9f))
                            (*p) -= 0x08;
                        else if ((*p >= 0xa8)
                            && (*p <= 0xaf))
                            (*p) -= 0x08;
                        else if ((*p >= 0xb8)
                            && (*p <= 0xb9))
                            (*p) -= 0x08;
                        break;
                    case 0xbf: // Greek extened
                        if ((*p >= 0x88)
                            && (*p <= 0x8c))
                            (*p) -= 0x08;
                        else if ((*p >= 0x98)
                            && (*p <= 0x9b))
                            (*p) -= 0x08;
                        else if ((*p >= 0xa8)
                            && (*p <= 0xac))
                            (*p) -= 0x08;
                        break;
                    default:
                        break;
                    }
                    break;
                case 0xf0: // Four byte code
                    pExtChar = p;
                    p++;
                    switch (*pExtChar) {
                    case 0x90:
                        pExtChar = p;
                        p++;
                        switch (*pExtChar) {
                        case 0x92: // Osage 
                            if ((*p >= 0xb0)
                                && (*p <= 0xbf)) {
                                *pExtChar = 0x93;
                                (*p) -= 0x18;
                            }
                            break;
                        case 0x93: // Osage 
                            if ((*p >= 0x80)
                                && (*p <= 0x93))
                                (*p) += 0x18;
                            break;
                        default:
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                    break;
                case 0x9E:
                    pExtChar = p;
                    p++;
                    switch (*pExtChar) {
                    case 0xA4: // Adlam
                        if ((*p >= 0x80)
                            && (*p <= 0xA1))
                            (*p) += 0x22;
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
                pExtChar = 0;
            }
            p++;
        }
    }
    return pString;
}

int StrnCiCmp(const char* s1, const char* s2, size_t ztCount)
{
    if (s1 && *s1 && s2 && *s2) {
        char cExtChar = 0;
        unsigned char* pStr1Low = (unsigned char*)calloc(strlen(s1) + 1, sizeof(char));
        unsigned char* pStr2Low = (unsigned char*)calloc(strlen(s2) + 1, sizeof(char));
        if (pStr1Low && pStr2Low) {
            unsigned char* p1 = pStr1Low;
            unsigned char* p2 = pStr2Low;
            strcpy((char*)pStr1Low, s1);
            strcpy((char*)pStr2Low, s2);
            StrToLwrExt(pStr1Low);
            StrToLwrExt(pStr2Low);
            for (; ztCount--; p1++, p2++) {
                int iDiff = *p1 - *p2;
                if (iDiff != 0 || !*p1 || !*p2) {
                    free(pStr1Low);
                    free(pStr2Low);
                    return iDiff;
                }
            }
            free(pStr1Low);
            free(pStr2Low);
            return 0;
        }
        return (-1);
    }
    return (-1);
}

int StrCiCmp(const char* s1, const char* s2)
{
    return StrnCiCmp(s1, s2, (size_t)(-1));
}

char* StrCiStr(const char* s1, const char* s2)
{
    if (s1 && *s1 && s2 && *s2) {
        char* p = (char*)s1;
        size_t len = strlen(s2);
        while (*p) {
            if (StrnCiCmp(p, s2, len) == 0)
                return (char*)p;
            p++;
        }
    }
    return (0);
}

unsigned char* StrToUprExt(unsigned char* pString)
{
    if (pString && *pString) {
        unsigned char* p = pString;
        unsigned char* pExtChar = 0;
        while (*p) {
            if ((*p >= 0x61) && (*p <= 0x7a)) // US ASCII
                (*p) -= 0x20;
            else if (*p > 0xc0) {
                pExtChar = p;
                p++;
                switch (*pExtChar) {
                case 0xc3: // Latin 1
                    if ((*p >= 0xa0)
                        && (*p <= 0xbe)
                        && (*p != 0xb7))
                        (*p) -= 0x20; // US ASCII shift
                    break;
                case 0xc4: // Latin Exteneded
                    if ((*p >= 0x80)
                        && (*p <= 0xb7)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0xb9)
                        && (*p <= 0xbe)
                        && (!(*p % 2))) // Even
                        (*p)--; // Prev char is upr
                    else if (*p == 0xbf) {
                        *pExtChar = 0xc5;
                        (*p) = 0x80;
                    }
                    break;
                case 0xc5: // Latin Exteneded
                    if ((*p >= 0x80)
                        && (*p <= 0x88)
                        && (!(*p % 2))) // Even
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0x8a)
                        && (*p <= 0xb7)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0xb9)
                        && (*p <= 0xbe)
                        && (!(*p % 2))) // Even
                        (*p)--; // Prev char is upr
                    break;
                case 0xc6: // Latin Exteneded
                    switch (*p) {
                    case 0x83:
                    case 0x85:
                    case 0x88:
                    case 0x8c:
                    case 0x92:
                    case 0x99:
                    case 0xa1:
                    case 0xa3:
                    case 0xa5:
                    case 0xa8:
                    case 0xad:
                    case 0xb0:
                    case 0xb4:
                    case 0xb6:
                    case 0xb9:
                    case 0xbd:
                        (*p)--; // Prev char is upr
                        break;
                    default:
                        break;
                    }
                    break;
                case 0xc7: // Latin Exteneded
                    if (*p == 0x86)
                        (*p) = 0x84;
                    else if (*p == 0x85)
                        (*p)--; // Prev char is upr
                    else if (*p == 0x89)
                        (*p) = 0x87;
                    else if (*p == 0x88)
                        (*p)--; // Prev char is upr
                    else if (*p == 0x8c)
                        (*p) = 0x8a;
                    else if (*p == 0x8b)
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0x8d)
                        && (*p <= 0x9c)
                        && (!(*p % 2))) // Even
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0x9e)
                        && (*p <= 0xaf)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    else if (*p == 0xb3)
                        (*p) = 0xb1;
                    else if (*p == 0xb2)
                        (*p)--; // Prev char is upr
                    else if (*p == 0xb4)
                        (*p)--; // Prev char is upr
                    else if (*p == 0xb8)
                        (*p)--; // Prev char is upr
                    else if (*p == 0xba)
                        (*p)--; // Prev char is upr
                    else if (*p == 0xbc)
                        (*p)--; // Prev char is upr
                    else if (*p == 0xbe)
                        (*p)--; // Prev char is upr
                    break;
                case 0xc8: // Latin Exteneded
                    if ((*p >= 0x80)
                        && (*p <= 0x9f)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0xa2)
                        && (*p <= 0xb3)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    else if (*p == 0xbc)
                        (*p)--; // Prev char is upr
                    break;
                case 0xcd: // Greek & Coptic
                    switch (*p) {
                    case 0xb1:
                    case 0xb3:
                    case 0xb7:
                        (*p)--; // Prev char is upr
                        break;
                    default:
                        if (*p == 0xbb) {
                            *pExtChar = 0xcf;
                            (*p) = 0xbd;
                        }
                        else if (*p == 0xbc) {
                            *pExtChar = 0xcf;
                            (*p) = 0xbe;
                        }
                        else if (*p == 0xbd) {
                            *pExtChar = 0xcf;
                            (*p) = 0xbf;
                        }
                        break;
                    }
                    break;
                case 0xce: // Greek & Coptic
                    if (*p == 0xac)
                        (*p) = 0x86;
                    else if (*p == 0xad)
                        (*p) = 0x88;
                    else if (*p == 0xae)
                        (*p) = 0x89;
                    else if (*p == 0xaf)
                        (*p) = 0x8a;
                    else if ((*p >= 0xb1)
                        && (*p <= 0xbf))
                        (*p) -= 0x20; // US ASCII shift
                    break;
                case 0xcf: // Greek & Coptic
                    if (*p == 0xb4)
                        (*p) = 0x8f;
                    else if (*p == 0x92)
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0x98)
                        && (*p <= 0xaf)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    else if (*p == 0x91)
                        (*p) = 0xb4;
                    else if (*p == 0xb8)
                        (*p)--; // Prev char is upr
                    else if (*p == 0xb2)
                        (*p) = 0xb9;
                    else if (*p == 0xbc)
                        (*p)--; // Prev char is upr
                    else if (*p == 0x8c) {
                        *pExtChar = 0xce;
                        (*p) = 0x8c;
                    }
                    else if (*p == 0x8d) {
                        *pExtChar = 0xce;
                        (*p) = 0x8e;
                    }
                    else if (*p == 0x8e) {
                        *pExtChar = 0xce;
                        (*p) = 0x8f;
                    }
                    else if ((*p >= 0x80)
                        && (*p <= 0x8b)
                        && (*p != 0x82)) {
                        *pExtChar = 0xce;
                        (*p) += 0x20;
                    }
                    else if (*p == 0xb3) {
                        *pExtChar = 0xcd;
                        (*p) = 0xbf;
                    }
                    break;
                case 0xd0: // Cyrillic
                    if ((*p >= 0xb0)
                        && (*p <= 0xbf))
                        (*p) -= 0x20; // US ASCII shift
                    break;
                case 0xd1: // Cyrillic supplement
                    if ((*p >= 0x90)
                        && (*p <= 0x9f)) {
                        *pExtChar = 0xd0;
                        (*p) -= 0x10;
                    }
                    else if ((*p >= 0x80)
                        && (*p <= 0x8f)) {
                        *pExtChar = 0xd0;
                        (*p) += 0x20;
                    }
                    else if ((*p >= 0xa0)
                        && (*p <= 0xbf)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    break;
                case 0xd2: // Cyrillic supplement
                    if (*p == 0x80)
                        (*p)++; // Prev char is upr
                    else if ((*p >= 0x8a)
                        && (*p <= 0xbf)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    break;
                case 0xd3: // Cyrillic supplement
                    if ((*p >= 0x81)
                        && (*p <= 0x8e)
                        && (!(*p % 2))) // Even
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0x90)
                        && (*p <= 0xbf)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    break;
                case 0xd4: // Cyrillic supplement & Armenian
                    if ((*p >= 0x80)
                        && (*p <= 0xaf)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    break;
                case 0xd5: // Armenian
                    if ((*p >= 0x80)
                        && (*p <= 0x96)
                        && (*p % 2)) // Odd
                        (*p)--; // Prev char is upr
                    else if ((*p >= 0xa1)
                        && (*p <= 0xaf)) {
                        *pExtChar = 0xd4;
                        (*p) += 0x10;
                    }
                    break;
                case 0xe1: // Three byte code
                    pExtChar = p;
                    p++;
                    switch (*pExtChar) {
                    case 0x82: // Georgian
                        break;
                    case 0x83: // Georgian
                        if ((*p >= 0x90)
                            && (*p <= 0xaf)) {
                            *pExtChar = 0x82;
                            (*p) += 0x10;
                        }
                        else if ((*p >= 0xb0)
                            && ((*p <= 0xb5)
                                || (*p == 0xb7))
                            || (*p == 0xbd))
                            (*p) -= 0x30;
                        break;
                    case 0xb8: // Latin extened
                        if ((*p >= 0x80)
                            && (*p <= 0xbf)
                            && (*p % 2)) // Odd
                            (*p)--; // Prev char is upr
                        break;
                    case 0xb9: // Latin extened
                        if ((*p >= 0x80)
                            && (*p <= 0xbf)
                            && (*p % 2)) // Odd
                            (*p)--; // Prev char is upr
                        break;
                    case 0xba: // Latin extened
                        if ((*p >= 0x80)
                            && (*p <= 0x94)
                            && (*p % 2)) // Odd
                            (*p)--; // Prev char is upr
                        else if ((*p >= 0x9e)
                            && (*p <= 0xbf)
                            && (*p % 2)) // Odd
                            (*p)--; // Prev char is upr
                        break;
                    case 0xbb: // Latin extened
                        if ((*p >= 0x80)
                            && (*p <= 0xbf)
                            && (*p % 2)) // Odd
                            (*p)--; // Prev char is upr
                        break;
                    case 0xbc: // Greek extened
                        if ((*p >= 0x80)
                            && (*p <= 0x87))
                            (*p) += 0x08;
                        else if ((*p >= 0x90)
                            && (*p <= 0x97))
                            (*p) += 0x08;
                        else if ((*p >= 0xa0)
                            && (*p <= 0xa7))
                            (*p) += 0x08;
                        else if ((*p >= 0xb0)
                            && (*p <= 0x87))
                            (*p) += 0x08;
                        break;
                    case 0xbd: // Greek extened
                        if ((*p >= 0x80)
                            && (*p <= 0x87))
                            (*p) += 0x08;
                        else if ((*p >= 0x90)
                            && (*p <= 0x97))
                            (*p) += 0x08;
                        else if ((*p >= 0xa0)
                            && (*p <= 0xa7))
                            (*p) += 0x08;
                        else if ((*p >= 0xb0)
                            && (*p <= 0x87))
                            (*p) += 0x08;
                        break;
                    case 0xbe: // Greek extened
                        if ((*p >= 0x80)
                            && (*p <= 0x87))
                            (*p) += 0x08;
                        else if ((*p >= 0x90)
                            && (*p <= 0x97))
                            (*p) += 0x08;
                        else if ((*p >= 0xa0)
                            && (*p <= 0xa7))
                            (*p) += 0x08;
                        else if ((*p >= 0xb0)
                            && (*p <= 0xb1))
                            (*p) += 0x08;
                        break;
                    case 0xbf: // Greek extened
                        if ((*p >= 0x80)
                            && (*p <= 0x84))
                            (*p) += 0x08;
                        else if ((*p >= 0x90)
                            && (*p <= 0x93))
                            (*p) += 0x08;
                        else if ((*p >= 0xa0)
                            && (*p <= 0xa4))
                            (*p) += 0x08;
                        break;
                    default:
                        break;
                    }
                    break;
                case 0xf0: // Four byte code
                    pExtChar = p;
                    p++;
                    switch (*pExtChar) {
                    case 0x90:
                        pExtChar = p;
                        p++;
                        switch (*pExtChar) {
                        case 0x92: // Osage 
                            break;
                        case 0x93: // Osage 
                            if ((*p >= 0x80)
                                && (*p <= 0x93))
                                (*p) += 0x18;
                            else if ((*p >= 0x98)
                                && (*p <= 0xa7)) {
                                *pExtChar = 0x92;
                                (*p) += 0x18;
                            }
                            break;
                        default:
                            break;
                        }
                        break;
                    case 0x9E:
                        pExtChar = p;
                        p++;
                        switch (*pExtChar) {
                        case 0xA4: // Adlam     
                            if ((*p >= 0xa2)
                                && (*p <= 0xc3))
                                (*p) -= 0x22;
                            break;
                        default:
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
                pExtChar = 0;
            }
            p++;
        }
    }
    return pString;
}


#ifdef _WIN32

// https://www.linux.org.ru/forum/development/3968525
// Изменено

string cp1251_to_utf8(const char* str) {
    string res;
    int result_u, result_c;
    result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
    if (!result_u) { return string(); }
    wchar_t* ures = new wchar_t[result_u];
    if (!MultiByteToWideChar(1251, 0, str, -1, ures, result_u)) {
        delete[] ures;
        return string();
    }
    result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
    if (!result_c) {
        delete[] ures;
        return string();
    }
    char* cres = new char[result_c];
    if (!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)) {
        delete[] cres;
        return string();
    }
    delete[] ures;
    res.append(cres);
    delete[] cres;
    return res;
}

#endif

#pragma once
#include <Windows.h>
#include <minwindef.h>
#include <set>
#include <map>

namespace stringUtil
{
    const unsigned int MAX_BUF_SIZE = 4096;

    const char* unicodeToAnsi(const wchar_t *buffer);
    const wchar_t* AnsiToUnicode(const char *buffer);
};

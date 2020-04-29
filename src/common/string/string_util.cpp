#include "string_util.h"


const char * stringUtil::unicodeToAnsi(const wchar_t * buffer) {
    static char s_buf[MAX_BUF_SIZE];

    WideCharToMultiByte(CP_ACP, 0, buffer, -1, s_buf, MAX_BUF_SIZE - 1, 0, 0);
    s_buf[MAX_BUF_SIZE - 1] = '\0';

    return s_buf;
}

const wchar_t * stringUtil::AnsiToUnicode(const char * buffer) {
    static wchar_t s_buf[MAX_BUF_SIZE];

    MultiByteToWideChar(CP_ACP, 0, buffer, -1, s_buf, MAX_BUF_SIZE - 1);
    s_buf[MAX_BUF_SIZE - 1] = '\0';

    return s_buf;
}

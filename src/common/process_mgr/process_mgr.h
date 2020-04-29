#pragma once
#include <Windows.h>
#include <minwindef.h>
#include <set>
#include <string>

namespace ProcessMgr {
    void getThisProcessName(std::wstring &strProcessName);
    void getThisProcessPath(std::wstring &strProcessPath);
    void getProcessIdByName(const std::wstring &strProcessName, std::set<DWORD> &setProcessId);

    bool killProcess(DWORD dwProcessId);
    bool isRepeted();
    bool isRepeted(const std::wstring &strProcessName);
}

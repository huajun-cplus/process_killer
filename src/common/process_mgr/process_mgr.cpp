#include "process_mgr.h"
#include <tlhelp32.h>


void ProcessMgr::getThisProcessName(std::wstring &strProcessName) {
    ProcessMgr::getThisProcessPath(strProcessName);
    strProcessName = strProcessName.substr(strProcessName.find_last_of(L"\\") + 1);
}

void ProcessMgr::getThisProcessPath(std::wstring & strProcessPath) {
    TCHAR szModulePath[_MAX_PATH] = { 0 };
    GetModuleFileNameW(nullptr, szModulePath, _countof(szModulePath));
    strProcessPath = szModulePath;
}

void ProcessMgr::getProcessIdByName(const std::wstring & strProcessName, std::set<DWORD> &setProcessId) {
    if (strProcessName.empty()) {
        return;
    }

    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapShot, &pe)) {
        return;
    }

    while (Process32Next(hSnapShot, &pe)) {
        if (std::wstring(pe.szExeFile) != strProcessName) {
            continue;
        }

        setProcessId.insert(pe.th32ProcessID);
    }
}

bool ProcessMgr::killProcess(DWORD dwProcessId) {
    HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
    if (INVALID_HANDLE_VALUE == hProcess) {
        return false;
    }

    TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);

    return true;
}

bool ProcessMgr::isRepeted() {
    std::wstring strThisProcessNmae;
    getThisProcessName(strThisProcessNmae);

    return isRepeted(strThisProcessNmae);
}

bool ProcessMgr::isRepeted(const std::wstring & strProcessName) {
    std::set<DWORD> setProcessId;

    getProcessIdByName(strProcessName, setProcessId);

    return setProcessId.size() > 1;
}

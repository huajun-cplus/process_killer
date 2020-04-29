// process_killer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <chrono>
#include <thread>
#include "common/process_mgr/process_mgr.h"
#include "common/string/string_util.h"
#include "common/key_hook/key_hook.h"

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
    // 进程单开
    if (ProcessMgr::isRepeted()) {
        std::cerr << "you have started this process" << std::endl;
        std::cerr << "the program will end in 10 seconds" << std::endl;
        std::this_thread::sleep_for(10s);
        exit(0);
    }

    // 参数
    if (2 != argc) {
        std::cerr << "please enter the name of the process to be killed" << std::endl;
        std::cerr << "the program will end in 10 seconds" << std::endl;
        std::this_thread::sleep_for(10s);
        exit(-1);
    }

    std::wstring strTarProcessName(stringUtil::AnsiToUnicode(argv[1]));

    // 注册
    KeyHook::getInstance().reg(
        'K'
        , { VK_CONTROL, VK_SHIFT }
        , [&strTarProcessName] {
            std::set<DWORD> setProcessId;
            ProcessMgr::getProcessIdByName(strTarProcessName, setProcessId);
            if (setProcessId.empty()) {
                std::wcerr << strTarProcessName << L" not found" << std::endl;
                return;
            }

            int nSucCount = 0;
            for (auto dwId : setProcessId) {
                if (!ProcessMgr::killProcess(dwId)) {
                    continue;
                }

                ++nSucCount;
            }
            std::wcout << nSucCount << L" " << strTarProcessName << L" process be killed" << std::endl;
        });

    std::wcout << L"press the key combination to kill " << strTarProcessName << std::endl;

    // run
    KeyHook::getInstance().run();
}

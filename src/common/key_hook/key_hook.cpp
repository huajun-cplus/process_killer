#include "key_hook.h"


bool KeyHook::KeyHookContext::isHooked() const {
    for (auto nKey : setCompositekeys) {
        if (GetKeyState(nKey) >= 0) {
            return false;
        }
    }

    return true;
}

KeyHook::KeyHookContext::KeyHookContext(
    const std::set<int> &setCompositekeys
    , const std::function<void()> &funCallBack) {
    this->setCompositekeys = setCompositekeys;
    this->funCallBack = funCallBack;
}

KeyHook::KeyHook()
    :m_bSkip(false) {
}

KeyHook & KeyHook::getInstance() {
    static KeyHook s_keyHook;
    return s_keyHook;
}

LRESULT KeyHook::handleHook(int nCode, WPARAM wParam, LPARAM lParam) {
    return KeyHook::getInstance().handleHookInternal(nCode, wParam, lParam);
}

void KeyHook::reg(int nTriggerKey, const std::function<void(void)> &funCallBack) {
    m_mapKeyHook[nTriggerKey].emplace_back(std::set<int>(), funCallBack);
}

void KeyHook::reg(int nTriggerKey, const std::set<int>& setCompositeKeys, const std::function<void(void)>& funCallBack) {
    m_mapKeyHook[nTriggerKey].emplace_back(setCompositeKeys, funCallBack);
}

void KeyHook::run() {
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, KeyHook::handleHook, 0, 0);

    MSG msg;
    while (!GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
}

LRESULT KeyHook::handleHookInternal(int nCode, WPARAM wParam, LPARAM lParam) {
    BOOL fEatKeystroke = FALSE;

    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
    if (nCode == HC_ACTION) {
        switch (wParam) {
        case WM_KEYDOWN:
            {
                if (m_bSkip) {
                    break;
                }

                auto itCompositekey = m_mapKeyHook.find(p->vkCode);
                if (m_mapKeyHook.end() == itCompositekey) {
                    break;
                }

                for (auto context : itCompositekey->second) {
                    if (!context.isHooked()) {
                        continue;
                    }

                    context.funCallBack();

                    m_bSkip = true;
                }

            }
            break;

        case WM_KEYUP:
            {
                m_bSkip = false;
            }
            break;
        }
    }

    return m_bSkip ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam);
}

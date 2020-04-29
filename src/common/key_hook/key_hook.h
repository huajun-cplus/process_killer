#pragma once
#include <Windows.h>
#include <minwindef.h>
#include <set>
#include <map>
#include <vector>
#include <functional>


class KeyHook
{

struct KeyHookContext
{
    std::set<int>               setCompositekeys;
    std::function<void()>       funCallBack;

    bool isHooked() const;

    KeyHookContext(const std::set<int> &setCompositekeys, const std::function<void()> &funCallBack);
};

public:
    KeyHook();

    static KeyHook & getInstance();
    static LRESULT CALLBACK handleHook(int nCode, WPARAM wParam, LPARAM lParam);

public:
    void reg(int nTriggerKey, const std::function<void()> &funCallBack);
    void reg(int nTriggerKey, const std::set<int> &setCompositeKeys, const std::function<void()> &funCallBack);
    void run();

private:
    LRESULT CALLBACK handleHookInternal(int nCode, WPARAM wParam, LPARAM lParam);

private:
    std::map<int, std::vector<KeyHookContext>>  m_mapKeyHook;
    bool                                        m_bSkip;
};

#include "pch.h"
#include <Windows.h>
#include <iostream>

extern "C" __declspec(dllexport) void move_mouse_rel(int x, int y)
{
    mouse_event(1, x, y, 0, 0);
}

extern "C" __declspec(dllexport) bool async_keystate(int key)
{
    if (GetAsyncKeyState(key)) {
        return true;
    }
    else {
        return false;
    }
}

extern "C" __declspec(dllexport) const char* get_active_window_title()
{
    HWND fg = GetForegroundWindow();
    if (fg)
    {
        char title[256];
        GetWindowTextA(fg, title, 256);
        return _strdup(title);
    }
    return _strdup("Nope.avi");
}

extern "C" __declspec(dllexport)
void read_proc_mem(char* title, uintptr_t start_loc, uintptr_t out)
{
    HWND hwnd = FindWindowA(0, title);
    DWORD pid;

    if (!hwnd)      return;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE h = OpenProcess(PROCESS_VM_READ, 0, pid);

    if (!h)     return;
    ReadProcessMemory(h, (LPCVOID)start_loc, &out, sizeof(out), NULL);
}

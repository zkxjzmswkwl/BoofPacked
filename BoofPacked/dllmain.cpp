#include "pch.h"
#include <Windows.h>
#include <iostream>
#include "xxhash.h"

#define _INTERCEPTION_SUPPORT

#ifdef _INTERCEPTION_SUPPORT
#include "interception.h"
#endif

extern "C" __declspec(dllexport)
void move_mouse_rel(int x, int y)
{
    mouse_event(1, x, y, 0, 0);
}

extern "C" __declspec(dllexport)
bool async_keystate(int key)
{
    if (GetAsyncKeyState(key)) {
        return true;
    }
    else {
        return false;
    }
}

/**
* Useful when you know the title of a window but it's sure to change.
*/
extern "C" __declspec(dllexport)
HWND get_hwnd_by_title(char* windowTitle)
{
    return FindWindowA(0, windowTitle);
}

extern "C" __declspec(dllexport)
HANDLE get_handle_by_title(char* windowTitle)
{
    HWND hwnd = FindWindowA(0, windowTitle);
    if (!hwnd)  return NULL;

    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (!hproc) return NULL;

    return hproc;
}

extern "C" __declspec(dllexport)
const char* get_title_by_hwnd(HWND hwnd)
{
    char out[256];
    GetWindowTextA(hwnd, out, 255);
    return _strdup(out);
}

extern "C" __declspec(dllexport)
const char* get_active_window_title()
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

extern "C" __declspec(dllexport)
uint32_t hash_xxh32(char* to_hash)
{
    return XXH32(to_hash, strlen(to_hash), 77777UL);
}

#ifdef _INTERCEPTION_SUPPORT
extern "C" __declspec(dllexport)
void interception_click()
{
    InterceptionContext context;
    context = interception_create_context();
    InterceptionMouseStroke mouse_stroke[2];
    ZeroMemory(mouse_stroke, sizeof(mouse_stroke));
    mouse_stroke[0].state = INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN;
    mouse_stroke[1].state = INTERCEPTION_MOUSE_LEFT_BUTTON_UP;
    interception_send(context, INTERCEPTION_MOUSE(0), (InterceptionStroke*)mouse_stroke, _countof(mouse_stroke));
    interception_destroy_context(context);
}
#endif

/**
 * Simple program that uses hotkeys to store and restore mouse position.
 * Usefull if you have a large (or many) screens.
 */
#include <windows.h>
#include "hotkeys.h"

HWND Init(HINSTANCE hInstance);
ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Cleanup(HWND hWnd);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd = Init(hInstance);
    MSG msg;

    if (!hWnd)
    {
        LPVOID msgBuf;
        DWORD error = GetLastError();

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                      FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS, 
                      NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (LPSTR)&msgBuf, 0, NULL);

        MessageBox(NULL, (LPCSTR)msgBuf, (LPCSTR)"ERROR", MB_ICONERROR);
        return FALSE;
    }

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Cleanup(hWnd);

    return (int)msg.wParam;
}

HWND Init(HINSTANCE hInstance)
{
    if (!MyRegisterClass(hInstance))
    {
        return NULL;
    }

    HWND hWnd = CreateWindow("Mouser", "Mouser", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL,
                             NULL, hInstance, NULL);

    if (hWnd)
    {
        if (!RegisterHotkeys(hWnd))
        {
            return NULL;
        }
        UpdateWindow(hWnd);
    }

    return hWnd;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = 0;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = NULL;
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = "Mouser";
    wcex.hIconSm        = NULL;

    return RegisterClassEx(&wcex);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_HOTKEY:
        HandleHotkey(LOWORD(lParam), HIWORD(lParam));
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Cleanup(HWND hWnd)
{
    UnregisterHotkeys(hWnd);
}

#include <windows.h>
#include <assert.h>
#include "hotkeys.h"

// Local Variables
static const int MAX_HOTKEYS = 10;

static const int MOD_STORE_POSITION   = MOD_ALT;
static const int MOD_RESTORE_POSITION = MOD_CONTROL;

// if you change the hotkeys you may have to change FindHotkeyIndex
static const char hotkeys[MAX_HOTKEYS] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

static POINT cursorPositions[MAX_HOTKEYS];
static BOOL cursorPositionIsSet[MAX_HOTKEYS];

// Local Functions
static int FindHotkeyIndex(char c);

BOOL RegisterHotkeys(HWND hWnd)
{
    BOOL success = TRUE;

    for (int i = 0; i < MAX_HOTKEYS && success; ++i)
    {
        if (!RegisterHotKey(hWnd, i, MOD_RESTORE_POSITION, hotkeys[i]))
            success = FALSE;
        if (!RegisterHotKey(hWnd, MAX_HOTKEYS + i, MOD_STORE_POSITION, hotkeys[i]))
            success = FALSE;
    }

    if (!success)
    {
        UnregisterHotkeys(hWnd);
    }

    return success;
}

void UnregisterHotkeys(HWND hWnd)
{
    // Hold & restore the last error incase we unregister a hotkey that we
    // didn't register, so we don't clobber the last error for a more important
    // error.
    int error = GetLastError();

    for (int i = 0; i < MAX_HOTKEYS; ++i)
    {
        UnregisterHotKey(hWnd, i);
        UnregisterHotKey(hWnd, MAX_HOTKEYS + i);
    }

    SetLastError(error);
}

void HandleHotkey(int modifier, int key)
{
    int i = FindHotkeyIndex(key);

    if (modifier == MOD_STORE_POSITION)
    {
        cursorPositionIsSet[i] = TRUE;
        GetCursorPos(&cursorPositions[i]);
    }
    else if (modifier == MOD_RESTORE_POSITION)
    {
        if (cursorPositionIsSet[i])
        {
            SetCursorPos(cursorPositions[i].x, cursorPositions[i].y);

			HWND overWindow = WindowFromPoint(cursorPositions[i]);
			if (overWindow)
			{
				SetForegroundWindow(overWindow);
			}
        }
        else
        {
            // No warning, no nothing.
        }
    }
    else
    {
        // No warning, no nothing.
    }
}

/*
 * Given the key pressed for the hotkey, this finds the index to use for the
 * cursor positioning data.
 */
static int FindHotkeyIndex(char c)
{
    assert(c <= '9' && c >= '0');
    return c - '0';
}

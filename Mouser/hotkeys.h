#pragma once

// Registers the hotkeys to be used.
BOOL RegisterHotkeys(HWND hWnd);

// Unregisters the hotkeys used.
void UnregisterHotkeys(HWND hWnd);

// Handles pressed hotkeys.
void HandleHotkey(int modifier, int key);
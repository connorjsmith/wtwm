/*
 * we may have to switch wtwm to be separated into a hotkey daemon and the actual WM.
 * advantages: environment variables for wtwm commands (cmd would work fine)
 * disadvantages: more complex program.
 */
#pragma once
#include "stdafx.h"

#define ERROR_CODE -1
typedef KBDLLHOOKSTRUCT KeyInfo;

LRESULT CALLBACK hook_KeyboardHandler(int, WPARAM, LPARAM);
void populateModifierKeyState(bool& winHeld, bool& ctrlHeld, bool& altHeld);

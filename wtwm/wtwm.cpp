// wtwm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "wtwm.h"
#include "parse.h"
#include <iostream>

using namespace std;

/* Globals */
string path_keybindings = "keybindings.cfg";
HHOOK kbHook; // used to propagate the keypress to the rest of the system
// Keybindings keybindings = null;
// bool(*keybindings[2][2][2][2][256])(); // [shift][ctrl][alt][win][ASCII code]

/* Main Event Loop and Entry Point */
int main() {
	// keybindings = new Keybindings(path_keybindings);

	load_config(path_keybindings.c_str());

	return 0;
	MSG msg;
	int BRET;
	kbHook = SetWindowsHookEx(WH_KEYBOARD_LL, wtwm::hook_KeyboardHandler, nullptr, 0);
	while ((BRET = GetMessage(&msg, NULL, NULL, NULL)) != 0) {
		if (BRET < 0) {
			cout << "Error" << endl;
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(kbHook);
    return 0;
}

/* Function Definitions */
LRESULT CALLBACK wtwm::hook_KeyboardHandler(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) {
		return CallNextHookEx(kbHook, nCode, wParam, lParam);
	}
	if (wParam == WM_KEYDOWN) {
		KeyInfo* keypress = (KeyInfo*)lParam;
		bool winHeld, ctrlHeld, altHeld;
		PopulateModifierKeyState(winHeld, ctrlHeld, altHeld);
		cout << "Key pressed ";
		if (winHeld) {
			cout << "Win + ";
		}
		if (ctrlHeld) {
			cout << "Ctrl + ";
		}
		if (altHeld) {
			cout << "Alt + ";
		}
		cout << keypress->vkCode << endl;
	} else if (wParam == WM_SYSKEYDOWN) {
		KeyInfo* keypress = (KeyInfo*)lParam;
		cout << "Syskey down " << keypress->vkCode << endl;
	}
	// Allow the key to propagate to the rest of the system
	return CallNextHookEx(kbHook, nCode, wParam, lParam);
}

void wtwm::PopulateModifierKeyState(bool& winHeld, bool& ctrlHeld, bool& altHeld) {
	winHeld =  (GetAsyncKeyState(VK_LWIN) != 0)     || (GetAsyncKeyState(VK_RWIN) != 0);
	ctrlHeld = (GetAsyncKeyState(VK_LCONTROL) != 0) || (GetAsyncKeyState(VK_RCONTROL != 0));
	altHeld =  (GetAsyncKeyState(VK_MENU) != 0);
}

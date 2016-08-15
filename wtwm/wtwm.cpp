// wtwm.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "wtwm.h"
#include "parse_config.h" // parsing config files => pair<string, string>
#include "parse_functions.h" // parsing pair<string, string> => pair<string, fn_ptr>
// #include "hotkey.h" // parsing pair<string, fn_ptr> => pair<Hotkey, fn_ptr>
#include "keybindings.h"
#include <iostream>

using namespace std;

/* Globals */
string default_path_keybindings = "keybindings.cfg";
wtwm::Hotkeys hotkeys;
HHOOK kbHook; // used to propagate the keypress to the rest of the system

/* Main Event Loop and Entry Point */
int main() {
	// TODO: read commandline switch for an override config
	vector<pair<string, string>> keybindingPairs = load_config(default_path_keybindings);
	if (keybindingPairs.empty()) {
		cout << "Failed to fully parse the keybindings from the config file. Exiting..." << endl;
		return ERROR_CODE;
	}
	vector<pair<string, fn_ptr>> keybindingFuncs = parseFunctions(keybindingPairs);
	if (keybindingFuncs.empty()) {
		cout << "Failed to fully parse the functions specified in the config file. Exiting..." << endl;
		return ERROR_CODE;
	}
	/*
	vector<pair<wtwm::Hotkey, fn_ptr>> hotkeyFunctions = parseHotkeys(keybindingFuncs);
	keybindingPairs.clear();
	keybindingFuncs.clear();
	for (auto binding : hotkeyFunctions) {
		bool success = hotkeys.bindHotkey(binding.first, binding.second);
		if (!success) {
			return ERROR_CODE;
		}
	}
	*/
	return 0;
	MSG msg;
	int BRET;
	kbHook = SetWindowsHookEx(WH_KEYBOARD_LL, hook_KeyboardHandler, nullptr, 0);
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
LRESULT CALLBACK hook_KeyboardHandler(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0) {
		return CallNextHookEx(kbHook, nCode, wParam, lParam);
	}
	if (wParam == WM_KEYDOWN) {
		KeyInfo* keypress = (KeyInfo*)lParam;
		bool winHeld, ctrlHeld, altHeld;
		populateModifierKeyState(winHeld, ctrlHeld, altHeld);
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

void populateModifierKeyState(bool& winHeld, bool& ctrlHeld, bool& altHeld) {
	winHeld =  (GetAsyncKeyState(VK_LWIN) != 0)     || (GetAsyncKeyState(VK_RWIN) != 0);
	ctrlHeld = (GetAsyncKeyState(VK_LCONTROL) != 0) || (GetAsyncKeyState(VK_RCONTROL != 0));
	altHeld =  (GetAsyncKeyState(VK_MENU) != 0);
}

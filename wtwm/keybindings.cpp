#include "stdafx.h"
#include "keybindings.h"
#include <iostream>

std::unordered_map<std::string, short> VK_Mapping({
		{"A",0x41},
		{"B",0x42},
		{"C",0x43},
		{"D",0x44},
		{"E",0x45},
		{"F",0x46},
		{"G",0x47},
		{"H",0x48},
		{"I",0x49},
		{"J",0x4A},
		{"K",0x4B},
		{"L",0x4C},
		{"M",0x4D},
		{"N",0x4E},
		{"O",0x4F},
		{"P",0x50},
		{"Q",0x51},
		{"R",0x52},
		{"S",0x53},
		{"T",0x54},
		{"U",0x55},
		{"V",0x56},
		{"W",0x57},
		{"X",0x58},
		{"Y",0x59},
		{"Apps",VK_APPS},
		{"F1",VK_F1},
		{"F2",VK_F2},
		{"F3",VK_F3},
		{"F4",VK_F4},
		{"F5",VK_F5},
		{"F6",VK_F6},
		{"F7",VK_F7},
		{"F8",VK_F8},
		{"F9",VK_F9},
		{"F10",VK_F10},
		{"F11",VK_F11},
		{"F12",VK_F12},
});

wtwm::Hotkey::Hotkey(std::string hotkey) {
	// TODO: Parse the hotkey string for modifiers, keypresses
	// probably use a stringstream?
}

const std::string wtwm::Hotkey::hashString() {
	std::stringstream ss;
	// TODO: fix this hash
	if (alt) ss << wtwm::altStr;
	if (ctrl) ss << wtwm::ctrlStr;
	if (shift) ss << wtwm::shiftStr;
	if (win) ss << wtwm::winStr;
	if (menu) ss << wtwm::menuStr;
	if (caps) ss << wtwm::capsStr;
	return ss.str();
}

// TOOD: Move this to the functions.h file
fn_ptr createFunction(std::string command) {
	return nullptr;
}

bool wtwm::Hotkeys::bindHotkey(wtwm::Hotkey& hotkey, fn_ptr command) {
	// TODO: add this to the dictionary
	const std::string hash = hotkey.hashString();
	if (keymap.find(hash) == keymap.end()) {
		keymap[hash] = command;
		return true;
	}
	// hotkey was already bound
	std::cout << "Hotkey " << hash << " was bound twice.";
	return false;
}
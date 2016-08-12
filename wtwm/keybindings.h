#pragma once
#include "stdafx.h"
#include <unordered_map>
#include <fstream>
#include <sstream>


typedef void(*fn_ptr)(void);

namespace wtwm {
	// Constants
	const std::string capsStr	= "Caps";
	const std::string shiftStr	= "Shift";
	const std::string altStr	= "Alt";
	const std::string ctrlStr	= "Ctrl";
	const std::string winStr	= "Win";
	const std::string menuStr	= "Menu";


	// Classes
	class Hotkey {
	public:
		bool shift	= false	,
			 alt	= false	,
			 ctrl	= false	,
			 win	= false	,
			 menu	= false	,
			 caps	= false
		;
		short key; // virtual keycodes
		Hotkey(std::string hotkey);
		const std::string hashString();
	};

	class Hotkeys {
	private:
		std::unordered_map<std::string, fn_ptr> keymap;
	public:
		// TODO: determine format for commands. cmd::, bash::, wtwm:: namespacing?
		bool bindHotkey(Hotkey& hotkey, fn_ptr command);
		bool unbindHotkey(Hotkey& hotkey);
		fn_ptr getFunction(Hotkey& hotkey);
	};
}

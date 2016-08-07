#pragma once
#include "stdafx.h"

// Probably only needed for debugging
#include <string>


#define ERROR_CODE -1
typedef KBDLLHOOKSTRUCT KeyInfo;

namespace wtwm {
	LRESULT CALLBACK hook_KeyboardHandler(int, WPARAM, LPARAM);
	void PopulateModifierKeyState(bool& winHeld, bool& ctrlHeld, bool& altHeld);
}

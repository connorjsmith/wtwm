#pragma once
#include "stdafx.h"

// Probably only needed for debugging
#include <iostream>
#include <string>

typedef KBDLLHOOKSTRUCT KeyInfo;

LRESULT CALLBACK hook_KeyboardHandler(int, WPARAM, LPARAM);
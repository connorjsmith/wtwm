// TODO: Make this a keybindings class, add lookup etc. for functions
//		 Be able to read starting definitions from a file when specified
#include "stdafx.h"
#include "keybindings.h"
#include <iostream>


void wtwm::populateKeybindings(const std::string& path) {
	std::ifstream cfgFile(path, std::ifstream::in);
	std::string line;
	while (std::getline(cfgFile, line)) {
		// ignore comments
		if (line.find_first_not_of(wtwm::whitespace) != wtwm::commentCharacter) {
			bool success = wtwm::addKeybinding(line);
			if (!success) {
				std::cout << "ERROR: Failed to parse the following line in config file '" << path << "':" << std::endl;
				std::cout << line << std::endl;
				exit(ERROR_CODE);
			}
		}
	}
}
bool wtwm::addKeybinding(const std::string& binding) {
	std::stringstream parseableBinding(binding);
	std::string token;
	bool shift, ctrl, alt, win, capsLock;
	short key = 0;
	while (parseableBinding >> token) {
		if (token.compare(wtwm::shiftStr) == 0) {
			shift = true;
		}
		else if (token.compare(wtwm::capsStr) == 0) {
			capsLock = true;
		}
		else if (token.compare(wtwm::ctrlStr) == 0) {
			ctrl = true;
		}
		else if (token.compare(wtwm::altStr) == 0) {
			alt = true;
		}
		else if (token.compare(wtwm::winStr) == 0) {
			win = true;
		}
		else {
			key = wtwm::getVirtualKey(token);
			if (key <= 0) return false; // no virtual key mapping for this item
		}
	}
	if (key == 0) {
		return false; // no ascii binding associated with this line
	}
	return true;
}
short wtwm::getVirtualKey(const std::string& token) {
	if (token.length() == 1) {
		std::string temp = "A";
		temp[0] = toupper(token[0]);
		return wtwm::VK_Mapping[temp];
	}
	return wtwm::VK_Mapping[token];
}

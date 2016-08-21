#include "parse_functions.h"
#include "wtwm_functions.h"
#include "stdafx.h"
#include "helpers.h"
#include <sstream>
#include <iostream>
#include <unordered_map>

using namespace std;

extern unordered_map < string, std::function<void(vector<string>)> > wtwm_functions;

fn_ptr cmd_function(const string command) {
	// TODO: how to create a closure with the needed parameters?	
	auto ret = [=]()->void
	{
		system(command.c_str());
	};
	return ret;
}

fn_ptr wtwm_function(const string command) {
	// TODO: how to create a closure with the needed parameters and return it?
	stringstream ss_command(command);
	string command_base;
	ss_command >> command_base;
	bool does_not_exist = (wtwm_functions.find(command_base) == wtwm_functions.end());
	if (does_not_exist) {
		cout << "wtwm function \"" << command_base << "\" was not found." << endl;
		return nullptr;
	}
	auto fn = wtwm_functions[command_base];
	vector<string> arguments = {"arg1", "arg2", "arg3"};
	// TODO: use command base to look up the function
	auto ret = [=]()->void
	{
		fn(arguments);
	};
	ret();
	return ret;
}

fn_ptr get_function(const string command) {
	int length = command.length();
	if (starts_with(command, "wtwm::")) {
		return wtwm_function(command.substr(6, length - 6));
	}
	else if (starts_with(command, "cmd::")) {
		return cmd_function(command.substr(5, length - 5));
	}
	else {
		cout << "Function string must start with \"wtwm::\" or \"cmd::\". Given string: \""<< command << "\""<< endl;
		return nullptr;
	}
}

vector<pair<string, fn_ptr>> parseFunctions(vector<pair<string, string>>& keybindingPairs)
{
	vector<pair<string, fn_ptr>> functionPairs;
	for (auto p : keybindingPairs) {
		fn_ptr function = get_function(p.second);
		if (function == nullptr) return vector<pair<string, fn_ptr>>();
		functionPairs.push_back({ p.first, function });
	}
	return functionPairs;
}

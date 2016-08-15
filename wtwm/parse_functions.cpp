#include "parse_functions.h"
#include "stdafx.h"
#include "helpers.h"
#include <sstream>
#include <iostream>

using namespace std;

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
	// TODO: use command base to look up the function
	return nullptr;
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

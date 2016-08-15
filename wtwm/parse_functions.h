#pragma once
#include "stdafx.h"

// parse a function string specified in a config file.
// Decides if the function is specific to wtwm, or an external command
fn_ptr get_function(const std::string function_string);

// returns a function which runs a the 'command' string in cmd
fn_ptr cmd_function(const std::string command);

// returns a function which runs a wtwm specific command
fn_ptr wtwm_function(const std::string command);

std::vector<std::pair<std::string, fn_ptr>> parseFunctions(std::vector<std::pair<std::string, std::string>>& keybindingPairs);
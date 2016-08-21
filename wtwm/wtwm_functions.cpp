#include "stdafx.h"
#include "wtwm_functions.h"

#include <iostream>

using namespace std;

void temp_function(vector<string> arguments) {
    cout << "arguments: " << arguments.size() << endl;
    for (auto s : arguments) {
        cout << "\t- " << s << endl;
    }
}

unordered_map < string, std::function<void(vector<string>)> > wtwm_functions = {
    {"temp", temp_function}
};

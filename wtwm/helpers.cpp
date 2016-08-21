#include "stdafx.h"
#include "helpers.h"

using namespace std;

bool starts_with(const string& str, const string& substr) {
    if (str.length() < substr.length()) return false;
    for (int i = 0; i < substr.length(); ++i) {
        if (str[i] != substr[i]) return false;
    }
    return true;
}


#pragma once
#include <algorithm>
#include <string_view>
#include <vector>
using namespace std;
string_view strip(const string_view& str, vector<char> strip_chars = { ' ', ',', '{', '}', '"', ':' });

vector<string_view> split(const string_view& str, char delim);

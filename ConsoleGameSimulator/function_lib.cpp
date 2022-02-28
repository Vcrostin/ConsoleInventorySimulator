#include "function_lib.h"

string_view strip(const string_view& str, vector<char> strip_chars) {
    if (str.size() == 0) {
        return "";
    }
    auto it_f = str.begin();
    while (it_f != str.end() && count(strip_chars.begin(), strip_chars.end(), *it_f) > 0) {
        it_f++;
    }
    if (it_f == str.end()) {
        return "";
    }
    auto it_b = str.end();
    //to get value;
    it_b--;
    while (it_b != it_f && count(strip_chars.begin(), strip_chars.end(), *it_b) > 0) {
        it_b--;
    }
    // return correct range
    it_b++;
    return { it_f, it_b };
}

vector<string_view> split(const string_view& str, char delim)
{
    vector<string_view> tokens;

    auto start = str.begin(), end = find(start, str.end(), delim);
    while (start != str.end())
    {
        if (start != end)
            tokens.push_back(string_view(start, end));
        start = end != str.end() ? end + 1 : end;
        while (start != str.end() && *start == delim)
            start++;
        end = find(start, str.end(), delim);
    }

    return tokens;
}

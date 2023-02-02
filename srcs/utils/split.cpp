#include "header.hpp"
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string>	split(const std::string& str, const std::string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
        token = str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (token[token.size() -1] == '\r') token.pop_back();
        if (token.length() != 0) res.push_back(token);
    }
    token = str.substr(pos_start);
    if (token.length() != 0) res.push_back(token);
    return res;
}

#include "stdext.h"

#include <algorithm>
#include <cctype>

std::string stdext::trim(const std::string& str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }
    if (start == str.size()) {
        return "";
    }

    size_t end = str.size() - 1;
    while (end > start && std::isspace(static_cast<unsigned char>(str[end]))) {
        --end;
    }

    return str.substr(start, end - start + 1);
}

std::string stdext::tolowercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return str;
}

std::vector<std::string> stdext::split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    size_t start = 0, pos;
    while ((pos = str.find(delimiter, start)) != std::string::npos) {
        result.push_back(str.substr(start, pos - start));
        start = pos + 1;
    }
    result.push_back(str.substr(start));
    return result;
}

std::string stdext::replace_all(std::string str, const std::string& from, const std::string& to) {
    if (from.empty())
        return str;
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}
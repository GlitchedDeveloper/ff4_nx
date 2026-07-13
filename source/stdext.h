#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace stdext {
std::string trim(const std::string& str);
std::string tolowercase(std::string str);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string replace_all(std::string str, const std::string& from, const std::string& to);

inline bool iequals(const std::string& a, const std::string& b) {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i])))
            return false;
    }
    return true;
}

template <typename Iter>
Iter ifind(Iter first, Iter last, const std::string& value) {
    return std::find_if(first, last, [&](const auto& s) {
        return iequals(s, value);
    });
}
}
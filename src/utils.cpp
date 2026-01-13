#include <iostream>
#include <cstdio>
#include <string>
#include <memory>
#include <array>
#include <sstream>

#include "utils.hpp"

std::optional<std::string> exec(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;

    try {
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("[error] popen failed to open");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return result;
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return {};
    }
}

std::vector<std::string> split(const std::string& line, const char delim) {
    std::istringstream ss(line);
    std::string segment;
    std::vector<std::string> labels_vec;
    while (std::getline(ss, segment, delim)) {
        labels_vec.push_back(trim(segment));
    }

    return labels_vec;
}

std::string trim(const std::string& line) {
    const char* whitespace = " \t";
    size_t start = line.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return line;
    }

    size_t end = line.find_last_not_of(whitespace);
    return line.substr(start, end - start + 1);
}

bool vec_has_value(const std::vector<std::string>& src, const std::string& val) {
    for (const auto& el : src) {
        if (el == val) {
            return true;
        }
    }   
    return false;
}

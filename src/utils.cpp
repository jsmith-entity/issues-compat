#include <iostream>
#include <cstdio>
#include <string>
#include <memory>
#include <array>

#include "utils.h"

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

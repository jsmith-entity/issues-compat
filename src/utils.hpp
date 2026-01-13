#ifndef UTILS_HPP
#define UTILS_HPP

#include <optional>
#include <string>
#include <vector>

std::optional<std::string> exec(const std::string& cmd);
std::vector<std::string> split(const std::string& line, const char delim);
std::string trim(const std::string& line);
bool vec_has_value(const std::vector<std::string>& src, const std::string& val);

#endif

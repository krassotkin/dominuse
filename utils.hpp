/*
name: utils.hpp
description: Several utility functions for the dominuse project.
author: Alexander Krassotkin (krassotkin.com)
created: 2024-05-14T09:16:51Z
modified: 2024-05-14T09:17:09Z
*/

#pragma once
#ifndef DOMINUSE_UTILS_HPP
#define DOMINUSE_UTILS_HPP

#include <algorithm> // for std::remove_if
#include <iostream> // std::cout, std::endl
#include <string> // std::string

namespace dominuse {

bool isDomain(const std::string& domain) {
 if(domain.empty()) return false;
 if(domain.find_first_of("/\\:*?\"<>|") != std::string::npos) return false;
 if(domain.size() < 2) return false;
 if(domain[0] == '.') return false;
 if(domain[domain.size() - 1] == '.') return false;
 size_t firstPoint = domain.find('.');
 if(firstPoint == std::string::npos) return false;
 if(domain.size() - firstPoint < 2) return false;
 return true;
}

void trim(std::string& str) {
 str.erase(std::remove_if(str.begin(), str.end(), [](char c) {return std::isspace(c);}), str.end());
}

} // dominuse

#endif // #ifndef DOMINUSE_UTILS_HPP

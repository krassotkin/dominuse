/*
name: utils.hpp
description: Several utility functions for the dominuse project.
author: Alexander Krassotkin (krassotkin.com)
version: 202406171230
created: 2024-05-14T09:16:51Z
modified: 2024-06-17T09:30:31Z
*/

#pragma once
#ifndef DOMINUSE_UTILS_HPP
#define DOMINUSE_UTILS_HPP

#include <arpa/inet.h> // for inet_aton()

#include <algorithm> // for std::remove_if
#include <cstring> // for memset()
#include <iostream> // for std::cout, std::endl
#include <regex> // for std::regex, std::regex_match
#include <string> // for std::string

namespace dominuse {

std::regex domainRegex("^((?!-)[a-z0-9-]{1,63}\\.)+(xn--)?[a-z0-9-]{1,63}$", std::regex_constants::icase);

bool isDomain(const std::string& domain);
bool isIP(const std::string& ips);
void trim(std::string& str);

/**
 * @brief Checks if the given domain is valid.
 * @param domain The domain to check.
 * @return true if the domain is valid, false otherwise.
 * @note This function is not fully implemented and may not cover all possible cases.
 * @see RFC1034, RFC2821, RFC5321
 */
bool isDomain(const std::string& domain) {
 if(domain.size() < 2 || domain.size() > 255) return false;
 if(domain[0] == '.' || domain.back() == '-') return false;
 if(!std::regex_match(domain, domainRegex)) return false;
 if(isIP(domain)) return false;
 return true;
}

/**
 * @brief Checks if the given string is a valid IP address.
 * @param ips The string to check.
 * @return true if the string is a valid IP address, false otherwise.
 * @see inet_aton's IP forms
 */
bool isIP(const std::string& ips) {
 struct in_addr addr;
 memset(&addr, 0, sizeof(addr)); // Initialize to zero
 return inet_aton(ips.c_str(), &addr) != 0; // inet_aton() returns 1 if the string is a valid IP address, 0 otherwise
}

/**
 * @brief Trims the given string from both ends, removing any leading and trailing whitespace characters.
 * @param str The string to trim.
 */
void trim(std::string& str) {
 str.erase(std::remove_if(str.begin(), str.end(), [](char c) {return std::isspace(c);}), str.end());
}

} // dominuse

#endif // #ifndef DOMINUSE_UTILS_HPP

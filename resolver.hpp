/*
name: resolver.hpp
description: Resolve a domain name to an IP address.
author: Alexander Krassotkin (krassotkin.com)
created: 2024-05-11T16:10:00Z
modified: 2024-05-14T09:17:25Z
*/

#pragma once
#ifndef DOMAINUSE_RESOLVER_HPP
#define DOMAINUSE_RESOLVER_HPP

#include <arpa/inet.h> // inet_ntop
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>

namespace dominuse {

/**
 @brief Resolve a domain name to an IP address.
 @param domain the domain name to resolve.
 @return the IP address of the given domain name.
*/
std::string resolve(const std::string& domain) {
 struct addrinfo hints, *result;
 std::memset(&hints, 0, sizeof(struct addrinfo));
 hints.ai_family = AF_INET;
 hints.ai_socktype = SOCK_STREAM;
 hints.ai_flags = AI_CANONNAME;
 int status = getaddrinfo(domain.c_str(), nullptr, &hints, &result);
 if(status != 0) return "";
 void *addr;
 if(result->ai_family == AF_INET) {
  struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
  addr = &(ipv4->sin_addr);
 } else {
  struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)result->ai_addr;
  addr = &(ipv6->sin6_addr);
 }
 char ipstr[INET6_ADDRSTRLEN];
 inet_ntop(result->ai_family, addr, ipstr, sizeof ipstr);
 freeaddrinfo(result);
 return ipstr;
}

} // dominuse

#endif // #ifndef DOMAINUSE_RESOLVER_HPP

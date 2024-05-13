/*
name: domainuse.cpp
description: Dominuse (domain in use) is a command line tool for quickly checking domain usage.
compilation: 
g++ -O3 -Wall -Wextra dominuse.cpp -o dominuse
run:
./dominuse
*/

#include <algorithm> // for std::remove_if
#include <fstream> // std::ifstream, std::ofstream
#include <iostream> // std::cout, std::endl
#include <string> // std::string
#include <vector> // std::vector

#include "resolver.hpp" // resolve

const std::string aboutDomainuse = "dominuse (domain in use) is a command line tool that allows you to quickly check if a domain is unavailable for registration by checking its IP address.";
const std::string usageDomainuse = R"""(Usage: 
 dominuse [options] [<domain_name>]...

Options:

 Without domain name:
  -h, --help                 Show this help message and exit.
  -v, --version              Show version and exit.

 With domain name:
  --input <file>             Read domains from file (one domain per line).
  --ip                       Show IP address.
  --output <file>            Write results to file.
  --used                     Show only used domains.
  --non-domains              Show non domains.
  --not-used, --unused       Show only not used domains.

Examples:
 ./dominuse -h
 ./dominuse -v
 ./dominuse krassotkin.com
 ./dominuse --ip dominuse.com cheat-sheets.org krassotkin.com
 ./dominuse --input input_domain_list.txt --output output_domain_list.txt
)""";
const std::string versionDomainuse = "20240511.20240513";

std::string inputFileName;
bool isHelp = false;
bool isVersion = false;
bool isIP = false;
std::vector<std::string> domains;
std::string outputFileName;
bool showUsed = false;
bool showNotUsed = false;
bool showNonDomains = false;

int checkFileName(const std::string& fileName);
bool isDomain(const std::string& domain);
int loadFromFile();
void parseArgs(int argc, char *argv[]);
int saveToFile(const std::string& data);
void trim(std::string& str);

int checkFileName(const std::string& fileName) {
 if(fileName.empty()) {
  std::cout << "Error: empty file name" << std::endl;
  return 1;
 }
 if(fileName.find_first_of("/\\:*?\"<>|") != std::string::npos) {
  std::cout << "Error: invalid file name \"" << fileName << "\"." << std::endl;
  return 2;
 }
 return 0;
}

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

int loadFromFile() {
 const int checkFileStatus = checkFileName(inputFileName);
 if(checkFileStatus != 0) return checkFileStatus;
 std::ifstream inputFile(inputFileName);
 if(!inputFile.is_open()) {
  std::cout << "Error: can't open input file \"" << inputFileName << "\"." << std::endl;
  return 1;
 }
 std::string domain;
 while(std::getline(inputFile, domain)) {
  trim(domain);
  if(domain.empty()) continue;
  domains.push_back(domain);
 }
 inputFile.close();
 return 0;
}

void parseArgs(int argc, char *argv[]) {
 bool fromFile = false;
 bool toFile = false;
 for(int i = 1; i < argc; i++) {
  std::string arg = argv[i];
  if(fromFile) {
   inputFileName = arg;
   trim(inputFileName);
   if(inputFileName.empty()) std::cout << "Warning: empty input file name." << std::endl;
   fromFile = false;
   continue;
  } else if(toFile) {
   outputFileName = arg;
   trim(outputFileName);
   if(outputFileName.empty()) std::cout << "Warning: empty output file name." << std::endl;
   toFile = false;
   continue;
  }
  if(arg == "-h" || arg == "--help") {
   isHelp = true;
  } else if(arg == "--input") {
   fromFile = true;
  } else if(arg == "--non-domains") {
   showNonDomains = true;
  } else if(arg == "--not-used" || arg == "--unused") {
   showNotUsed = true;
  } else if(arg == "--output") {
   toFile = true;
  } else if(arg == "--used") {
   showUsed = true;
  } else if(arg == "-v" || arg == "--version") {
   isVersion = true;
  } else if(arg == "--ip") {
   isIP = true;
  } else {
   trim(arg);
   if(arg.empty()) continue;
   domains.push_back(arg);
  }
 } 
 if(!showNotUsed && !showUsed) {
  showUsed = true;
  showNotUsed = true;
 } 
}

int saveToFile(const std::string& data) {
 const int checkFileStatus = checkFileName(outputFileName);
 if(checkFileStatus != 0) return checkFileStatus;
 std::ofstream outputFile(outputFileName);
 if(!outputFile.is_open()) {
  std::cout << "Error: can't open output file \"" << outputFileName << "\"." << std::endl;
  return 1;
 }
 outputFile << data;
 outputFile.close();
 return 0;
}

void trim(std::string& str) {
 str.erase(std::remove_if(str.begin(), str.end(), [](char c) {return std::isspace(c);}), str.end());
}

int main(int argc, char *argv[]) {
 if(argc == 1) isHelp = true;
 else parseArgs(argc, argv);
 if(isHelp) {
  std::cout << std::endl << aboutDomainuse << std::endl << std::endl;
  std::cout << usageDomainuse << std::endl << std::endl;
  return 0;
 }
 if(isVersion) {
  std::cout << versionDomainuse << std::endl;
  return 0;
 }
 if(!inputFileName.empty()) loadFromFile();
 if(domains.empty()) {
  std::cout << "Error: no domains found." << std::endl;
  return 1;
 }
 std::string result;
 for(std::string domain : domains) {
  if(!isDomain(domain)) {
   if(showNonDomains) {
    std::string line = domain + " is not a domain";
    if(outputFileName.empty()) std::cout << line << std::endl;
    else result += line + "\n";
   }
   continue;
  }
  const std::string ip = resolve(domain);
  if(showUsed && !showNotUsed && ip.empty()) continue;
  if(showNotUsed && !showUsed && !ip.empty()) continue;
  std::string line = domain + " ";
  if(ip.empty()) {
   line += "is not used";
  } else {
  if(isIP) line += ip;
  else line += "is used";
  }
  if(outputFileName.empty()) std::cout << line << std::endl;
  else result += line + "\n";
 }
 if(!outputFileName.empty()) {
  const int st = saveToFile(result);
  if(st != 0) {
   std::cout << "Error (" << st << "): can't save file \"" << outputFileName << "\"." << std::endl;
   return st;
  }
 }
 return 0;
}

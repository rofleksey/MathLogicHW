#ifndef CPP_SOLUTION_UTIL_H
#define CPP_SOLUTION_UTIL_H

#include <iostream>
#include <fstream>
#include <utility>
#include <cctype>
#include <vector>
#include <algorithm>
#include <list>
#include <map>
#include <sstream>
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>

#define swag long long

inline void split(const std::string &s, const std::string & del, std::vector<std::string> & elems) {
  	std::size_t current, previous = 0;
    current = s.find(del);
    while (current != std::string::npos) {
        elems.push_back(s.substr(previous, current - previous));
        previous = current + del.length();
        current = s.find(del, previous);
    }
    elems.push_back(s.substr(previous, current - previous));
}



#endif

#ifndef  _STRING_UTIL_H_
#define  _STRING_UTIL_H_

#include <string>
#include <vector>

std::string Trim(std::string s) {
	if (s.empty()) {
		return s;
	}

	s.erase(0, s.find_first_not_of(" \t\n"));
	s.erase(s.find_last_not_of(" \t\n") + 1);
	return s;
}

static void Split(const std::string& s, const std::string& delim, std::vector<std::string>* ret) {
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos) {
		ret->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0) {
		ret->push_back(s.substr(last, index - last));
	}
}

#endif  //_STRING_UTIL_H_

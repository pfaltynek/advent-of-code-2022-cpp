#ifndef _STRING_UTILS_HPP_
#define _STRING_UTILS_HPP_

#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string to_split, const std::string& delimiter) {
	std::vector<std::string> result;
	size_t pos;

	result.clear();

	pos = to_split.find(delimiter, 0);
	while (pos != std::string::npos) {
		if (pos) {
			result.push_back(to_split.substr(0, pos));
		}
		to_split = to_split.substr(pos + delimiter.size(), std::string::npos);

		pos = to_split.find(delimiter, 0);
	}

	if (to_split.size()) {
		result.push_back(to_split);
	}

	return result;
}

std::string join(const std::vector<std::string> to_join, const std::string delimiter) {
	std::stringstream ss;

	ss.clear();

	if (to_join.size()) {
		ss << to_join[0];
	}

	for (size_t i = 1; i < to_join.size(); i++) {
		ss << delimiter << to_join[i];
	}

	return ss.str();
}

#endif // _STRING_UTILS_HPP_

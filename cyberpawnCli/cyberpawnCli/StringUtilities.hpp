#ifndef CYBERPAWN_CYBERPAWN_CLI_STRING_UTILITIES_H_INCLUDED
#define CYBERPAWN_CYBERPAWN_CLI_STRING_UTILITIES_H_INCLUDED

#include <vector>
#include <string>

namespace string_utilities {
	inline std::vector<std::string> split(const std::string & s, char separator) {
		std::vector<std::string> ret;
		std::string currentSegment = "";

		for (int i = 0; i < s.size(); i++) {
			if (s[i] == separator) {
				ret.push_back(currentSegment);
				currentSegment = "";
			}
			else {
				currentSegment.push_back(s[i]);
			}
		}

		ret.push_back(currentSegment);
		return ret;
	}

	inline std::string toLowerCase(const std::string & s) {
		std::string ret = s;
		for (char & c : ret) {
			if ('A' <= c && c <= 'Z') {
				c += ('a' - 'A');
			}
		}
		return ret;
	}
}


#endif // CYBERPAWN_CYBERPAWN_CLI_STRING_UTILITIES_H_INCLUDED

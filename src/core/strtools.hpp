
#ifndef DEF_CORE_STRTOOLS
#define DEF_CORE_STRTOOLS

#include <vector>
#include <string>
#include <sstream>

namespace core
{
	std::vector<std::string> cutByChar(const std::string& src, char del);

	template<typename T>
		std::string toString(const T& value)
		{
			std::istringstream oss(value);
			return oss.str();
		}

	template<typename T>
		bool fromString(const std::string& str, T& dest)
		{
			std::istringstream iss(str);
			return iss >> dest != 0;
		}
};//namespace core

#endif//DEF_CORE_STRTOOLS


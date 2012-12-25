
#include "strtools.hpp"

#include <sstream>

namespace core
{
	std::vector<std::string> cutByChar(const std::string& src, char del)
	{
		std::istringstream iss(src);
		std::vector<std::string> parts;

		std::string part;
		while( std::getline(iss, part, del) )
		{
			if( !part.empty() )
				parts.push_back(part);
		}

		return parts;
	}
};//namespace core


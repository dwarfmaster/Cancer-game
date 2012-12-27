
#include "sanecell.hpp"

#include <sstream>
#include "core/exception.hpp"
#include "i18n.hpp"

size_t Cell::m_cellCount = 0;

Cell::Cell()
{
	++m_cellCount;
}

Cell::~Cell()
{
	--m_cellCount;
}

size_t Cell::nbCells()
{
	return m_cellCount;
}

Cell* Cell::load(const std::string& src)
{
	size_t pos = src.find_first_of(':');

	if(pos == std::string::npos)
	{
		std::ostringstream oss;
		oss << _i("Error while parsing a cell description \"") << src << _i("\" : there is no ':' separation");
		throw core::Exception( oss.str() );
	}

	std::string type = src.substr(0, pos - 1);
	std::string arg = src.substr(pos);
	if(type == "muted")
	{
		// TODO : return MutedCell::load(arg);
	}
	else if(type == "sane")
	{
		return SaneCell::load(arg);
	}
	else
	{
		std::ostringstream oss;
		oss << _i("Error while parsing a cell description : type\"") << type << _i("\" is invalid");
		throw core::Exception( oss.str() );
	}

	return NULL; // Ne doit pas arriver, pour éviter les warnings
}



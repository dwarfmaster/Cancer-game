
#include "cell.hpp"

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
	// TODO
}



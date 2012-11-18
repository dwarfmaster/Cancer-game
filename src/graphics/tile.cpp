
#include "tile.hpp"

namespace graphics
{
	Tile::~Tile()
	{
	}

	unsigned int Tile::getXidx() const
	{
		return m_Xidx;
	}

	unsigned int Tile::getYidx() const
	{
		return m_Yidx;
	}
			
	HexaMap* Tile::getHexaMap() const
	{
		return m_father;
	}
};



#include "hexamap.hpp"

#include <SDL/SDL.h>
#include "tile.hpp"

#include <boost/filesystem/path.hpp>
#include <cmath>
#define PI 3.14159265

HexaMap::HexaMap(unsigned int tileSize)
	: m_size(0,0), m_ori(0,0), m_tileSize(tileSize), m_s(m_tileSize / std::cos(30.0*PI/180.0) / 2)
{
}

HexaMap::HexaMap(const boost::filesystem::path& src, unsigned int tileSize)
	: m_size(0,0), m_ori(0,0), m_tileSize(tileSize), m_s(m_tileSize / std::cos(30.0*PI/180.0) / 2)
{
	load(src);
}

HexaMap::~HexaMap()
{
	clear();
}

void HexaMap::load(const boost::filesystem::path& src)
{
	clear();
	// TODO
}

void HexaMap::save(const boost::filesystem::path& dest) const
{
	// TODO
}

void HexaMap::clear()
{
	for(size_t i = 0; i < m_size.x; ++i)
	{
		for(size_t j = 0; j < m_size.y; ++j)
		{
			if( m_map[i][j] != NULL )
				delete m_map[i][j];
		}
	}

	m_map.resize( boost::extents[0][0] );
	m_size.set(0,0);
	m_ori.set(0,0);
}

void HexaMap::scroll(const sdl::Vector2f& dec)
{
	// TODO
}

Tile* HexaMap::getTileAt(unsigned int x, unsigned int y)
{
	if( x < m_size.x
			|| y < m_size.y )
		return m_map[x][y];
	else
		return NULL;
}

const Tile* HexaMap::getTileAt(unsigned int x, unsigned int y) const
{
	if( x < m_size.x
			|| y < m_size.y )
		return m_map[x][y];
	else
		return NULL;
}

Tile* HexaMap::getTileAtPos(const sdl::Pointui& pos)
{
	sdl::Pointui rpos = pos + sdl::Vector2f(m_ori);
	long int x = rpos.x / m_s / 1.5;
	long int y = (y - (x%2) * m_tileSize / 2) / m_tileSize;

	if( y < 0
			|| x < 0 )
		return NULL;
	else
		return getTileAt( (unsigned int)x, (unsigned int)y );
}

const Tile* HexaMap::getTileAtPos(const sdl::Pointui& pos) const
{
	sdl::Pointui rpos = pos + sdl::Vector2f(m_ori);
	long int x = rpos.x / m_s / 1.5;
	long int y = (y - (x%2) * m_tileSize / 2) / m_tileSize;

	if( y < 0
			|| x < 0 )
		return NULL;
	else
		return getTileAt( (unsigned int)x, (unsigned int)y );
}

SDL_Surface* HexaMap::get(const sdl::AABB& size) const
{
	// TODO
}

void HexaMap::setSize(const sdl::AABB& size)
{
	m_pictSize = size;
}

SDL_Surface* HexaMap::get() const
{
	return get(m_pictSize);
}

HexaMap::operator SDL_Surface*() const
{
	return get(m_pictSize);
}


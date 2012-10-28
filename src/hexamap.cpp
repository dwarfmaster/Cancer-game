
#include "hexamap.hpp"

#include <SDL/SDL.h>
#include "tile.hpp"

#include <boost/filesystem/path.hpp>

HexaMap::HexaMap()
{
}

HexaMap::HexaMap(const boost::filesystem::path& src)
{
}

HexaMap::~HexaMap()
{
}

void HexaMap::save(const boost::filesystem::path& dest) const
{
}

void HexaMap::free()
{
}

void HexaMap::scroll(const sdl::Vector2f& dec)
{
}

Tile* HexaMap::getTileAt(unsigned int x, unsigned int y)
{
}

const Tile* HexaMap::getTileAt(unsigned int x, unsigned int y) const
{
}

Tile* HexaMap::getTileAtPos(const sdl::Pointui& pos)
{
}

const Tile* HexaMap::getTileAtPos(const sdl::Pointui& pos) const
{
}

SDL_Surface* HexaMap::get(const sdl::AABB& size) const
{
}

void HexaMap::setSize(const sdl::AABB& size)
{
}

SDL_Surface* HexaMap::get() const
{
}

HexaMap::operator SDL_Surface*() const
{
}


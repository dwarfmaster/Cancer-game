
#ifndef DEF_CELL
#define DEF_CELL

#include "graphics/tile.hpp"

#include <list>
#include <string>
struct SDL_Surface;

class Cell : public graphics::Tile
{
	public:
		Cell();
		virtual ~Cell();

		static size_t nbCells();

		virtual SDL_Surface* getImg() const =0;
		virtual std::string save() const =0;

		static Cell* load(const std::string& src);

	private:
		static size_t m_cellCount;
};//class Cell

#endif//DEF_CELL


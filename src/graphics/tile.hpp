
#ifndef DEF_TILE
#define DEF_TILE

#include <string>
#include <boost/function.hpp>
struct SDL_Surface;

namespace graphics
{
	class Tile
	{
		public:
			virtual SDL_Surface* getImg() const =0;
			virtual std::string save() const =0;

			virtual ~Tile();

	};//class Tile
};//namespace graphics

#endif//DEF_TILE


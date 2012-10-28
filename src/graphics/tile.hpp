
#ifndef DEF_TILE
#define DEF_TILE

#include <string>
struct SDL_Surface;

namespace graphics
{
	class Tile
	{
		public:
			virtual SDL_Surface* getImg() const =0;
			virtual std::string save() const =0;
			virtual void load(const std::string& src) =0; // Lance une exception en cas d'erreur

	};//class Tile
};//namespace graphics

#endif//DEF_TILE

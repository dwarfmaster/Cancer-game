
#ifndef DEF_TILE
#define DEF_TILE

#include <string>
#include <boost/function.hpp>
struct SDL_Surface;
class HexaMap;

namespace graphics
{
	class Tile
	{
		public:
			virtual SDL_Surface* getImg() const =0;
			virtual std::string save() const =0;

			virtual ~Tile();

			unsigned int getXidx() const;
			unsigned int getYidx() const;
			HexaMap* getHexaMap() const;

		private:
			friend HexaMap;

			unsigned int m_Xidx;
			unsigned int m_Yidx;
			HexaMap* m_father;
	};//class Tile
};//namespace graphics

#endif//DEF_TILE


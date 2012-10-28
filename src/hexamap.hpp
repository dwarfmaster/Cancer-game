
#ifndef DEF_HEXAMAP
#define DEF_HEXAMAP

#include <SDLP_position.hpp>

namespace boost
{
	namespace filesystem
	{
		class path;
	};
};
class Tile;
struct SDL_Surface;

class HexaMap
{
	public:
		HexaMap();
		HexaMap(const boost::filesystem::path& src);
		~HexaMap();

		void save(const boost::filesystem::path& dest) const; // Lance une exception en cas d'erreur
		void free();
		void scroll(const sdl::Vector2f& dec);

		// Retournent NULL si la case pointée est vide.
		Tile* getTileAt(unsigned int x, unsigned int y); // En nombre de tiles
		const Tile* getTileAt(unsigned int x, unsigned int y) const;
		Tile* getTileAtPos(const sdl::Pointui& pos); // En pixels, tient compte du scroll
		const Tile* getTileAtPos(const sdl::Pointui& pos) const;

		SDL_Surface* get(const sdl::AABB& size) const;
		void setSize(const sdl::AABB& size);
		SDL_Surface* get() const; // Utilise la taille enregistrée
		operator SDL_Surface*() const;

	private:
};//class HexaMap

#endif//DEF_HEXAMAP


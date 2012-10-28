
#ifndef DEF_HEXAMAP
#define DEF_HEXAMAP

#include <SDLP_position.hpp>
#include <vector>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>

namespace boost
{
	namespace filesystem
	{
		class path;
	};
};
struct SDL_Surface;

namespace graphics
{
	class Tile;

	class HexaMap
	{
		public:
			HexaMap(unsigned int tileSize);
			HexaMap(const boost::filesystem::path& src, unsigned int tileSize);
			~HexaMap();

			void load(const boost::filesystem::path& src); // Lance une exception en cas d'erreur
			void save(const boost::filesystem::path& dest) const; // Idem
			void clear();
			// Se bloque sur les bords
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
			typedef boost::multi_array<Tile*,2> array_t;
			typedef boost::multi_array_types::index_range vue_t;
			typedef sdl::Point<size_t> array_size_t;

			array_t m_map;
			array_size_t m_size;

			sdl::Pointui m_ori; // Origine du scroll
			sdl::AABB m_pictSize;

			const unsigned int m_tileSize;
			const unsigned int m_s; // Constante utilisée pour les calculs

			HexaMap();
	};//class HexaMap
};//namespace graphics

#endif//DEF_HEXAMAP


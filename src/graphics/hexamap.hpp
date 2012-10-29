
#ifndef DEF_HEXAMAP
#define DEF_HEXAMAP

#include <SDLP_position.hpp>
#include <vector>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/array.hpp>

namespace boost
{
	namespace filesystem
	{
		class path;
	};
};
struct SDL_Surface;
class TiXmlElement;

namespace graphics
{
	class Tile;

	class HexaMap
	{
		public:
			typedef boost::function<Tile* (const std::string&)> load_tile_f;
			enum Around{top, bottom, top_left, top_right, bottom_left, bottom_right};
			typedef boost::array<Tile*, 6> near_t;
			typedef boost::array<const Tile*, 6> const_near_t;

			HexaMap(unsigned int height);
			HexaMap(const boost::filesystem::path& src, const load_tile_f& loader, unsigned int height);
			~HexaMap();

			void load(const boost::filesystem::path& src, const load_tile_f& loader); // Lance une exception en cas d'erreur, loader est utilisé pour créer les tiles
			void save(const boost::filesystem::path& dest) const; // Idem
			void clear();
			// Se bloque sur les bords
			void scroll(const sdl::Vector2f& dec);

			// Retournent NULL si la case pointée est vide.
			Tile* getTileAt(unsigned int x, unsigned int y); // En nombre de tiles
			boost::optional<sdl::Pointui> getTilePos(const Tile* tile) const; // Retourne boost::none si rien n'a été trouvé
			const Tile* getTileAt(unsigned int x, unsigned int y) const;
			Tile* getTileAtPos(const sdl::Pointui& pos); // En pixels, tient compte du scroll
			const Tile* getTileAtPos(const sdl::Pointui& pos) const;
			near_t getAroundTiles(const Tile* tile);
			const_near_t getAroundTiles(const Tile* tile) const;

			void select(const Tile* t);
			void unselect();

			SDL_Surface* get(const sdl::AABB& size) const;
			void setSize(const sdl::AABB& size);
			SDL_Surface* get() const; // Utilise la taille enregistrée
			operator SDL_Surface*() const;

			sdl::AABB totalSize() const;

		private:
			typedef boost::multi_array<Tile*,2> array_t;
			typedef boost::multi_array_types::index_range vue_t;
			typedef sdl::Point<size_t> array_size_t;

			array_t m_map;
			array_size_t m_size;

			sdl::Pointui m_ori; // Origine du scroll
			sdl::AABB m_pictSize;

			const unsigned int m_height;
			const unsigned int m_width;
			const unsigned int m_s; // Constante utilisée pour les calculs

			boost::optional<sdl::Pointui> m_selected; // boost::none = pas de sélection

			typedef std::vector<Tile*> row_t;
			row_t parseRow(const TiXmlElement* row, const load_tile_f& loader, size_t size);

			SDL_Surface* m_hexa;
			void createHexa();

			HexaMap();
	};//class HexaMap
};//namespace graphics

#endif//DEF_HEXAMAP


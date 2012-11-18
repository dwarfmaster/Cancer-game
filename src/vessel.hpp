
#ifndef DEF_VESSEL
#define DEF_VESSEL

#include "graphics/tile.hpp"
#include <boost/array.hpp>
#include <vector>
#include <list>
#include <SDL.h>
class Cell;

class Vessel;
void update(Vessel* ves); // Met à jour tous les vessels

class Vessel : public graphics::Tile
{
	friend void update(Vessel* ves);

	public:
		Vessel();
		virtual ~Vessel();

		virtual SDL_Surface* getImg() const;
		virtual std::string save() const;

		void addMed();

		static Vessel* load(const std::string& src);

	private:
		std::vector<Vessel*> m_next; // Les cases de vaisseau vers lesquelles coule cette case
		std::vector<Vessel*> m_prev; // L'inverse
		int m_dir; // La direction, voir enum ci-dessous
		std::list<Uint32> m_meds; // Le nombre de médiator, avec le temps écoulé depuis leur arrivée sur le vessel

		void selfUpdate();
		void selfLoad(); // Va mettre à jour cette case après le chargement de la map

		enum{TOP, BOTTOM, TOP_LEFT, BOTTOM_LEFT, TOP_RIGHT, BOTTOM_RIGHT, ALL_DIR}; // Les directions dans lesquelles peut couler la case
		static boost::array<SDL_Surface*, ALL_DIR> m_imgs; // Les images
};//class Vessel

#endif//DEF_VESSEL


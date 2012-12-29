
#ifndef DEF_MUTEDCELL
#define DEF_MUTEDCELL

#include <list>
#include <SDL/SDL.h>
#include "cell.hpp"
#include "sanecell.hpp"

class MutedCell;
void update(MutedCell* cell);

class MutedCell : public Cell
{
	friend void update(MutedCell* cell);

	public:
		MutedCell();
		virtual ~MutedCell();

		virtual SDL_Surface* getImg() const;
		virtual std::string save() const;

		static size_t nbMuted();
		static MutedCell* load(const std::string& src);

		size_t attackers() const; // Nombre d'attackers
		void createAttacker(SaneCell* dest);

		static void addMuted(MutedCell* nv);

	private:
		Uint32 m_lastTime;

		// Les attackers
		typedef std::list<SaneCell*> listCell;
		typedef listCell::iterator cells_it;
		typedef listCell::const_iterator cells_cit;
		listCell m_attackers; // Les attackers à créer
		Uint32 m_atTimeSpend; // Temps passé depuis le début de la création

		// Les mediators
		struct AMediator // Structure servant à stocker les mediator avant leur création
		{
			enum dest_t{CELL,VESSEL}; // Le type de la destination
			dest_t dest;

			graphics::Tile* tdest; // La destination, sera castée en fonction de son type
		};
		typedef std::list<AMediator> listMed;
		typedef listMed::iterator meds_it;
		typedef listMed::const_iterator meds_cit;
		listMed m_mediators;
		Uint32 m_medTimeSpend; // Temps passé depuis la création

		void selfUpdate();

		static SDL_Surface* m_img;

		// Le compteur
		static size_t m_nb;
		typedef std::list<MutedCell*> all_t;
		typedef all_t::iterator all_it;
		typedef all_t::const_iterator all_cit;
		all_t m_all;
		all_it m_selfPos;
};//MutedCell

#endif//DEF_MUTEDCELL


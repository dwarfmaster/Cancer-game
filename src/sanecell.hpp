
#ifndef DEF_SANECELL
#define DEF_SANECELL

#include <list>
#include "cell.hpp"
#include "mutedcell.hpp"
#include "attacker.hpp"

struct SDL_Surface;

void update(SaneCell* cell);

class SaneCell : public Cell
{
	friend void update(SaneCell* cell);
	friend Attacker;
	// friend mediator

	public:
		SaneCell();
		virtual ~SaneCell();

		size_t nbMediators();
		size_t nbAttackers();

		virtual SDL_Surface* getImg() const;
		virtual std::string save() const;

		static size_t nbSane();
		static SaneCell* load(const std::string& src);
		static MutedCell* toMuted(SaneCell* cell);

	private:
		size_t m_nbMed; // Ces deux sont mis à jour par les mediator et attacker
		size_t m_nbAtt;

		static SDL_Surface* m_img;

		void selfUpdate();

		// Le compteur
		static size_t m_nb;
		typedef std::list<SaneCell*> all_t;
		typedef all_t::iterator all_it;
		typedef all_t::const_iterator all_cit;
		all_t m_all;
		all_it m_selfPos;
};//class SaneCell

#endif//DEF_SANECELL


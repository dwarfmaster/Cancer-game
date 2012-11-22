
#ifndef DEF_SANECELL
#define DEF_SANECELL

#include <list>
#include "cell.hpp"
#include "attacker.hpp"
#include "mediator.hpp"

struct SDL_Surface;
class MutedCell;

class SaneCell;
void update(SaneCell* cell);

class SaneCell : public Cell
{
	friend void update(SaneCell* cell);
	friend Attacker;
	friend Mediator;

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
		size_t m_nbMed; // Ces quatres sont mis à jour par les mediator et attacker
		std::list<Mediator*> m_meds;
		size_t m_nbAtt;
		std::list<Attacker*> m_atts;

		int m_def; // Défense restante
		int m_conv; // Niveau de conviction

		static SDL_Surface* m_img;

		void selfUpdate();

		// Le compteur
		static size_t m_nb;
		typedef std::list<SaneCell*> all_t;
		typedef all_t::iterator all_it;
		typedef all_t::const_iterator all_cit;
		static all_t m_all;
		all_it m_selfPos;
};//class SaneCell

#endif//DEF_SANECELL


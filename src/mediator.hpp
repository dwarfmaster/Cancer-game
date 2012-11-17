
#ifndef DEF_MEDIATOR
#define DEF_MEDIATOR

#include <list>
#include <map>
#include <string>

class SaneCell;
struct SDL_Surface;

class Mediator;
void update(Mediator* med); // Met à jour tous les mediators

class Mediator
{
	friend void update(Mediator* med);

	public:
		Mediator(SaneCell* dest);
		~Mediator();

	private:
		SaneCell* m_dest;

		void selfUpdate();
		void blit();

		typedef std::list<Mediator*> listMeds;
		typedef std::map<SaneCell*, listMeds> mapBySaneCell;
		typedef listMeds::iterator meds_it;
		typedef listMeds::const_iterator meds_cit;
		typedef mapBySaneCell::iterator all_it;
		typedef mapBySaneCell::const_iterator all_cit;
		
		static mapBySaneCell m_all;
		static size_t m_nb;
		static SDL_Surface* m_img;

		// On ne peut pas les utiliser
		Mediator();
		Mediator(const Mediator& cp);
		Mediator& operator=(const Mediator& cp);
};//class Mediator

#endif//DEF_MEDIATOR


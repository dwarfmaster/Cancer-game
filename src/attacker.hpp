
#ifndef DEF_ATTACKER
#define DEF_ATTACKER

#include <map>
#include <list>
#include <string>

class SaneCell;
struct SDL_Surface;

class Attacker;
void update(Attacker* at); // Met à jour tous les attackers

class Attacker
{
	friend void update(Attacker* at);

	public:
		Attacker(SaneCell* dest);
		~Attacker();

	private:
		SaneCell* m_dest;
		const size_t m_nbByImg = 5; // Le nombre d'attackers pour chaque image

		typedef std::list<Attacker*> listAttackers;
		typedef std::map<SaneCell*, listAttackers> mapBySaneCell;
		typedef listAttackers::iterator attackers_it;
		typedef listAttackers::const_iterator attackers_cit;
		typedef mapBySaneCell::iterator all_it;
		typedef mapBySaneCell::const_iterator all_cit;

		static size_t m_nb; // Le nombre d'attackers
		static SDL_Surface* m_img; // L'image à blitter
		static mapBySaneCell m_all; // Tous les attackers classés par destination

		void selfUpdate(); // Ne met à jour que cet attacker
		void blit(size_t nb); // nb = nombre d'attackers à blitter, maximum 6
		
		// On ne peut pas les utiliser
		Attacker();
		Attacker(const Attacker& cp);
		Attacker& operator=(const Attacker& cp);
};//class Attacker

#endif//DEF_ATTACKER


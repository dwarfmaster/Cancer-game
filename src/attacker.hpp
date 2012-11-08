
#ifndef DEF_ATTACKER
#define DEF_ATTACKER

#include <map>
#include <list>
#include <string>

class Cell; // TODO à coder les Cell
struct SDL_Surface;

class Attacker;
void update(Attacker* at); // Met à jour tous les attackers

class Attacker
{
	friend void update(Attacker* at);

	public:
		Attacker(Cell* dest);
		~Attacker();

	private:
		Cell* m_dest;
		const size_t m_nbByImg = 5; // Le nombre d'attackers pour chaque image

		typedef std::list<Attacker*> listAttackers;
		typedef std::map<Cell*, listAttackers> mapByCell;
		typedef listAttackers::iterator attackers_it;
		typedef listAttackers::const_iterator attackers_cit;
		typedef mapByCell::iterator all_it;
		typedef mapByCell::const_iterator all_cit;

		static size_t m_nb; // Le nombre de cellules
		static SDL_Surface* m_img; // L'image à blitter
		static mapByCell m_all; // Tous les attackers classés par destination

		void selfUpdate(); // Ne met à jour que cet attacker
		void blit(size_t nb); // nb = nombre d'attackers à blitter, maximum 6
		
		// On ne peut pas les utiliser
		Attacker();
		Attacker(const Attacker& cp);
		Attacker& operator=(const Attacker& cp);
};//class Attacker

#endif//DEF_ATTACKER


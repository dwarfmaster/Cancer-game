
#ifndef DEF_ATTACKER
#define DEF_ATTACKER

#include <map>
#include <list>
#include <string>

class SaneCell;
struct SDL_Surface;

class Attacker;

class Attacker
{
	friend void update(Attacker* at);

	public:
		Attacker(SaneCell* dest);
		~Attacker();

		std::string save() const;
		void load(const std::string& src);

		void selfUpdate(); // Ne met à jour que cet attacker, appelé par SaneCell.

	private:
		SaneCell* m_dest;
		const size_t m_nbByImg = 5; // Le nombre d'attackers pour chaque image
		unsigned int m_timeSpent; // Temps écoulé depuis création
		unsigned int m_lastTime;

		static size_t nb;
		static SDL_Surface* m_img; // L'image à blitter

		void blit(size_t nb); // nb = nombre d'attackers à blitter, maximum 6
		
		// On ne peut pas les utiliser
		Attacker();
		Attacker(const Attacker& cp);
		Attacker& operator=(const Attacker& cp);
};//class Attacker

#endif//DEF_ATTACKER


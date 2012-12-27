
#ifndef DEF_MEDIATOR
#define DEF_MEDIATOR

#include <list>
#include <map>
#include <string>

class SaneCell;
struct SDL_Surface;

class Mediator;

class Mediator
{
	public:
		Mediator(SaneCell* dest);
		~Mediator();

		std::string save() const;
		void load(const std::string& src);

		void selfUpdate(); // Appelé par sanecell

	private:
		SaneCell* m_dest;
		unsigned int m_timeSpent;
		unsigned int m_lastTime;

		static size_t m_nb;
		static SDL_Surface* m_img;

		// On ne peut pas les utiliser
		Mediator();
		Mediator(const Mediator& cp);
		Mediator& operator=(const Mediator& cp);
};//class Mediator

#endif//DEF_MEDIATOR



#include "attacker.hpp"

#include <SDL/SDL.h>
#include <sstream>

#include "i18n.hpp"
#include "core/exception.hpp"

#include "sanecell.hpp"

// Durée de survie : 45 secondes
const Uint32 timeLive = 45000;

size_t Attacker::nb = 0;
SDL_Surface* Attacker::m_img = NULL;

Attacker::Attacker(SaneCell* dest)
	: m_dest(dest), m_timeSpent(0)
{
	m_lastTime = SDL_GetTicks();

	if( nb == 0 )
	{
		// TODO charger l'image
	}
	++nb;
}

Attacker::~Attacker()
{
	--nb;
	if( nb == 0 )
		SDL_FreeSurface(m_img);
}

std::string Attacker::save() const
{
	// La sauvegarde ne contient que m_timeSpent
	std::ostringstream save;
	save << m_timeSpent;
	return save.str();
}

void Attacker::load(const std::string& src)
{
	std::istringstream iss(src);
	void* err = iss >> m_timeSpent;

	if( err == NULL
			|| !iss.eof() )
	{
		std::ostringstream oss;
		oss << _i("Error in the time spent of an attacker : \"") << src << _i("\"");
		throw core::Exception( oss.str() );
	}
}

void Attacker::selfUpdate()
{
	Uint32 timeLapsed = m_lastTime - SDL_GetTicks();

	m_timeSpent += timeLapsed;
	if( m_timeSpent >= timeLive )
	{
		for(std::list<Attacker*>::iterator it = m_dest->m_atts.begin(); it != m_dest->m_atts.end(); ++it)
		{
			if( *it == this )
			{
				m_dest->m_atts.erase(it);
				break;
			}
		}
	}

	// TODO libérer la mémoire
}



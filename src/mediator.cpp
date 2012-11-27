
#include "mediator.hpp"

#include <sstream>
#include <SDL/SDL.h>

size_t Mediator::m_nb = 0;
SDL_Surface* Mediator::m_img = NULL;

Mediator::Mediator(SaneCell* dest)
	: m_timeSpent(0);
{
	m_lastTime = SDL_GetTicks();

	if( m_nb == 0 )
	{
		// TODO charger l'image
	}
	++m_nb;
}

Mediator::~Mediator()
{
	--m_nb;
	if( m_nb == 0 )
		SDL_FreeSurface(m_img);
}

std::string Mediator::save() const
{
	// La sauvegarde ne contient que la temps écoulé
	std::ostringstream save;
	save << m_timeSpent;
	return save.str();
}

void Mediator::load(const std::string& src)
{
	std::istringstream iss(src);
	void* err = iss >> m_timeSpent;

	if( err == NULL
			|| !iss.eof() )
	{
		std::ostringstream oss;
		oss << _i("Error while loading the time spent of an mediator : \"") << src << _i("\"");
		throw core::Exception( oss.str() );
	}
}

void Mediator::selfUpdate()
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

	delete this;
}

void Mediator::blit()
{
	// TODO
}



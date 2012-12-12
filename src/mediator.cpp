
#include "mediator.hpp"

#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "sanecell.hpp"
#include "i18n.hpp"
#include "core/exception.hpp"
#include "core/config.hpp"

size_t Mediator::m_nb = 0;
SDL_Surface* Mediator::m_img = NULL;

// La durée de survie : 2 minutes
const Uint32 timeLive = 120000;

Mediator::Mediator(SaneCell* dest)
	: m_dest(dest), m_timeSpent(0)
{
	m_lastTime = SDL_GetTicks();

	if( m_nb == 0 )
	{
		std::string path = (core::cfg->gamedir() / "mediator.png").string();
		m_img = IMG_Load( path.c_str() );
		if( m_img == NULL )
		{
			std::ostringstream oss;
			oss << _i("Error while loading the picture \"") << path << _i("\" : ") << SDL_GetError();
			throw core::Exception( oss.str() );
		}
	}
	++m_nb;

	++m_dest->m_nbMed;
	m_dest->m_meds.push_back(this);
}

Mediator::~Mediator()
{
	--m_nb;
	if( m_nb == 0 )
		SDL_FreeSurface(m_img);

	for(std::list<Mediator*>::iterator it = m_dest->m_meds.begin(); it != m_dest->m_meds.end(); ++it)
	{
		if( *it == this )
		{
			m_dest->m_meds.erase(it);
			break;
		}
	}
	--m_dest->m_nbMed;
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
		delete this;
	}
}

void Mediator::blit()
{
	// TODO
}



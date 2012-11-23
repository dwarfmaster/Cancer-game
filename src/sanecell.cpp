
#include "sanecell.hpp"

#include <SDL.h>

size_t SaneCell::m_nb = 0;
all_t SaneCell::m_all;
SDL_Surface* SaneCell::m_img = NULL;

SaneCell::SaneCell()
	: m_def(100), m_conv(0)
{
	if( m_nb == 0 )
	{
		// TODO : chargement de m_img
	}

	++m_nb;
}

SaneCell::~SaneCell()
{
	--m_nb;
	if( m_nb == 0 )
		SDL_FreeSurface(m_img);
}

size_t SaneCell::nbMediators()
{
	return m_nbMed;
}

size_t SaneCell::nbAttackers()
{
	return m_nbAtt;
}

SDL_Surface* SaneCell::getImg() const
{
	return m_img;
}

std::string SaneCell::save() const
{
	// TODO
	// Syntaxe sauvegarde : def;conv;nb_med(amed/anothermed);nb_att(aatt/anotheratt)
}

size_t SaneCell::nbSane()
{
	return m_nb;
}

SaneCell* SaneCell::load(const std::string& src)
{
	// TODO
}

MutedCell* SaneCell::toMuted(SaneCell* cell)
{
	// TODO
}

void SaneCell::selfUpdate()
{
	// TODO
}

void update(SaneCell* cell)
{
	// TODO
}



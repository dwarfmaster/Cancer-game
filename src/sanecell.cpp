
#include "sanecell.hpp"

#include <SDL.h>
#include <sstream>

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
	// Syntaxe sauvegarde : def;conv;nb_med(amed/anothermed);nb_att(aatt/anotheratt)
	
	std::ostringstream save;
	save << m_def << ";" << m_conv << ";";
	save << m_nbMed << "(";
	for(std::list<Mediator*>::const_iterator it = m_meds.begin(); it != m_meds.end(); ++it)
		save << it->save() << "/";
	save << ");" << m_nbAtt << "(";
	for(std::list<Attacker*>::const_iterator it = m_atts.begin(); it != m_atts.end(); ++it)
		save << it->save() << "/";
	save << ")";

	return save.c_str();
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



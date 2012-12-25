
#include "sanecell.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>

#include "core/config.hpp"
#include "core/exception.hpp"
#include "i18n.hpp"
#include "core/strtools.hpp"

size_t SaneCell::m_nb = 0;
SaneCell::all_t SaneCell::m_all;
SDL_Surface* SaneCell::m_img = NULL;

	SaneCell::SaneCell()
: m_def(100), m_conv(0), m_lastTime(SDL_GetTicks())
{
	if( m_nb == 0 )
	{
		std::string path = (core::cfg->gamedir() / "sanecell.png").string();
		m_img = IMG_Load( path.c_str() );
		if( m_img == NULL )
		{
			std::ostringstream oss;
			oss << _i("Error while loading the picture \"") << path << _i("\" : ") << SDL_GetError();
			throw core::Exception( oss.str() );
		}
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
		save << (*it)->save() << "/";
	save << ");" << m_nbAtt << "(";
	for(std::list<Attacker*>::const_iterator it = m_atts.begin(); it != m_atts.end(); ++it)
		save << (*it)->save() << "/";
	save << ")";

	return save.str();
}

size_t SaneCell::nbSane()
{
	return m_nb;
}

	template<typename T>
void loadPart(const std::string& src, T& dest, const std::string& caption)
{
	if( !core::fromString(src, dest) )
	{
		std::ostringstream oss;
		oss << _i("Error while parsing ") << caption << _i(" of a sanecell save : \"") << src << _i("\"");
		throw core::Exception( oss.str() );
	}
}

	template<typename Dest, typename NbDest>
void loadComps(const std::string& src, NbDest& nb)
{
	size_t pos = src.find_first_of("(");
	if( pos == std::string::npos )
	{
		std::ostringstream oss;
		oss << _i("Invalid syntax in a sanecell save : \"") << src << _i("\"");
		throw core::Exception( oss.str() );
	}

	if( ! ); // TODO continue
}

SaneCell* SaneCell::load(const std::string& src)
{
	// TODO
	// Syntaxe sauvegarde : def;conv;nb_med(amed/anothermed);nb_att(aatt/anotheratt)
	std::vector<std::string> parts = core::cutByChar(src, ';');
	if( parts.size() != 4 )
	{
		std::ostringstream oss;
		oss << _i("Invalid number of parts in a sanecell save : \"") << src << _i("\"");
		throw core::Exception( oss.str() );
	}

	SaneCell* cell;
	loadPart(parts[0], cell->m_def, _i("defense"));
	loadPart(parts[1], cell->m_conv, _i("level of conviction"));
}

MutedCell* SaneCell::toMuted(SaneCell* cell)
{
	// TODO
}

void SaneCell::selfUpdate()
{
	// Mise à jour des médiators et attackers
	for(med_it it = m_meds.begin(); it != m_meds.end(); ++it)
		(*it)->selfUpdate();
	for(att_it it = m_atts.begin(); it != m_atts.end(); ++it)
		(*it)->selfUpdate();

	// TODO améliorer la précision
	Uint32 timeSpent = SDL_GetTicks() - m_lastTime;
	while(timeSpent >= 1000)
	{
		// Les attackers
		int vdefense = int(m_nbAtt / 2) * -1;
		int vaccept = 0;
		if(m_def > 50)
			vaccept = ((m_def - 50) / 25) * -1;
		else
			vaccept = (50 - m_def) / 25;

		// Les Mediator
		vaccept = int(m_nbMed / 4) * int((100 - m_def) / 100);
		vaccept = m_conv / 4;
		if(vaccept < 0)
			vaccept = 0;

		m_def += vdefense;
		m_conv += vaccept;

		// TODO conversion

		timeSpent -= 1000;
	}

	m_lastTime = SDL_GetTicks();
}

void update(SaneCell* cell)
{
	// TODO
}



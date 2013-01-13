
#include "sanecell.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>

#include "core/config.hpp"
#include "core/exception.hpp"
#include "i18n.hpp"
#include "core/strtools.hpp"

#include "mutedcell.hpp"

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
	// Syntaxe sauvegarde : def;conv;amed/anothermed;aatt/anotheratt

	std::ostringstream save;
	save << m_def << ";" << m_conv << ";";
	for(std::list<Mediator*>::const_iterator it = m_meds.begin(); it != m_meds.end(); ++it)
		save << (*it)->save() << "/";
	save << ";";
	for(std::list<Attacker*>::const_iterator it = m_atts.begin(); it != m_atts.end(); ++it)
		save << (*it)->save() << "/";

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

	template<typename Dest, typename List>
void loadComps(const std::string& src, List& dest, SaneCell* tthis)
{
	std::vector<std::string> dests = core::cutByChar(src, '/');
	if( dests.size() == 0 )
	{
		std::ostringstream oss;
		oss << _i("Invalid save of a sanecell : \"") << src << _i("\"");
		throw core::Exception( oss.str() );
	}

	dest.clear();
	for(size_t i = 0; i < dests.size(); ++i)
	{
		Dest* nv = new Dest(tthis); // nv s'ajoute tout seul à dest
		nv->load( dests[i] );
	}
}

SaneCell* SaneCell::load(const std::string& src)
{
	// Syntaxe sauvegarde : def;conv;amed/anothermed;aatt/anotheratt

	std::vector<std::string> parts = core::cutByChar(src, ';');
	if( parts.size() != 4 )
	{
		std::ostringstream oss;
		oss << _i("Invalid number of parts in a sanecell save : \"") << src << _i("\"");
		throw core::Exception( oss.str() );
	}

	SaneCell* cell = new SaneCell;
	loadPart(parts[0], cell->m_def, _i("defense"));
	loadPart(parts[1], cell->m_conv, _i("level of conviction"));

	loadComps<Mediator, std::list<Mediator*>>(parts[2], cell->m_meds, cell);
	cell->m_nbMed = cell->m_meds.size();
	loadComps<Attacker, std::list<Attacker*>>(parts[3], cell->m_atts, cell);
	cell->m_nbAtt = cell->m_atts.size();

	return cell;
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

		if( m_def <= 0
				|| m_conv >= 100 )
		{
			toMuted(this);
			return;
		}

		timeSpent -= 1000;
	}

	m_lastTime = SDL_GetTicks();
	m_lastTime -= timeSpent;
}

void update(SaneCell* cell)
{
	for(SaneCell::all_it it = cell->m_all.begin(); it != cell->m_all.end(); ++it)
		(*it)->selfUpdate();
}



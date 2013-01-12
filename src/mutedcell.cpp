
#include "mutedcell.hpp"

#include "core/config.hpp"
#include "core/exception.hpp"
#include "i18n.hpp"

#include <sstream>
#include <SDL/SDL_image.h>

size_t MutedCell::m_nb = 0;
SDL_Surface* MutedCell::m_img = NULL;

MutedCell::MutedCell()
{
	// TODO constructor
	
	if( m_nb == 0 )
	{
		std::string path = (core::cfg->gamedir() / "mutedcell.png").string();
		m_img = IMG_Load( path.c_str() );
		if( m_img == NULL )
		{
			std::ostringstream oss;
			oss << _i("Error while loading the picture \"") << path << _i("\" : ") << SDL_GetError();
			throw core::Exception( oss.str() );
		}
	}

	m_selfPos = m_all.insert(m_all.begin(), this);
	++m_nb;
}

MutedCell::~MutedCell()
{
	m_all.erase(m_selfPos);
	--m_nb;

	if( m_nb == 0 )
		SDL_FreeSurface(m_img);

	// TODO destructor
}

SDL_Surface* MutedCell::getImg() const
{
	return m_img;
}

std::string MutedCell::save() const
{
	// TODO suavegarder
}

size_t MutedCell::nbMuted()
{
	return m_nb;
}

MutedCell* MutedCell::load(const std::string& src)
{
	// TODO charger
}

size_t MutedCell::attackers() const
{
	// TODO le nombre d'attaquants en préparation
}

void MutedCell::createAttacker(SaneCell* dest)
{
	// TODO créer un attaquant
}

void MutedCell::addMuted(MutedCell* nv)
{
	// TODO ajouter un mutant
}

void MutedCell::selfUpdate()
{
	// TODO se mettre à jour
}

void update(MutedCell* cell)
{
	// TODO tout mettre à jour
}




#include "mutedcell.hpp"

size_t MutedCell::m_nb = 0;
SDL_Surface* MutedCell::m_img = NULL;

MutedCell::MutedCell()
{
	// TODO constructor
	
	m_selfPos = m_all.insert(m_all.begin(), this);
	++m_nb;
}

MutedCell::~MutedCell()
{
	m_all.erase(m_selfPos);
	--m_nb;
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



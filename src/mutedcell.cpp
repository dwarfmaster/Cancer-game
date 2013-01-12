
#include "mutedcell.hpp"

size_t MutedCell::m_nb = 0;
SDL_Surface* MutedCell::m_img = NULL;

MutedCell::MutedCell()
{
	// TODO constructor
}

MutedCell::~MutedCell()
{
	// TODO destructor
}

SDL_Surface* MutedCell::getImg() const
{
	// TODO retourner image
}

std::string MutedCell::save() const
{
	// TODO suavegarder
}

size_t MutedCell::nbMuted()
{
	// TODO nombre de mutedcell
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



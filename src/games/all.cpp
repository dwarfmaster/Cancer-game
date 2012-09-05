
#include "all.hpp"

#include "model.hpp"
// Include les jeux

#include <ctime>
#include <iostream>
#include <SDL/SDL.h>

namespace games
{
	All::All()
	{
		std::srand( std::time(NULL) );
		init();
	}

	All::~All()
	{
		clear();
	}

	void All::init()
	{
		// À compléter avec les jeux
	}

	void All::clear()
	{
		for(game_iterator it = m_games.begin(); it != m_games.end(); ++it)
			delete it->second;
		m_games.clear();
	}

	size_t All::nbGames() const
	{
		return m_games.size();
	}

	std::vector<std::string> All::listGames() const
	{
		std::vector<std::string> games;
		for(cgame_iterator it = m_games.begin(); it != m_games.end(); ++it)
			games.push_back(it->first);

		return games;
	}

	Model* All::playGame(const std::string& name)
	{
		if( m_games.find(name) != m_games.end() )
			return m_games[name];
		else
			return NULL;
	}

	std::string All::randomGame() const
	{
		std::vector<std::string> games = listGames();
		size_t id = rand() % games.size();

		return games[id];
	}

	Model* All::playRandomGame()
	{
		return playGame( randomGame() );
	}

	void All::setSize(const SDL_Rect& size)
	{
		for(game_iterator it = m_games.begin(); it != m_games.end(); ++it)
			it->second->resize(size);
	}

};//namespace games



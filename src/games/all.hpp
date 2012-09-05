
#ifndef DEF_GAMES_ALL
#define DEF_GAMES_ALL

#include <map>
#include <string>
#include <vector>
struct SDL_Rect;

namespace games
{
	class Model;

	class All
	{
		public:
			All();
			~All();

			void init();
			void clear();

			inline size_t nbGames() const;
			std::vector<std::string> listGames() const;
			Model* playGame(const std::string& name);
			std::string randomGame() const;
			inline Model* playRandomGame();

			void setSize(const SDL_Rect& size);

		private:
			std::map<std::string,Model*> m_games;

			typedef std::map<std::string,Model*>::iterator game_iterator;
			typedef std::map<std::string,Model*>::const_iterator cgame_iterator;

			All(const All&);
			All& operator=(const All&);
	};//class All
};//namespace games

#endif//DEF_GAMES_ALL

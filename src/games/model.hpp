
#ifndef DEF_GAMES_MODEL
#define DEF_GAMES_MODEL

struct SDL_Rect;
struct SDL_Surface;
struct SDL_keysym;

namespace sdl
{
	class Event;
	class Pointsi;
};

namespace games
{
	// Cette classe définit l'interface pour les mini-jeux
	class Model
	{
		public:
			Model();
			virtual ~Model();

			virtual void resize(const SDL_Rect&) =0; // Définit la taille de la 'fenêtre' du jeu
			virtual SDL_Surface* update() =0; // Appellée à chaque frame
			virtual SDL_Surface* getScreen() =0; // Sert à obtenir l'écran

			virtual void setEventReceiver(const sdl::Event* ev) =0; // Donne le capteur d'évènements
			virtual void pressMouse(const sdl::Pointsi& pos) =0; // Indique que la sourie a été pressée à cet endroit
			virtual void releaseMouse(const sdl::Pointsi& pos) =0; // Indique que la sourie a été relachée à cet endroit
			virtual void pressKey(const SDL_keysym& key) =0; // Indique qu'une touche a été pressée
			virtual void releaseKey(const SDL_keysym& key) =0; // Indique d=qu'une touche a été relachée

		private:
	};//class Model
};//namespace games

#endif//DEF_GAMES_MODEL



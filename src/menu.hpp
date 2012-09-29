
#ifndef DEF_MENU
#define DEF_MENU

struct SDL_Surface;
namespace graphics
{
	class Gui;
	class Theme;
	class Button;
};

class Menu
{
	public:
		Menu();
		void run(); // the game
		~Menu();

	private:
		graphics::Gui& m_gui;
		graphics::Theme& m_theme;
		void setGui(); // Place les éléments de la gui
		SDL_Surface* m_bg; // Background img
		void loadBG();
		SDL_Surface* ecran;

		void initGui();
		graphics::Button *load, *ngame, *opts;

		// Les callbacks
		void loadGame();
		void newGame();
		void editOpts();
};//class Menu

#endif//DEF_MENU

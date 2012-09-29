
#ifndef DEF_CONFIGEDITOR
#define DEF_CONFIGEDITOR

#include <boost/filesystem/path.hpp>
struct SDL_Surface;
namespace graphics
{
	class Gui;
	class CheckBox;
	class TextField;
	class Slider;
};

class ConfigEditor
{
	public:
		ConfigEditor(SDL_Surface* bg);
		void run();

	private:
		graphics::Gui& m_gui;
		void setGui();
		SDL_Surface* ecran;
		SDL_Surface* m_bg;
		bool m_quit;

		graphics::CheckBox* m_fullscreen;
		graphics::TextField* m_width;
		graphics::TextField* m_height;
		graphics::Slider* m_music;
		graphics::Slider* m_sound;

		boost::filesystem::path m_sounds;
		boost::filesystem::path m_gtheme;

		// callbacks
		void save();
		void cancel();
		void setSound(const boost::filesystem::path& path);
		void setGtheme(const boost::filesystem::path& path);

		// Interdit
		ConfigEditor();
};

#endif//DEF_CONFIGEDITOR

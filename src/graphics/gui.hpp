
#ifndef DEF_GRAPHICS_GUI
#define DEF_GRAPHICS_GUI

#include <map>
#include <string>
#include <vector>
#include <SDLP_position.hpp>

struct SDL_Surface;
struct TTF_Font;
namespace gcn
{
	class BasicContainer;
	class SDLInput;
	class SDLGraphics;
	class SDLImageLoader;
	class Gui;
	class Container;
	class ImageFont;
};
namespace boost
{
	namespace filesystem
	{
		class path;
	};
};

namespace graphics
{
	class DeleterContainer;
	class Theme;

	class Gui
	{
		public:
			Gui(const sdl::AABB& size, const boost::filesystem::path& patht);
			~Gui();
			sdl::AABB size() const;
			Theme* getTheme();
			const Theme* getTheme() const;

			bool addContainer(const std::string& name);
			bool deleteContainer(const std::string& name);
			std::vector<std::string> listContainers() const;

			bool setCurrent(const std::string& ncur) const; // Ne change pas m_current si ncur n'existe pas

			bool exists(const std::string& name) const;
			bool usable() const; // Indique si le conteneur courrant est utilisable

			gcn::Container* operator->(); // Accès direct au container courrant, lance une exception s'il n'est pas utilisable
			const gcn::Container* operator->() const;
			gcn::Container& operator*();
			const gcn::Container& operator*() const;

			gcn::Container* get(const std::string& name); // Renvoie NULL si le container n'existe pas
			const gcn::Container* get(const std::string& name) const;

			Gui& processEvent(const SDL_Event& ev);
			Gui& update();
			bool hasFocus(const sdl::Pointsi& posm) const;

		private:
			std::map<std::string, DeleterContainer*> m_contains;
			const sdl::AABB m_rect; // Position
			mutable std::string m_current;
			Theme* m_theme;

			typedef std::map<std::string, DeleterContainer*>::iterator cont_iterator;
			typedef std::map<std::string, DeleterContainer*>::const_iterator ccont_iterator;

			SDL_Surface* ecran;

			gcn::SDLInput* m_input;
			gcn::SDLGraphics* m_graphs;
			gcn::SDLImageLoader* m_loader;

			gcn::Gui* m_gui;
			gcn::Container* m_top;
			gcn::ImageFont* m_font;

			Gui();
			Gui(const Gui&); // On interdit l'usage
			Gui& operator=(const Gui&);
	};//class Gui
};//namespace graphics

#endif//DEF_GRAPHICS_GUI


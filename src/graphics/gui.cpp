
#include "gui.hpp"

#include <SDL/SDL.h>
#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include "../i18n.hpp"
#include "../core/exception.hpp"
#include <sstream>
#include "deleterContainer.hpp"

namespace graphics
{
	Gui* gui;

	Gui::Gui(const sdl::AABB& size, const boost::filesystem::path& patht)
		: m_rect(size), ecran(NULL),
		m_input(NULL), m_graphs(NULL), m_loader(NULL),
		m_gui(NULL), m_top(NULL), m_font(NULL)
	{
		ecran = SDL_GetVideoSurface();
		if( ecran == NULL )
		{
			std::ostringstream oss;
			oss << _i("Gui : Error while getting the actual rendering surface : \"") << SDL_GetError() << _i("\"");
			throw core::Exception( oss.str() );
		}

		m_loader = new gcn::SDLImageLoader;
		gcn::Image::setImageLoader(m_loader);
		m_graphs = new gcn::SDLGraphics;
		m_graphs->setTarget(ecran);
		m_input = new gcn::SDLInput;

		m_theme = new Theme(patht);
		if( !m_theme->isLoaded() )
		{
			std::ostringstream oss;
			// Est suivit d'un chemin
			oss << _i("Can't load graphics theme at \"") << patht.string() << _i("\"");
			throw core::Exception( oss.str() );
		}

		m_font = m_theme->m_fonts[Theme::normal];
		gcn::Widget::setGlobalFont(m_font);

		m_gui = new gcn::Gui;
		m_gui->setGraphics(m_graphs);
		m_gui->setInput(m_input);

		m_top = new gcn::Container;
		m_top->setDimension( gcn::Rectangle(0, 0, m_rect->w, m_rect->h) );
		m_top->setOpaque(false);
		m_gui->setTop(m_top);
	}
	
	Gui::~Gui()
	{
		delete m_top;
		delete m_gui;

		delete m_input;
		delete m_graphs;
		delete m_loader;

		for(cont_iterator it = m_contains.begin(); it != m_contains.end(); ++it)
			delete it->second;
	}

	sdl::AABB Gui::size() const
	{
		gcn::Rectangle rec = m_top->getDimension();
		return sdl::makeRect(0, 0, rec.width, rec.height);
	}

	Theme* Gui::getTheme()
	{
		return m_theme;
	}

	const Theme* Gui::getTheme() const
	{
		return m_theme;
	}

	bool Gui::addContainer(const std::string& name)
	{
		if( exists(name) )
			return false;

		m_contains[name] = new DeleterContainer;
		m_contains[name]->setOpaque(true);
		m_theme->applyInter( m_contains[name] );
		m_top->add( m_contains[name] );

		return true;
	}

	bool Gui::deleteContainer(const std::string& name)
	{
		if( !exists(name) )
			return false;

		delete m_contains[name];
		m_contains.erase(name);

		return true;
	}

	std::vector<std::string> Gui::listContainers() const
	{
		std::vector<std::string> ret;
		for(ccont_iterator it = m_contains.begin(); it != m_contains.end(); ++it)
			ret.push_back(it->first);

		return ret;
	}

	bool Gui::setCurrent(const std::string& ncur) const
	{
		if( exists(ncur) )
			m_current = ncur;
		else
			return false;
		return true;
	}

	bool Gui::exists(const std::string& name) const
	{
		return m_contains.find(name) != m_contains.end();
	}

	bool Gui::usable() const
	{
		return exists(m_current);
	}

	gcn::Container* Gui::operator->()
	{
		if( usable() )
			return m_contains[m_current];
		else
		{
			std::ostringstream oss;
			oss << _i("gui : Can't acces to container \"") << m_current << _i("\"");
			throw core::Exception( oss.str() );
		}
	}

	const gcn::Container* Gui::operator->() const
	{
		if( usable() )
			return m_contains.find(m_current)->second;
		else
		{
			std::ostringstream oss;
			oss << _i("gui : Can't acces to container \"") << m_current << _i("\"");
			throw core::Exception( oss.str() );
		}
	}

	gcn::Container& Gui::operator*()
	{
		if( usable() )
			return *m_contains[m_current];
		else
		{
			std::ostringstream oss;
			oss << _i("gui : Can't acces to container \"") << m_current << _i("\"");
			throw core::Exception( oss.str() );
		}
	}

	const gcn::Container& Gui::operator*() const
	{
		if( usable() )
			return *m_contains.find(m_current)->second;
		else
		{
			std::ostringstream oss;
			oss << _i("gui : Can't acces to container \"") << m_current << _i("\"");
			throw core::Exception( oss.str() );
		}
	}

	gcn::Container* Gui::get(const std::string& name)
	{
		if( !exists(name) )
			return NULL;
		else
			return m_contains[name];
	}

	const gcn::Container* Gui::get(const std::string& name) const
	{
		if( !exists(name) )
			return NULL;
		else
			return m_contains.find(name)->second;
	}

	Gui& Gui::processEvent(const SDL_Event& ev)
	{
		m_input->pushInput(ev);
		return *this;
	}

	Gui& Gui::update()
	{
		m_gui->logic();
		m_gui->draw();

		return *this;
	}

	bool Gui::hasFocus(const sdl::Pointsi& posm) const
	{
		for(ccont_iterator it = m_contains.begin(); it != m_contains.end(); ++it)
		{
			gcn::Rectangle r = it->second->getDimension();
			sdl::AABB rect = sdl::makeRect(0, 0, r.width, r.height);

			if( rect.contain(posm) )
				return true;
		}
		return false;
	}

	void Gui::setDefaultFont(const Theme::FontType& type)
	{
		m_font = m_theme->m_fonts[type];
		gcn::Widget::setGlobalFont(m_font);
	}
			
};//namespace graphics



#include "theme.hpp"

#include "../core/exception.hpp"
#include "../i18n.hpp"
#include "tabbedArea.hpp"
#include "textBox.hpp"
#include "fileexplorer.hpp"
#include "textField.hpp"
#include "list.hpp"
#include "button.hpp"
#include "checkBox.hpp"
#include <sstream>
#include <tinyxml.h>

namespace graphics
{
	Theme::Theme()
		: m_load(false)
	{}

	Theme::Theme(const boost::filesystem::path& path)
		: m_load(false)
	{
		load(path);
	}

	Theme::~Theme()
	{
		for(size_t i = 0; i < ALL_FONTS; ++i)
			delete m_fonts[i];
	}

	bool Theme::load(const boost::filesystem::path& path)
	{
		// On ouvre le document
		TiXmlDocument file(path.string());
		if( !file.LoadFile() )
			return false;

		// On récupère les principales couleurs
		std::map<std::string, gcn::Color> colors = parseColors(&file);

		// On récupère les thèmes
		std::map<std::string, ctheme> themes = parseThemes(&file, colors);
		if( themes.begin() == themes.end() )
			return false;

		// On charge les polices
		TiXmlElement* elem = file.FirstChildElement("fonts");
		if( elem == NULL )
			return false;
		if( !parseFonts(elem, path.branch_path()) )
			return false;

		// On fait les liens
		elem = file.FirstChildElement("gtheme");
		if( elem == NULL )
			return false;

		boost::array<bool, LAST> inits;
		for(size_t i = 0; i < inits.size(); ++i)
			inits[i] = false;

		elem = elem->FirstChildElement("link");
		while( elem != NULL )
		{
			if( elem->Attribute("part") != NULL )
			{
				std::string part = elem->Attribute("part");
				Types type;

				if(part == "default")
					type = DEFAULT;
				else if(part == "guicont")
					type = GUICONT;
				else if(part == "button")
					type = BUTTON;
				else if(part == "checkbox")
					type = CHECKBOX;
				else if(part == "radio")
					type = RADIO;
				else if(part == "container")
					type = CONTAINER;
				else if(part == "list")
					type = LIST;
				else if(part == "text")
					type = TEXT;
				else if(part == "label")
					type = LABEL;
				else if(part == "tab")
					type = TAB;
				else if(part == "scroll")
					type = SCROLL;
				else if(part == "slider")
					type = SLIDER;
				else
					type = LAST; // Erreur

				if( type != LAST 
						&& elem->Attribute("ctheme") != NULL )
				{
					std::string theme = elem->Attribute("ctheme");
					if( themes.find(theme) != themes.end() )
					{
						m_links[type] = themes[theme];
						inits[type] = true;
					}
				}
			}

			elem = elem->NextSiblingElement("link");
		}

		if( !inits[DEFAULT] )
			return false;
		for(size_t i = 0; i < inits.size(); ++i)
		{
			if( !inits[i] )
				m_links[i] = m_links[DEFAULT];
		}

		m_load = true;
		return true;
	}

	bool Theme::isLoaded() const
	{
		return m_load;
	}

	Theme::operator bool() const
	{
		return m_load;
	}

	bool Theme::operator!() const
	{
		return !m_load;
	}

	void Theme::apply(graphics::TabbedArea* w)
	{
		apply(w, TAB);

		for(size_t i = 0; i < w->mTabs.size(); ++i)
			apply(w->mTabs[i].first, TAB);

		apply(w->mTabContainer, TAB);
		apply(w->mWidgetContainer, TAB);
	}

	void Theme::apply(graphics::TextBox* w)
	{
		apply(w, TEXT);
		apply(w->m_text, TEXT);
	}

	void Theme::apply(graphics::FileExplorer* w)
	{
		apply(w, CONTAINER);
		apply(w->m_field);
		apply(w->m_dirs);
		if( !w->m_dir )
			apply(w->m_files);

		apply(w->m_valid);
		apply(w->m_enter);
		apply(w->m_up);
		apply(w->m_call);
		apply(w->m_scrldir);
		if( !w->m_dir )
			apply(w->m_scrlfile);
	}

	void Theme::apply(gcn::Widget* w, const Types& type)
	{
		if( !m_load )
		{
			std::ostringstream oss;
			oss << _i("Trying to use a no loaded graphics theme.");
			throw core::Exception( oss.str() );
		}

		ctheme t = m_links[type];
		w->setForegroundColor(t.fg);
		w->setBackgroundColor(t.bg);
		w->setSelectionColor(t.sl);
		w->setBaseColor(t.bs);
	}

	std::map<std::string, gcn::Color> Theme::parseColors(TiXmlDocument* file)
	{
		std::map<std::string, gcn::Color> colors;

		TiXmlElement* elem = file->FirstChildElement("defcolor");
		while( elem != NULL )
		{
			if( elem->Attribute("name") != NULL )
			{
				std::string name = elem->Attribute("name");
				colors[name] = parseColor(elem);
			}

			elem = elem->NextSiblingElement("defcolor");
		}

		return colors;
	}

	gcn::Color Theme::parseColor(TiXmlElement* elem)
	{
		gcn::Color color;

		if( elem->Attribute("r") != NULL )
		{
			std::istringstream iss( elem->Attribute("r") );
			iss >> color.r;
		}
		else
			color.r = 0;

		if( elem->Attribute("g") != NULL )
		{
			std::istringstream iss( elem->Attribute("g") );
			iss >> color.g;
		}
		else
			color.g = 0;

		if( elem->Attribute("b") != NULL )
		{
			std::istringstream iss( elem->Attribute("b") );
			iss >> color.b;
		}
		else
			color.b = 0;

		if( elem->Attribute("a") != NULL )
		{
			std::istringstream iss( elem->Attribute("a") );
			iss >> color.a;
		}
		else
			color.a = 255;

		return color;

	}

	std::map<std::string, Theme::ctheme> Theme::parseThemes(TiXmlDocument* file, const std::map<std::string,gcn::Color>& colors)
	{
		std::map<std::string, ctheme> themes;

		TiXmlElement* elem = file->FirstChildElement("ctheme");
		while( elem != NULL )
		{
			if( elem->Attribute("name") != NULL )
			{
				std::string name = elem->Attribute("name");
				ctheme theme;
				bool fg = false, bg = false, sl = false, bs = false;

				TiXmlElement* color = elem->FirstChildElement("color");
				while( color != NULL )
				{
					if( color->Attribute("part") != NULL )
					{
						std::string part = color->Attribute("part");

						gcn::Color pcolor;
						if( color->Attribute("name") == NULL )
							pcolor = parseColor(color);
						else
							pcolor = colors.find( color->Attribute("name") )->second;

						if(part == "fg")
						{
							fg = true;
							theme.fg = pcolor;
						}
						else if(part == "bg")
						{
							bg = true;
							theme.bg = pcolor;
						}
						else if(part == "bs")
						{
							bs = true;
							theme.bs = pcolor;
						}
						else if(part == "sl")
						{
							sl = true;
							theme.sl = pcolor;
						}
					}

					color = color->NextSiblingElement("color");
				}

				if( !fg )
					theme.fg = gcn::Color(0, 0, 0, 255);
				if( !bg )
					theme.bg = gcn::Color(0, 0, 0, 255);
				if( !bs )
					theme.bs = gcn::Color(0, 0, 0, 255);
				if( !sl )
					theme.sl = gcn::Color(0, 0, 0, 255);

				themes[name] = theme;
			}

			elem = elem->NextSiblingElement("ctheme");
		}

		return themes;
	}

	bool Theme::parseFonts(TiXmlElement* elem, const boost::filesystem::path& base)
	{
		boost::array<bool, ALL_FONTS> inits;
		for(size_t i = 0; i < ALL_FONTS; ++i)
			inits[i] = false;

		elem = elem->FirstChildElement("font");
		while( elem != NULL )
		{
			if( elem->Attribute("part") != NULL )
			{
				std::string part = elem->Attribute("part");
				FontType idx;

				if(part == "menu")
					idx = menu;
				else if(part == "normal")
					idx = normal;
				else
					idx = ALL_FONTS;

				if( idx != ALL_FONTS )
				{
					std::string letters;
					if( elem->Attribute("letters") == NULL )
						letters = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"";
					else
						letters = elem->Attribute("letters");

					if( elem->GetText() != NULL )
					{
						boost::filesystem::path path = base / elem->GetText();

						if( inits[idx] )
							delete m_fonts[idx];
						m_fonts[idx] = new gcn::ImageFont( path.string(), letters );
						inits[idx] = true;
					}
				}
			}

			elem = elem->NextSiblingElement("font");
		}

		for(size_t i = 0; i < ALL_FONTS; ++i)
		{
			if( !inits[i] )
				return false;
		}
		return true;
	}

};//namespace graphics


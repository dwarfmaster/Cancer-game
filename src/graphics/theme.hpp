
#ifndef DEF_GRAPHICS_THEME
#define DEF_GRAPHICS_THEME

#include <guichan.hpp>
#include <map>
#include <string>
#include <boost/array.hpp>
#include <boost/filesystem/path.hpp>
#include "deleterContainer.hpp"

class TiXmlDocument;
class TiXmlElement;

namespace graphics
{
	class DeleterContainer;
	class Gui;
	class TabbedArea;
	class TextBox;
	class FileExplorer;

	class Theme
	{
		public:
			enum Types{DEFAULT, GUICONT, BUTTON, CHECKBOX, RADIO, CONTAINER, LIST, TEXT, LABEL, TAB, SCROLL, SLIDER, LAST}; // LAST est réservé à un usage interne
			enum FontType{normal, menu, ALL_FONTS};

			Theme();
			Theme(const boost::filesystem::path& path);
			~Theme();

			bool load(const boost::filesystem::path& path);

			bool isLoaded() const;
			operator bool() const;
			bool operator!() const;

			// Ces fonctions lancent une exception si le theme n'a pas été chargé
			inline void apply(gcn::Container* w)
			{ apply(w, CONTAINER); }
			inline void apply(gcn::Button* w)
			{ apply(w, BUTTON); }
			inline void apply(gcn::CheckBox* w)
			{ apply(w, CHECKBOX); }
			inline void apply(gcn::RadioButton* w)
			{ apply(w, RADIO); }
			inline void apply(gcn::ListBox* w)
			{ apply(w, LIST); }
			inline void apply(gcn::DropDown* w)
			{ apply(w, LIST); }
			inline void apply(gcn::TextField* w)
			{ apply(w, TEXT); }
			inline void apply(gcn::TextBox* w)
			{ apply(w, TEXT); }
			inline void apply(gcn::Label* w)
			{ apply(w, LABEL); }
			inline void apply(gcn::Icon* w)
			{ apply(w, LABEL); }
			inline void apply(gcn::ScrollArea* w)
			{ apply(w, SCROLL); }
			inline void apply(gcn::Slider* w)
			{ apply(w, SLIDER); }
			void apply(gcn::TabbedArea* w) // Déconseillé : utiliser graphics::TabbedArea
			{ apply(w, TAB); } 
			void apply(graphics::TabbedArea* w);
			void apply(graphics::TextBox* w);
			void apply(graphics::FileExplorer* w);

			void apply(gcn::Widget* w, const Types& type);

		private:
			struct ctheme
			{
				gcn::Color fg;
				gcn::Color bg;
				gcn::Color sl;
				gcn::Color bs;
			};//struct ctheme
			boost::array<ctheme, LAST> m_links;
			bool m_load;

			// Pour l'usage interne de Gui
			friend Gui;
			inline void applyInter(DeleterContainer* w)
			{ apply(w, GUICONT); }
			boost::array<gcn::ImageFont*, ALL_FONTS> m_fonts;

			std::map<std::string, gcn::Color> parseColors(TiXmlDocument* file);
			gcn::Color parseColor(TiXmlElement* elem);
			bool parseFonts(TiXmlElement* elem, const boost::filesystem::path& base);
			std::map<std::string, ctheme> parseThemes(TiXmlDocument* file, const std::map<std::string,gcn::Color>& colors);

	};//class Theme
};//namespace graphics

#endif//DEF_GRAPHICS_THEME


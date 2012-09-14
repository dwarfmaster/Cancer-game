
#ifndef DEF_GRAPHICS_RADIOBUTTON
#define DEF_GRAPHICS_RADIOBUTTON

#include <guichan.hpp>
#include <string>
#include <vector>
#include <boost/function.hpp>

namespace graphics
{
	class RadioButton : public gcn::RadioButton
	{
		public:
			typedef boost::function<void (std::string, std::string)> callback_t; // groupe en premier, nom en deuxième

			RadioButton();
			RadioButton(const std::string& caption, const std::string& group, bool selected = false);

			virtual void mouseClicked(gcn::MouseEvent& event);
			virtual void keyPressed(gcn::KeyEvent& event);
			void setSelected(bool selected);

			std::string getSelected() const; // Retourne le nom du sélectionné dans le groupe, renvoit une chaine vide si aucun n'est sélectionné
			std::vector<std::string> groupContain() const;

			void set(const callback_t& callback);
			void clear();
			bool isSet() const;

		private:
			callback_t m_callback;
			bool m_last;

			void update();
	};//class RadioButton
};//namespace graphics

#endif//DEF_GRAPHICS_RADIOBUTTON

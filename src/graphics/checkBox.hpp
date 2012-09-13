
#ifndef DEF_GRAPHICS_CHECKBOX
#define DEF_GRAPHICS_CHECKBOX

#include "guichan.hpp"
#include <string>
#include <boost/function.hpp>

namespace graphics
{
	class CheckBox : public gcn::CheckBox
	{
		public:
			CheckBox();
			CheckBox(const std::string& caption, bool selected = false);

			void set(const boost::function<void (bool)>& callback);
			void clear();
			bool isSet() const;

			virtual void keyPressed(gcn::KeyEvent& event);
			virtual void mouseClicked(gcn::MouseEvent& event);
			void setSelected(bool s);

		private:
			boost::function<void (bool)> m_callback; // Appelée à chaque changement d'état du checkbox
			bool m_last;

			void update();
	};//class CheckBox
};//namespace graphics

#endif//DEF_GRAPHICS_CHECKBOX


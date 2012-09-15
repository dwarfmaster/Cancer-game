
#ifndef DEF_GRAPHICS_TEXTBOX
#define DEF_GRAPHICS_TEXTBOX

#include <guichan.hpp>
#include <boost/function.hpp>
#include <string>

namespace graphics
{
	namespace internal
	{
		class FocusListener;
	};

	class TextBox : public gcn::TextBox
	{
		public:
			TextBox();
			TextBox(const std::string& caption);
			~TextBox();

			virtual void keyPressed(gcn::KeyEvent& ev);
			void setEmptyMessage(const std::string& msg);
			std::string emptyMessage() const;

		private:
			std::string m_message;
			bool m_empty;
			graphics::internal::FocusListener* m_listener;

			void update();
			void setupListener();
			void changeFocus(bool foc);
	};//class TextBox
};//namespace graphics

#endif//DEF_GRAPHICS_TEXTBOX


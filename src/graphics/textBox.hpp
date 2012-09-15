
#ifndef DEF_GRAPHICS_TEXTBOX
#define DEF_GRAPHICS_TEXTBOX

#include <guichan.hpp>
#include <boost/function.hpp>
#include <string>

namespace graphics
{
	class TextBox;

	namespace internal
	{
		class FocusListener;

		class Box : public gcn::TextBox
		{
			public:
				Box(graphics::TextBox* b);
				Box(const std::string& c, graphics::TextBox* b);
				virtual void keyPressed(gcn::KeyEvent& ev);

			private:
				graphics::TextBox* m_parent;
		};
	};

	class TextBox : public gcn::ScrollArea
	{
		friend internal::Box;

		public:
			TextBox();
			TextBox(const std::string& caption);
			~TextBox();

			virtual void keyPressed(gcn::KeyEvent& ev);
			void setEmptyMessage(const std::string& msg);
			std::string emptyMessage() const;

		private:
			internal::Box* m_text;

			std::string m_message;
			bool m_empty;
			graphics::internal::FocusListener* m_listener;

			void prepare();
			void update();
			void setupListener();
			void setupText(const std::string& str);
			void changeFocus(bool foc);
	};//class TextBox
};//namespace graphics

#endif//DEF_GRAPHICS_TEXTBOX


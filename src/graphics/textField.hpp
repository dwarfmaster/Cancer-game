
#ifndef DEF_GRAPHICS_TEXTFIELD
#define DEF_GRAPHICS_TEXTFIELD

#include <guichan.hpp>
#include <boost/function.hpp>
#include <string>

namespace graphics
{
	namespace internal
	{
		class FocusListener;
	};

	class TextField : public gcn::TextField
	{
		public:
			typedef boost::function<void (std::string)> cb_t;

			TextField();
			TextField(const std::string& caption);
			~TextField();

			void setChangeCB(const cb_t& cb);
			void clearChangeCB();
			bool isChangeCB() const;

			void setValidCB(const cb_t& cb);
			void clearValidCB();
			bool isValidCB() const;

			void setEmptyMessage(const std::string& msg);
			std::string emptyMessage() const;

			virtual void keyPressed(gcn::KeyEvent& ev);

		private:
			cb_t m_changedCB;
			cb_t m_validCB;
			std::string m_last;
			std::string m_message;
			bool m_empty;
			graphics::internal::FocusListener* m_listener;

			void update();
			void setupListener();
			void changeFocus(bool foc);
	};//class TextField
};//namespace graphics

#endif//DEF_GRAPHICS_TEXTFIELD


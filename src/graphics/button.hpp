
#ifndef DEF_GRAPHICS_BUTTON
#define DEF_GRAPHICS_BUTTON

#include <guichan.hpp>
#include <boost/function.hpp>
#include <string>

namespace graphics
{
	class Button : public gcn::Button
	{
		public:
			Button();
			Button(const std::string& caption);
			Button(const std::string& caption, const boost::function<void ()>& callback);

			bool isSet() const;
			void set(const boost::function<void ()>& callback);
			void clear();

			virtual void keyPressed(gcn::KeyEvent &keyEvent);
			virtual void keyReleased(gcn::KeyEvent &keyEvent);
			virtual void mousePressed(gcn::MouseEvent &mouseEvent);
			virtual void mouseReleased(gcn::MouseEvent &mouseEvent);

			inline bool pressed() const
			{ return isPressed(); }

		private:
			boost::function<void ()> m_callback;

			inline void update()
			{
				if( pressed()
						&& m_callback )
					m_callback();
			}
	};//class Button
};//namespace graphics

#endif//DEF_GRAPHICS_BUTTON


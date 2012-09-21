
#ifndef DEF_GRAPHICS_SLIDER
#define DEF_GRAPHICS_SLIDER

#include <guichan.hpp>
#include <boost/function.hpp>

namespace graphics
{
	class Slider : public gcn::Slider
	{
		public:
			typedef boost::function<void (double)> callback_t;

			Slider(double end=1.0);
			Slider(double start, double end);

			virtual void keyPressed(gcn::KeyEvent& ev);
			virtual void mouseDragged(gcn::MouseEvent& ev);
			virtual void mousePressed(gcn::MouseEvent& ev);
			virtual void mouseWheelMovedDown(gcn::MouseEvent& ev);
			virtual void mouseWheelMovedUp(gcn::MouseEvent& ev);

			void setLimit(double nlimit);
			double limit() const;
			void set(const callback_t& cb);
			void clear();
			bool isSet() const;

		private:
			double m_last;
			double m_limit;
			callback_t m_callback;

			void update();
	};//class Slider
};//namespace graphics

#endif//DEF_GRAPHICS_SLIDER


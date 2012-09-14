
#ifndef DEF_GRAPHICS_INTERNAL_SELECTLISTENER
#define DEF_GRAPHICS_INTERNAL_SELECTLISTENER

#include <guichan.hpp>
#include <boost/function.hpp>

namespace graphics
{
	namespace internal
	{
		class SelectListener : public gcn::SelectionListener
		{
			public:
				SelectListener();
				SelectListener(const boost::function<void (std::string)>& callback);

				void set(const boost::function<void (std::string)>& callback);
				void clear();
				bool isSet();

				virtual void valueChanged(const gcn::SelectionEvent& ev);

			private:
				boost::function<void (std::string)> m_callback;
		};//class SelectListener
	};//namespace internal
};//namespace graphics

#endif//DEF_GRAPHICS_INTERNAL_SELECTLISTENER


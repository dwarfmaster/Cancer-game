
#ifndef DEF_GRAPHICS_INTERNAL_FOCUSLISTENER
#define DEF_GRAPHICS_INTERNAL_FOCUSLISTENER

#include <guichan.hpp>
#include <boost/function.hpp>

namespace graphics
{
	namespace internal
	{
		class FocusListener : public gcn::FocusListener
		{
			public:
				typedef boost::function<void (bool)> cb_t;

				FocusListener(const cb_t& cb);
				void set(const cb_t& cb);
				void clear();
				bool isSet() const;

				virtual void focusGained(const gcn::Event&);
				virtual void focusLost(const gcn::Event&);

			private:
				cb_t m_callback;

		};//class FocusListener
	};//namespace internal
};//namespace graphics

#endif//DEF_GRAPHICS_INTERNAL_FOCUSLISTENER



#include "focusListener.hpp"

namespace graphics
{
	namespace internal
	{
		FocusListener::FocusListener(const cb_t& cb)
			: m_callback(cb)
		{}

		void FocusListener::set(const cb_t& cb)
		{
			m_callback = cb;
		}

		void FocusListener::clear()
		{
			m_callback.clear();
		}

		bool FocusListener::isSet() const
		{
			return !m_callback.empty();
		}

		void FocusListener::focusGained(const gcn::Event&)
		{
			if( m_callback )
				m_callback(true);
		}

		void FocusListener::focusLost(const gcn::Event&)
		{
			if( m_callback )
				m_callback(false);
		}

	};//namespace internal
};//namespace graphics


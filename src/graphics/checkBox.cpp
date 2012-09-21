
#include "checkBox.hpp"

namespace graphics
{
	CheckBox::CheckBox()
		: gcn::CheckBox(), m_last(true)
	{}

	CheckBox::CheckBox(const std::string& caption, bool selected)
		: gcn::CheckBox(caption, selected), m_last(!selected)
	{}

	void CheckBox::set(const boost::function<void (bool)>& callback)
	{
		m_callback = callback;
	}

	void CheckBox::clear()
	{
		m_callback.clear();
	}

	bool CheckBox::isSet() const
	{
		return !m_callback.empty();
	}

	void CheckBox::keyPressed(gcn::KeyEvent& event)
	{
		update();
		gcn::CheckBox::keyPressed(event);
	}

	void CheckBox::mouseClicked(gcn::MouseEvent& event)
	{
		update();
		gcn::CheckBox::mouseClicked(event);
	}

	void CheckBox::setSelected(bool s)
	{
		gcn::CheckBox::setSelected(s);
		update();
	}
	

	void CheckBox::update()
	{
		bool state = isSelected();

		if( state != m_last )
		{
			if( m_callback )
				m_callback(!state);
			m_last = state;
		}
	}

};//namespace graphics


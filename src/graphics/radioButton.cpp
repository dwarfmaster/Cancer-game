
#include "radioButton.hpp"

namespace graphics
{
	RadioButton::RadioButton()
		: gcn::RadioButton(), m_last(false)
	{}

	RadioButton::RadioButton(const std::string& caption, const std::string& group, bool selected)
		: gcn::RadioButton(caption, group, selected), m_last(selected)
	{}

	void RadioButton::mouseClicked(gcn::MouseEvent& event)
	{
		gcn::RadioButton::mouseClicked(event);
		update();
	}

	void RadioButton::keyPressed(gcn::KeyEvent& event)
	{
		gcn::RadioButton::keyPressed(event);
		update();
	}
	
	std::string RadioButton::getSelected() const
	{
		if( !mGroup.empty() )
		{
			GroupIterator it, end;
			end = mGroupMap.upper_bound(mGroup);

			for(it = mGroupMap.lower_bound(mGroup); it != end; ++it)
			{
				if( it->second->isSelected() )
					return it->second->getCaption();
			}
		}

		return "";
	}

	std::vector<std::string> RadioButton::groupContain() const
	{
		std::vector<std::string> ret;

		if( mGroup.empty() )
			return ret;
		else
		{
			GroupIterator it, end;
			end = mGroupMap.upper_bound(mGroup);

			for(it = mGroupMap.lower_bound(mGroup); it != end; ++it)
				ret.push_back( it->second->getCaption() );
			return ret;
		}
	}

	void RadioButton::setSelected(bool selected)
	{
		gcn::RadioButton::setSelected(selected);
		update();
	}

	void RadioButton::set(const callback_t& callback)
	{
		m_callback = callback;
	}

	void RadioButton::clear()
	{
		m_callback.clear();
	}

	bool RadioButton::isSet() const
	{
		return !m_callback.empty();
	}

	void RadioButton::update()
	{
		bool state = isSelected();

		if( state != m_last )
		{
			if( m_callback )
				m_callback(mGroup, mCaption);
			m_last = state;
		}
	}

};//namespace graphics


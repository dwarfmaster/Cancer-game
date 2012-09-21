
#include "tabbedArea.hpp"

namespace graphics
{
	TabbedArea::TabbedArea()
		: gcn::TabbedArea(), m_last(0)
	{}
	
	TabbedArea::~TabbedArea()
	{
		for(size_t i = 0; i < mTabs.size(); ++i)
			delete mTabs[i].second;
	}

	bool TabbedArea::isSelected(const std::string& name) const
	{
		size_t idx = exists(name);

		if( idx == nbTabs() )
			return false;
		else
			return isTabSelected(idx);
	}

	size_t TabbedArea::exists(const std::string& name) const
	{
		for(size_t i = 0; i < mTabs.size(); ++i)
		{
			if( mTabs[i].first->getCaption() == name )
				return i;
		}

		return nbTabs();
	}

	size_t TabbedArea::nbTabs() const
	{
		return mTabs.size();
	}

	std::vector<std::string> TabbedArea::listOfTabs() const
	{
		std::vector<std::string> list;
		for(size_t i = 0; i < mTabs.size(); ++i)
			list.push_back( mTabs[i].first->getCaption() );

		return list;
	}
	
	bool TabbedArea::select(const std::string& name)
	{
		size_t idx = exists(name);
		if( idx == nbTabs() )
			return false;

		setSelectedTab(idx);
		return true;
	}

	bool TabbedArea::add(const std::string& name, Widget* widget)
	{
		if( exists(name) != nbTabs() )
			return false;

		addTab(name, widget);
		return true;
	}

	bool TabbedArea::remove(const std::string& name)
	{
		size_t idx = exists(name);
		if( idx == nbTabs() )
			return false;

		removeTabWithIndex(idx);
		return true;
	}

	bool TabbedArea::rename(const std::string& idx, const std::string& nname)
	{
		size_t id = exists(idx);
		if( id == nbTabs() )
			return false;

		mTabs[id].first->setCaption(nname);
		return true;
	}

	void TabbedArea::setCB(const cb_t& cb)
	{
		m_callback = cb;
	}

	void TabbedArea::clearCB()
	{
		m_callback.clear();
	}

	bool TabbedArea::isCB() const
	{
		return !m_callback.empty();
	}

	void TabbedArea::keyPressed(gcn::KeyEvent& ev)
	{
		gcn::TabbedArea::keyPressed(ev);
		update();
	}

	void TabbedArea::mousePressed(gcn::MouseEvent& ev)
	{
		gcn::TabbedArea::mousePressed(ev);
		update();
	}

	void TabbedArea::update()
	{
		size_t selected = getSelectedTabIndex();
		if( selected != m_last )
		{
			if( m_callback )
				m_callback( mSelectedTab->getCaption(), mTabs[selected].second );
			m_last = selected;
		}
	}
};//namespace graphics


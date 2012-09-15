
#include "textBox.hpp"
#include "focusListener.hpp"
#include <boost/bind.hpp>

namespace graphics
{
	TextBox::TextBox()
		: gcn::TextBox(), m_message(""), m_empty(true)
	{
		setupListener();
	}

	TextBox::TextBox(const std::string& caption)
		: gcn::TextBox(caption), m_message("")
	{
		m_empty = caption.empty();
		setupListener();
	}

	TextBox::~TextBox()
	{
		delete m_listener;
	}

	void TextBox::setEmptyMessage(const std::string& msg)
	{
		m_message = msg;
		update();
	}

	std::string TextBox::emptyMessage() const
	{
		return m_message;
	}

	void TextBox::keyPressed(gcn::KeyEvent& ev)
	{
		if( m_empty )
			setText("");

		gcn::TextBox::keyPressed(ev);
		update();
	}

	void TextBox::update()
	{
		std::string txt = getText();

		if( txt.empty() )
		{
			m_empty = true;
			if( isFocused() )
				setText("");
			else
				setText(m_message);
		}
		else
			m_empty = false;
	}
	
	void TextBox::setupListener()
	{
		m_listener = new graphics::internal::FocusListener(
				boost::bind(&graphics::TextBox::changeFocus, this, _1) );
		addFocusListener(m_listener);
	}

	void TextBox::changeFocus(bool foc)
	{
		if( m_empty )
		{
			if( foc )
				setText("");
			else
				setText(m_message);
		}
	}

};//namespace graphics


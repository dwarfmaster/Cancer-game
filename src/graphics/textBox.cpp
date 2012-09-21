
#include "textBox.hpp"
#include "focusListener.hpp"
#include <boost/bind.hpp>

namespace graphics
{
	TextBox::TextBox()
		: gcn::ScrollArea(), m_message(""), m_empty(true)
	{
		setupText("");
		setupListener();
		setScrollbarWidth(5);
	}

	TextBox::TextBox(const std::string& caption)
		: gcn::ScrollArea(), m_message("")
	{
		setupText(caption);
		m_empty = caption.empty();

		setupListener();
		setScrollbarWidth(5);
	}

	TextBox::~TextBox()
	{
		delete m_text;
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
		prepare();
		m_text->keyPressed(ev);
		update();
	}

	void TextBox::prepare()
	{
		if( m_empty )
			m_text->setText("");
	}

	void TextBox::update()
	{
		std::string txt = m_text->getText();

		if( txt.empty() )
		{
			m_empty = true;
			if( isFocused() 
					|| m_text->isFocused() )
				m_text->setText("");
			else
				m_text->setText(m_message);
		}
		else
			m_empty = false;
	}
	
	void TextBox::setupListener()
	{
		m_listener = new graphics::internal::FocusListener(
				boost::bind(&graphics::TextBox::changeFocus, this, _1) );
		addFocusListener(m_listener);
		m_text->addFocusListener(m_listener);
	}

	void TextBox::changeFocus(bool foc)
	{
		if( m_empty )
		{
			if( foc )
				m_text->setText("");
			else
				m_text->setText(m_message);
		}
	}
	
	void TextBox::setupText(const std::string& str)
	{
		m_text = new internal::Box(str, this);
		setContent(m_text);
	}

	namespace internal
	{
		Box::Box(graphics::TextBox* b)
			: gcn::TextBox(), m_parent(b)
		{}

		Box::Box(const std::string& c, graphics::TextBox* b)
			: gcn::TextBox(c), m_parent(b)
		{}

		void Box::keyPressed(gcn::KeyEvent& ev)
		{
			m_parent->update();
			gcn::TextBox::keyPressed(ev);
			m_parent->update();
		}
	};//namespace internal

};//namespace graphics


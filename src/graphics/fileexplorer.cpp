
#include "fileexplorer.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>

#include "../i18n.hpp"
#include "checkBox.hpp"
#include "list.hpp"
#include "button.hpp"
#include "textField.hpp"

namespace fs = boost::filesystem;

namespace graphics
{
	FileExplorer::FileExplorer(bool dir)
		: m_path(fs::initial_path()), m_dir(dir), m_all(false)
	{
		setup();
		update();
	}

	FileExplorer::FileExplorer(const fs::path& path, bool dir)
		: m_path(path), m_dir(dir), m_all(false)
	{
		setup();
		update();
	}

	FileExplorer::FileExplorer(const reglist_t& regs, bool dir)
		: m_regs(regs), m_path(fs::initial_path()), m_dir(dir), m_all(false)
	{
		setup();
		update();
	}

	FileExplorer::~FileExplorer()
	{
		delete m_field;
		delete m_dirs;
		if( !m_dir )
		{
			delete m_files;
			delete m_scrlfile;
		}

		delete m_valid;
		delete m_enter;
		delete m_up;
		delete m_call;
		delete m_scrldir;
	}

	FileExplorer::reglist_t FileExplorer::getRegs() const
	{
		return m_regs;
	}

	void FileExplorer::setRegs(const reglist_t& regs)
	{
		m_regs = regs;
	}

	void FileExplorer::addReg(const std::string& reg)
	{
		m_regs.push_back(reg);
	}

	void FileExplorer::setPath(const fs::path& path)
	{
		m_path = path;
	}

	fs::path FileExplorer::getActPath() const
	{
		return m_path;
	}

	void FileExplorer::setFoundCB(const cb_t& cb)
	{
		m_found = cb;
	}

	void FileExplorer::clearCB()
	{
		m_found.clear();
	}

	bool FileExplorer::isCB() const
	{
		return !m_found.empty();
	}

	void FileExplorer::setup()
	{
		m_field = new TextField( m_path.string() );
		m_field->setValidCB( boost::bind(&FileExplorer::changePath, this, _1) );
		add(m_field);

		// This text is about files
		m_up = new Button( _i("Up") );
		m_up->adjustSize();
		m_up->set( boost::bind(&FileExplorer::up, this) );
		add(m_up);

		m_dirs = new List;
		m_dirs->setWrappingEnabled(true);
		m_scrldir = new gcn::ScrollArea(m_dirs);
		add(m_scrldir);

		if( !m_dir )
		{
			m_files = new List;
			m_files->setWrappingEnabled(true);
			m_scrlfile = new gcn::ScrollArea(m_files);
			add(m_scrlfile);
		}

		// This text is about files
		m_call = new CheckBox( _i("Show all") );
		m_call->set( boost::bind(&FileExplorer::toggleAll, this, _1) );
		add(m_call);

		// This text is about files
		m_valid = new Button( _i("Select") );
		m_valid->adjustSize();
		m_valid->set( boost::bind(&FileExplorer::select, this) );
		add(m_valid);

		// This text is about files
		m_enter = new Button( _i("Enter") );
		m_enter->adjustSize();
		m_enter->set( boost::bind(&FileExplorer::enter, this) );
		add(m_enter);

		upPos();
	}

	void FileExplorer::select()
	{
		if( !m_found )
			return;

		std::string selected;
		if( m_dir )
			selected = m_dirs->getListModel()->getElementAt( m_dirs->getSelected() );
		else
			selected = m_files->getListModel()->getElementAt( m_files->getSelected() );

		if( selected.empty() )
			return;

		fs::path path = m_path / selected;
		if( m_dir )
			path /= "/";

		m_found( path.string() );
	}

	void FileExplorer::enter()
	{
		std::string tmp = m_dirs->getListModel()->getElementAt( m_dirs->getSelected() );
		if( tmp.empty() )
			return;

		m_path = m_path / tmp;
		m_field->setText( m_path.string() );
		update();
	}

	void FileExplorer::toggleAll(bool all)
	{
		m_all = all;
		update();
	}

	void FileExplorer::update()
	{
		upPos();
		Dir dir = scanDir( m_path );

		**m_dirs = dir.dirs;
		m_dirs->adjustSize();

		if( !m_dir )
		{
			**m_files = dir.files;
			m_files->adjustSize();
		}

		m_field->setText( m_path.string() );
	}

	FileExplorer::Dir FileExplorer::scanDir(const fs::path& path)
	{
		Dir dir;

		for(fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
		{
			std::string str = it->path().leaf().string();
			if( str[0] != '.'
					|| m_all )
			{
				if( fs::is_directory(*it) )
					dir.dirs.push_back(str);
				else
				{
					bool toShow = false;
					if( m_regs.empty() )
						toShow = true;
					else
					{
						for(size_t i = 0; i < m_regs.size() && !toShow; ++i)
						{
							boost::regex reg(m_regs[i]);
							if( boost::regex_match(str, reg) )
								toShow = true;
						}
					}

					if( toShow )
						dir.files.push_back(str);
				}
			}
		}

		return dir;
	}

	void FileExplorer::changePath(std::string npath)
	{
		fs::path path(npath);

		if( fs::exists(path)
				|| fs::is_directory(path) )
		{
			m_path = path;
			update();
		}
		else
			m_field->setText( m_path.string() );
	}

	void FileExplorer::draw(gcn::Graphics* g)
	{
		g->setColor(getBaseColor());
		g->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));

		drawChildren(g);
	}

	void FileExplorer::upPos()
	{
		gcn::Rectangle rect = getDimension();
		sdl::AABB size( sdl::makeRect(rect.x, rect.y, rect.width, rect.height) );

		m_field->setDimension( gcn::Rectangle(5, 5, size->w - m_up->getWidth() - 15, 20) );
		m_up->setPosition( size->w - m_up->getWidth() - 5, 5 );

		if( m_dir )
			m_scrldir->setDimension( gcn::Rectangle(5, 35, size->w - 10, size->h - 70) );
		else
			m_scrldir->setDimension( gcn::Rectangle(5, 35, size->w/2 - 10, size->h - 70) );
		m_dirs->setWidth( m_scrldir->getWidth() - m_scrldir->getScrollbarWidth() );

		if( !m_dir )
		{
			m_scrlfile->setDimension( gcn::Rectangle(size->w/2 + 5, 35, size->w/2 - 10, size->h - 70) );
			m_files->setWidth( m_scrlfile->getWidth() - m_scrlfile->getScrollbarWidth() );
		}

		m_call->setPosition(5, size->h - 25);
		m_valid->setDimension( gcn::Rectangle(size->w - m_valid->getWidth() - 5, size->h - 25, m_valid->getWidth(), 20) );
		m_enter->setDimension( gcn::Rectangle(size->w - m_valid->getWidth() - m_enter->getWidth() - 10, size->h - 25, m_enter->getWidth(), 20) );
	}

	void FileExplorer::setDimension(const gcn::Rectangle& dimension)
	{
		gcn::Widget::setDimension(dimension);
		update();
	}

	void FileExplorer::up()
	{
		fs::path process = m_path;
		m_path.clear();
		for(fs::path::iterator it = process.begin(); it != --process.end(); ++it)
			m_path /= *it;

		if( m_path.empty() )
			m_path = "/";

		update();
	}

};//namespace graphics


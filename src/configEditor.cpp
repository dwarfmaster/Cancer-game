
#include "configEditor.hpp"

#include "graphics/slider.hpp"
#include "graphics/textField.hpp"
#include "graphics/checkBox.hpp"
#include "graphics/gui.hpp"
#include "graphics/tabbedArea.hpp"
#include "graphics/theme.hpp"
#include "graphics/deleterContainer.hpp"
#include "graphics/fileexplorer.hpp"
#include "graphics/button.hpp"
#include "graphics/label.hpp"

#include <SDL/SDL.h>
#include <SDLP_position.hpp>
#include <SDLP_tools.hpp>

#include "i18n.hpp"
#include "core/exception.hpp"
#include "core/sounds.hpp"
#include "core/config.hpp"
#include "core/event.hpp"

#include <boost/bind.hpp>
#include <boost/filesystem/fstream.hpp>

	ConfigEditor::ConfigEditor(SDL_Surface* bg)
: m_gui( *graphics::gui ), ecran(SDL_GetVideoSurface()), m_bg(bg), m_quit(false)
{
	if( ecran == NULL )
		throw core::Exception( _i("Can't get the window surface.") );

	m_gtheme = core::cfg->gtheme();
	m_sounds = core::cfg->sounds();
}

void ConfigEditor::run()
{
	setGui();

	sdl::Event& ev = *core::ev;

	while( !ev.quit()
			&& !m_quit )
	{
		ev.update();
		m_gui.update();

		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		SDL_BlitSurface(m_bg, NULL, ecran, NULL);
		m_gui.draw();
		SDL_Flip(ecran);
	}

	return;
}

void ConfigEditor::setGui()
{
	// On vide la gui
	std::vector<std::string> conts = m_gui.listContainers();
	for(size_t i = 0; i < conts.size(); ++i)
		m_gui.deleteContainer(conts[i]);

	// On définit la nouvelle gui.
	m_gui.setDefaultFont( graphics::Theme::normal );

	sdl::AABB size = core::cfg->size();
	m_gui.addContainer("config");
	m_gui.setCurrent("config");
	m_gui->setDimension( gcn::Rectangle(10, 10, size->w - 20, size->h - 20) );

	graphics::Theme& t = *m_gui.getTheme();

	graphics::TabbedArea* tabs = new graphics::TabbedArea;
	tabs->setDimension( gcn::Rectangle(10, 10, m_gui->getWidth() - 20, m_gui->getHeight() - 40) );
	t.apply(tabs);
	m_gui->add(tabs);

	graphics::Button* bsave = new graphics::Button( _i("Save") );
	bsave->adjustSize();
	bsave->setPosition(m_gui->getWidth() - bsave->getWidth() - 5, tabs->getHeight() + tabs->getY() + 5);
	bsave->set( boost::bind(&ConfigEditor::save, this) );
	t.apply(bsave);
	m_gui->add(bsave);

	graphics::Button* bcancel = new graphics::Button( _i("Cancel") );
	bcancel->adjustSize();
	bcancel->setPosition(m_gui->getWidth() - bsave->getWidth() - bcancel->getWidth() - 10, tabs->getHeight() + tabs->getY() + 5);
	bcancel->set( boost::bind(&ConfigEditor::cancel, this) );
	t.apply(bcancel);
	m_gui->add(bcancel);

	// Graphics edition
	graphics::DeleterContainer* graph = new graphics::DeleterContainer;
	graph->setDimension( gcn::Rectangle(0, 0, tabs->getWidth(), tabs->getHeight() - 30) );
	t.apply(graph);

	m_fullscreen = new graphics::CheckBox( _i("fullscreen"), core::cfg->fullscreen() );
	t.apply(m_fullscreen);
	m_width = new graphics::TextField( sdl::itoa(size->w) );
	m_width->setEmptyMessage( _i("Window width.") );
	t.apply(m_width);
	m_height = new graphics::TextField( sdl::itoa(size->h) );
	m_height->setEmptyMessage( _i("Window height.") );
	t.apply(m_height);

	int width = tabs->getWidth() - 10 - m_fullscreen->getWidth();
	width /= 2;
	m_width->setDimension( gcn::Rectangle(5, 5, width - 5, 20) );
	m_height->setDimension( gcn::Rectangle(width + 5, 5, width - 5, 20) );
	m_fullscreen->setPosition(2 * width, 5);

	graphics::FileExplorer* gtheme = new graphics::FileExplorer( m_gtheme.branch_path() );
	gtheme->setDimension( gcn::Rectangle(5, 30, tabs->getWidth() - 10, tabs->getHeight() - 60) );
	gtheme->setFoundCB( boost::bind(&ConfigEditor::setGtheme, this, _1) );
	t.apply(gtheme);

	graph->add(m_width);
	graph->add(m_height);
	graph->add(m_fullscreen);
	graph->add(gtheme);
	tabs->addTab(_i("Graphics"), graph);

	// Sound edition
	graphics::DeleterContainer* sounds = new graphics::DeleterContainer;
	sounds->setDimension( gcn::Rectangle(0, 0, tabs->getWidth(), tabs->getHeight() - 30) );
	t.apply(sounds);

	width = sounds->getWidth();
	width /= 2;
	graphics::Label* lmusic = new graphics::Label( _i("Music volume :") );
	lmusic->setPosition(10, 5);
	t.apply(lmusic);

	m_music = new graphics::Slider(0, 255);
	m_music->setValue( core::cfg->volume() );
	m_music->setDimension( gcn::Rectangle(5, 5 + lmusic->getHeight(), width - 10, 15) );
	t.apply(m_music);

	graphics::Label* lsound = new graphics::Label( _i("Sound volume :") );
	lsound->setPosition(m_music->getX() + m_music->getWidth() + 10, 5);
	t.apply(lsound);

	m_sound = new graphics::Slider(0, 255);
	m_sound->setValue( core::cfg->volume(true) );
	m_sound->setDimension( gcn::Rectangle(lsound->getX() - 5, 5 + lsound->getHeight(), width - 10, 15) );
	t.apply(m_sound);

	graphics::FileExplorer* fsound = new graphics::FileExplorer(m_sounds, true);
	fsound->setDimension( gcn::Rectangle(5, m_music->getY() + m_music->getHeight() + 10, sounds->getWidth() - 10, sounds->getHeight() - m_music->getY() - m_music->getHeight() - 15) );
	fsound->setFoundCB( boost::bind(&ConfigEditor::setSound, this, _1) );
	t.apply(fsound);

	sounds->add(lmusic);
	sounds->add(m_music);
	sounds->add(lsound);
	sounds->add(m_sound);
	sounds->add(fsound);
	tabs->addTab(_i("Audio"), sounds);
}

void ConfigEditor::save()
{
	// On enregistre
	boost::filesystem::ofstream ofs( core::cfg->m_config );
	if( !ofs )
	{
		std::cerr << _i("Can't save the configuration, please check the file exists.") << std::endl;
		goto applySave;
	}

	if( m_fullscreen->isSelected() )
		ofs << "fullscreen=" << "yes" << std::endl;
	ofs << "size=" << sdl::atoi(m_width->getText()) << "x" << sdl::atoi(m_height->getText()) << std::endl;
	ofs << "gtheme=" << m_gtheme.string() << std::endl;

	ofs << "sounds=" << m_sounds.string() << std::endl;
	ofs << "\n[audio]" << std::endl;
	ofs << "sounds=" << static_cast<unsigned short int>(m_sound->getValue()) << std::endl;;
	ofs << "music=" << static_cast<unsigned short int>(m_music->getValue()) << std::endl;;

	// On applique
applySave:
	core::cfg->m_music = static_cast<unsigned short int>(m_music->getValue());
	core::cfg->m_sounds = static_cast<unsigned short int>(m_sound->getValue());
	core::sounds->setMusicVolume( core::cfg->volume() );
	core::sounds->setSoundsVolume( core::cfg->volume(true) );
	core::cfg->m_dsounds = m_sounds;

	core::cfg->m_gtheme = m_gtheme;
	// On doit relancer pour m_size et m_fullscreen

	core::sounds->playSound( core::Sounds::ok );
	m_quit = true;
}

void ConfigEditor::cancel()
{
	core::sounds->playSound( core::Sounds::cancel );
	m_quit = true;
}

void ConfigEditor::setSound(const boost::filesystem::path& path)
{
	m_sounds = path;
}

void ConfigEditor::setGtheme(const boost::filesystem::path& path)
{
	m_gtheme = path;
}



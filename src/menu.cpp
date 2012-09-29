
#include "menu.hpp"

#include "core/config.hpp"
#include "core/sounds.hpp"
#include "core/event.hpp"
#include "core/exception.hpp"
#include "i18n.hpp"

#include <guichan.hpp>
#include "graphics/gui.hpp"
#include "graphics/theme.hpp"
#include "graphics/button.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDLP_position.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/bind.hpp>
#include <sstream>
#include <vector>
#include <string>

#include "configEditor.hpp"

namespace fs = boost::filesystem;

const char* menu_bg_path = "menu/bg.png";

Menu::Menu()
	: m_gui(*graphics::gui), m_theme( *m_gui.getTheme() ), m_bg(NULL), ecran( SDL_GetVideoSurface() )
{
	if( ecran == NULL )
		throw core::Exception( _i("Can't get the video surface") );

	initGui();
	loadBG();
}

void Menu::run()
{
	setGui();

	sdl::Event& ev = *core::ev;
	core::sounds->playMusic(core::Sounds::menu);

	while( !ev.quit() )
	{
		ev.update();
		m_gui.update();

		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		SDL_BlitSurface(m_bg, NULL, ecran, NULL);
		m_gui.draw();
		SDL_Flip(ecran);
	}
}

Menu::~Menu()
{
	delete load;
	delete ngame;
	delete opts;

	SDL_FreeSurface(m_bg);
}

void Menu::initGui()
{
	// On place les éléments.
	load = new graphics::Button( _i("Load game") );
	load->set( boost::bind(&Menu::loadGame, this) );
	m_theme.apply(load);

	ngame = new graphics::Button( _i("New game") );
	ngame->set( boost::bind(&Menu::newGame, this) );
	m_theme.apply(ngame);

	opts = new graphics::Button( _i("Options") );
	opts->set( boost::bind(&Menu::editOpts, this) );
	m_theme.apply(opts);
}

void Menu::loadGame()
{
	core::sounds->playSound(core::Sounds::ok);
	std::cout << "Load game !!" << std::endl; // DEBUG
	m_gui->clear();
	// TODO
	setGui();
}

void Menu::newGame()
{
	core::sounds->playSound(core::Sounds::ok);
	std::cout << "New game !!" << std::endl; // DEBUG
	m_gui->clear();
	// TODO
	setGui();
}
		
void Menu::editOpts()
{
	core::sounds->playSound(core::Sounds::ok);

	m_gui->clear();
	ConfigEditor editor(m_bg);
	editor.run();
	setGui();
}

void Menu::loadBG()
{
	fs::path bgPath = core::cfg->getPath( menu_bg_path );
	m_bg = IMG_Load( bgPath.string().c_str() );
	if( m_bg == NULL )
	{
		std::ostringstream oss;
		oss << _i("Error while loading the picture \"") << bgPath.string() << _i("\" : \"") << IMG_GetError() << _i("\"") << std::endl;
		throw core::Exception( oss.str() );
	}

	double fact_x = double(ecran->w) / double(m_bg->w); 
	double fact_y = double(ecran->h) / double(m_bg->h);
	SDL_Surface* tmp = zoomSurface(m_bg, fact_x, fact_y, SMOOTHING_ON);
	if( tmp == NULL )
		throw core::Exception( _i("Error while resizing the menu picture") );

	SDL_FreeSurface(m_bg);
	m_bg = NULL;

	m_bg = SDL_DisplayFormat(tmp);
	if( m_bg == NULL )
		m_bg = tmp;
	else
		SDL_FreeSurface(tmp);
}

void Menu::setGui()
{
	// On vide la gui
	std::vector<std::string> conts = m_gui.listContainers();
	for(size_t i = 0; i < conts.size(); ++i)
		m_gui.deleteContainer(conts[i]);

	// On rédéfinit la gui
	m_gui.setDefaultFont( graphics::Theme::menu );

	sdl::AABB size = core::cfg->size();
	m_gui.addContainer("menu");
	m_gui.setCurrent("menu");
	m_gui->setDimension( gcn::Rectangle(size->w / 2 - 200, size->h / 2 - 150, 400, 300) );

	load->adjustSize();
	load->setPosition(m_gui->getWidth() / 2 - load->getWidth() / 2, m_gui->getHeight() / 2 - load->getHeight() - 60);
	ngame->adjustSize();
	ngame->setPosition(m_gui->getWidth() / 2 - ngame->getWidth() / 2, m_gui->getHeight() / 2 - ngame->getHeight() / 2);
	opts->adjustSize();
	opts->setPosition(m_gui->getWidth() / 2 - opts->getWidth() / 2, m_gui->getHeight() / 2 + 80);

	m_gui->add(load);
	m_gui->add(ngame);
	m_gui->add(opts);
}




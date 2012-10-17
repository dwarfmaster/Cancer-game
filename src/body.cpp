
#include "body.hpp"

#include <SDL/SDL.h>
#include <SDLP_tools.hpp>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include "i18n.hpp"
#include "core/exception.hpp"
#include "core/sounds.hpp"
#include "core/config.hpp"

#include <sstream>
#include <map>
#include <iostream> // TODO supprimer

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>

#include "graphics/theme.hpp"
#include "graphics/gui.hpp"
#include "graphics/deleterContainer.hpp"
#include "graphics/button.hpp"

namespace fs = boost::filesystem;

	Body::Body()
: m_selected(ALLORGS), m_veselected(NULL), m_corpse(NULL), 
	m_top(NULL),
	m_gvessels(NULL), m_gvessrc(NULL), m_gvessrct(NULL), m_gvesdest(NULL), m_gvesdestt(NULL), m_gvesthrow(NULL),
	m_gorgans(NULL), m_gorgtitle(NULL), m_gorgname(NULL), m_gorgst(NULL), m_gorgstate(NULL), m_gorgenter(NULL),
	ecran(SDL_GetVideoSurface())
{
	if( ecran == NULL )
	{
		std::ostringstream oss;
		oss << _i("Error while getting the video surface : \"") << SDL_GetError() << _i("\"");
		throw core::Exception( oss.str() );
	}

	load();
}

Body::~Body()
{
	for(size_t i = 0; i < m_orgs.size(); ++i)
	{
		if( m_orgs[i].healthy != NULL )
			SDL_FreeSurface(m_orgs[i].healthy);
		if( m_orgs[i].sick != NULL )
			SDL_FreeSurface(m_orgs[i].sick);
		if( m_orgs[i].control != NULL )
			SDL_FreeSurface(m_orgs[i].control);
	}

	for(size_t i = 0; i < m_vessels.size(); ++i)
	{
		if( m_vessels[i].picture != NULL )
			SDL_FreeSurface(m_vessels[i].picture);
	}

	if( m_corpse != NULL )
		SDL_FreeSurface(m_corpse);

	if( m_top != NULL )
		delete m_top;
	if( m_gvessels != NULL )
		delete m_gvessels;
	if( m_gorgans != NULL )
		delete m_gorgans;
}

void Body::run()
{
}

SDL_Surface* Body::loadPicture(const boost::filesystem::path& path, double fact)
{
	SDL_Surface* pict = IMG_Load( path.string().c_str() );
	if( pict == NULL )
	{
		std::ostringstream oss;
		oss << _i("Error while loading the picture ") << path.string() << _i(":") << IMG_GetError();
		throw core::Exception( oss.str() );
	}

	SDL_Surface* zoom = zoomSurface(pict, fact, fact, SMOOTHING_ON);
	if( zoom == NULL )
	{
		std::ostringstream oss;
		oss << _i("Error while resizing the picture ") << path.string();
		throw core::Exception( oss.str() );
	}
	SDL_FreeSurface(pict);

	pict = SDL_DisplayFormatAlpha(zoom);
	if( pict == NULL )
	{
		std::ostringstream oss;
		oss << _i("Error while adapting the picture ") << path.string() << _i(":") << SDL_GetError();
		throw core::Exception( oss.str() );
	}
	SDL_FreeSurface(zoom);

	return pict;
}

Body::Organ Body::loadOrgan(const fs::path& dir, double fact)
{
	Organ org;

	// Tests
	if( !fs::exists(dir) )
	{
		std::ostringstream oss;
		// Précédé par un chemin
		oss << dir.string() << _i(" doesn't exist");
		throw core::Exception( oss.str() );
	}
	else if( !fs::is_directory(dir) )
	{
		std::ostringstream oss;
		// Précédé par un chemin
		oss << dir.string() << _i(" is not a directory");
		throw core::Exception( oss.str() );
	}

	// Image healthy
	org.healthy = loadPicture(dir/"healthy.png", fact);

	// Image sick
	try {
		org.sick = loadPicture(dir/"sick.png", fact);
	} catch(core::Exception& e) {
		SDL_FreeSurface(org.healthy);
		throw e;
	}

	// Image control
	try {
		org.control = loadPicture(dir/"control.png", fact);
	} catch(core::Exception& e) {
		SDL_FreeSurface(org.healthy);
		SDL_FreeSurface(org.sick);
		throw e;
	}

	return org;
}

void Body::loadVessels(const fs::path& dir, double fact)
{
	// On liste et charge les fichiers
	for(fs::directory_iterator fit(dir); fit != fs::directory_iterator(); ++fit)
	{
		if( fs::exists(*fit)
				&& !fs::is_directory(*fit)
				&& fs::extension(*fit) == ".vessel" )
			loadVessel(*fit, fact);
	}
}

Body::Vessel Body::loadVessel(const fs::path& path, double fact)
{
	// Tests
	if( !fs::exists(path) )
	{
		std::ostringstream oss;
		// Est précédé d'un chemin
		oss << path.string() << _i(" doesn't exist");
		throw core::Exception( oss.str() );
	}
	else if( fs::is_directory(path) )
	{
		std::ostringstream oss;
		// Est précédé d'un chemin
		oss << path.string() << _i(" is a directory");
		throw core::Exception( oss.str() );
	}

	// Chargement du fichier de description
	fs::ifstream file(path);
	if( !file )
	{
		std::ostringstream oss;
		oss << _i("Can't open the file ") << path;
		throw core::Exception( oss.str() );
	}

	// Lecture du fichier
	std::map<std::string,std::string> map;
	std::string line;
	while( std::getline(file, line) )
	{
		// Suppression des commentaires
		if( line.find('#') != line.npos )
			line.erase(line.find('#'));

		// Vérification qu'il reste quelque chose dans la ligne
		if( line.find_first_not_of(' ') != line.npos )
		{
			// On vérifie la validité de la ligne
			boost::regex reg("[[:word:]]*([[:blank:]]*|)=([[:blank:]]|)*[[:word:]]*([[:space:]]*|)");
			if( regex_match(line, reg) )
			{
				// Position '='
				size_t pos = line.find('=');

				// On récupère la clée
				std::string key = line;
				key = key.substr(0, pos);
				if( key.find('\t') != line.npos
						|| key.find(' ') != line.npos )
					key.erase( key.find_first_of("\t ") );

				// On récupère la valeur
				std::string value = line;
				value = value.substr(pos + 1);
				value.erase(0, value.find_first_not_of("\t "));
				value.erase(value.find_last_not_of("\t ") + 1);

				// On la stocke
				map[key] = value;
			}
		}
	}

	// Analyse des valeurs
	Vessel vessel;

	if( map.find("picture") == map.end() )
	{
		std::ostringstream oss;
		oss << _i("Vessel file ") << path.string() << _i(" has not \"picture\" key.");
		throw core::Exception( oss.str() );
	}
	else
	{
		fs::path path = map["picture"];
		vessel.picture = loadPicture(path.string(), fact);
	}

	if( map.find("origin") == map.end() )
	{
		std::ostringstream oss;
		oss << _i("Vessel file ") << path.string() << _i(" has not \"origin\" key.");
		throw core::Exception( oss.str() );
	}
	else
	{
		vessel.src = strToOrgan( map["origin"] );
		if( vessel.src == ALLORGS )
		{
			std::ostringstream oss;
			oss << _i("Vessel file ") << path.string() << _i(" has a bad \"origin\" value : \"") << map["origin"] << _i("\"");
			throw core::Exception( oss.str() );
		}
	}

	if( map.find("destination") == map.end() )
	{
		std::ostringstream oss;
		oss << _i("Vessel file ") << path.string() << _i(" has not \"destination\" key.");
		throw core::Exception( oss.str() );
	}
	else
	{
		vessel.dest = strToOrgan( map["destination"] );
		if( vessel.dest == ALLORGS )
		{
			std::ostringstream oss;
			oss << _i("Vessel file ") << path.string() << _i(" has a bad \"destination\" value : \"") << map["destination"] << _i("\"");
			throw core::Exception( oss.str() );
		}
	}

	if( map.find("length") == map.end() )
	{
		std::ostringstream oss;
		oss << _i("Vessel file ") << path.string() << _i(" has not \"length\" key.");
		throw core::Exception( oss.str() );
	}
	else
		vessel.size = sdl::atoi( map["length"] );

	file.close();
}

void Body::load()
{
	// On charge l'image du corp
	fs::path rcdir = core::cfg->organs();

	fs::path cpath = rcdir / "corpse.png";
	m_corpse = IMG_Load( cpath.string().c_str() );
	if( m_corpse == NULL )
	{
		std::ostringstream oss;
		oss << _i("Error while loading the picture \"") << cpath.string() << _i("\" : \"") << SDL_GetError() << _i("\"");
		throw core::Exception( oss.str() );
	}

	double f1 = (double)ecran->w / (double)m_corpse->w;
	double f2 = (double)ecran->h / (double)m_corpse->h;
	double fact = (f1 > f2) ? f1 : f2;

	SDL_Surface* zoom = zoomSurface(m_corpse, fact, fact, SMOOTHING_ON);
	if( zoom == NULL )
	{
		std::ostringstream oss;
		oss << _i("Error while resizing the picture ") << cpath.string();
		throw core::Exception( oss.str() );
	}
	SDL_FreeSurface(m_corpse);

	m_corpse = SDL_DisplayFormatAlpha(zoom);
	if( m_corpse == NULL )
	{
		std::ostringstream oss;
		oss << _i("Error while adapting the picture ") << cpath.string() << _i(": \"") << SDL_GetError() << _i("\"");
		throw core::Exception( oss.str() );
	}
	SDL_FreeSurface(zoom);

	// On charge les organes
	for(size_t i = 0; i < ALLORGS; ++i)
	{
		fs::path orgpath = rcdir / organToStr( static_cast<Organs>(i) );
		loadOrgan(orgpath, fact);
	}
	
	// On charge les vaisseaux sanguins
	loadVessels(rcdir / "vessels", fact);
	
	// On charge la GUI
	initGui();
	setGui();
}

void Body::clic(const sdl::Pointsi& pos)
{
}

void Body::draw()
{
}

void Body::selectOrgan(const Organs& org)
{
}

void Body::selectVessel(Vessel* s)
{
}

void Body::setGui()
{
	graphics::Gui& gui = *graphics::gui;

	// On vide la précédente gui
	std::vector<std::string> conts = gui.listContainers();
	for(size_t i = 0; i < conts.size(); ++i)
		gui.deleteContainer(conts[i]);

	// On crée la nouvelle gui
	size_t width, height;
	width = ecran->w / 5;
	height = ecran->h - 20;

	gui.setDefaultFont( graphics::Theme::normal );
	gui.addContainer("body");
	gui.setCurrent("body");
	gui->setDimension( gcn::Rectangle(ecran->w - width - 10, 10, width, height) );

	m_top->setDimension( gcn::Rectangle(0, 0, width, height) );
	gui->add(m_top);

	// On place la gui pour organe
	size_t y = 10;
	m_gorgans->setDimension( gcn::Rectangle(0, 0, width, height) );

	m_gorgtitle->adjustSize();
	m_gorgtitle->setPosition(5, y);
	y += m_gorgtitle->getHeight();
	m_gorgname->adjustSize();
	m_gorgname->setPosition(20, y);
	y += m_gorgname->getHeight();

	y += 10;
	m_gorgst->adjustSize();
	m_gorgst->setPosition(5, y);
	y += m_gorgst->getHeight();
	m_gorgstate->adjustSize();
	m_gorgstate->setPosition(20, y);

	m_gorgenter->adjustSize();
	y = m_top->getHeight() - m_gorgenter->getHeight(); y -= 5;
	m_gorgenter->setPosition( m_top->getWidth() - m_gorgenter->getWidth() - 5, y );

	// On place la gui pour les vaisseaux sanguins
	y = 10;
	m_gvessels->setDimension( gcn::Rectangle(0, 0, width, height) );

	m_gvessrct->adjustSize();
	m_gvessrct->setPosition(5, y);
	y += m_gvessrct->getHeight();
	m_gvessrc->adjustSize();
	m_gvessrc->setPosition(20, y);
	y += m_gvessrc->getHeight();

	y += 10;
	m_gvesdestt->adjustSize();
	m_gvesdestt->setPosition(5, y);
	y += m_gvesdestt->getHeight();
	m_gvesdest->adjustSize();
	m_gvesdest->setPosition(20, y);

	m_gvesthrow->adjustSize();
	y = m_top->getHeight() - m_gvesthrow->getHeight(); y -= 5;
	m_gvesthrow->setPosition( m_top->getWidth() - m_gvesthrow->getWidth() - 5, y );

	// On choisit ce qu'on affiche
	m_top->clear();
	if( m_selected != ALLORGS )
		m_top->add(m_gorgans);
	else if( m_veselected != NULL )
		m_top->add(m_gvessels);
}

void Body::initGui()
{
	graphics::Theme& theme = *graphics::gui->getTheme();

	m_top = new gcn::Container;
	theme.apply(m_top);

	// Création de la gui d'un organe
	m_gorgans = new graphics::DeleterContainer;
	m_gorgtitle = new graphics::Label( _i("Organ :") );
	m_gorgname = new graphics::Label;
	m_gorgst = new graphics::Label( _i("State :") );
	m_gorgstate = new graphics::Label;
	m_gorgenter = new graphics::Button( _i("Enter"), boost::bind(&Body::enter, this));

	m_gorgans->add(m_gorgtitle);
	m_gorgans->add(m_gorgname);
	m_gorgans->add(m_gorgst);
	m_gorgans->add(m_gorgstate);
	m_gorgans->add(m_gorgenter);

	theme.apply(m_gorgans);
	theme.apply(m_gorgtitle);
	theme.apply(m_gorgname);
	theme.apply(m_gorgst);
	theme.apply(m_gorgstate);
	theme.apply(m_gorgenter);

	// Création de la gui d'un vaisseau
	m_gvessels = new graphics::DeleterContainer;
	m_gvessrct = new graphics::Label( _i("Origin :") );
	m_gvessrc = new graphics::Label;
	m_gvesdestt = new graphics::Label( _i("Destination :") );
	m_gvesdest = new graphics::Label;
	m_gvesthrow = new graphics::Button( _i("Throw"), boost::bind(&Body::vesthrow, this));

	m_gvessels->add(m_gvessrct);
	m_gvessels->add(m_gvessrc);
	m_gvessels->add(m_gvesdestt);
	m_gvessels->add(m_gvesdest);
	m_gvessels->add(m_gvesthrow);

	theme.apply(m_gvessrct);
	theme.apply(m_gvessrc);
	theme.apply(m_gvesdestt);
	theme.apply(m_gvesdest);
	theme.apply(m_gvesthrow);
}

Body::Organs Body::strToOrgan(const std::string& s)
{
	Organs ret;

	if( s == "brain" )
		ret = brain;
	else if( s == "heart" )
		ret = heart;
	else if( s == "lung" )
		ret = lung;
	else if( s == "liver" )
		ret = liver;
	else if( s == "kidney" )
		ret = kidney;
	else if( s == "digestive" )
		ret = digestive;
	else
		ret = ALLORGS;

	return ret;
}

std::string Body::organToStr(const Organs& org)
{
	switch(org)
	{
		case brain:
			return "brain";
		case heart:
			return "heart";
		case lung:
			return "lung";
		case liver:
			return "liver";
		case kidney:
			return "kidney";
		case digestive:
			return "digestive";
		case ALLORGS:
			return "alls";
		default:
			return "error";
	}
}

void Body::enter()
{
	core::sounds->playSound( core::Sounds::ok );
	// TODO
	std::cout << "Enter !!" << std::endl;
}

void Body::vesthrow()
{
	core::sounds->playSound( core::Sounds::ok );
	// TODO
	std::cout << "Vesthrow called !!" << std::endl;
}



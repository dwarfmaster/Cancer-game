
#include "body.hpp"

#include <SDL/SDL.h>
#include <SDLP_tools.hpp>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include "i18n.hpp"
#include "core/exception.hpp"
#include <sstream>
#include <map>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/regex.hpp>

namespace fs = boost::filesystem;

	Body::Body()
: m_selected(ALLORGS), m_corpse(NULL), ecran(SDL_GetVideoSurface())
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
}

void Body::clic(const sdl::Pointsi& pos)
{
}

void Body::draw()
{
}

void Body::setGui()
{
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


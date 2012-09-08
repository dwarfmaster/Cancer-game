
#include "config.hpp"
#include "../i18n.hpp"

#include <SDLP_tools.hpp>
#include <string>
#include <boost/filesystem/fstream.hpp>

namespace opt = boost::program_options;
typedef boost::filesystem::path path_t;

namespace core
{
	const char* default_path_sounds = "/usr/share/cancer-game/default.muscancer";
	const char* default_config_path = "/usr/share/cancer-game/conf";

	Config::Config()
		: m_opts(_i("Allowed options"))
	{
		setOpts();
	}

	Config::Config(int argc, char *argv[])
	{
		setOpts();
		load(argc, argv);
	}

	void Config::load(int argc, char *argv[])
	{
		// Lecture de la ligne de commande
		opt::parsed_options parsed = opt::command_line_parser(argc, argv)
			.options(m_opts)
			.run();
		opt::store(parsed, m_vm);
		opt::notify(m_vm);

		// Lecture du fichier de conf
		path_t confPath;
		if( m_vm.count("config") )
			confPath = m_vm["config"].as<path_t>();
		else
			confPath = default_config_path;

		boost::filesystem::ifstream file(confPath);
		if(file)
		{
			opt::store( opt::parse_config_file(file, m_opts), m_vm );
			opt::notify(m_vm);
		}
	}

	sdl::AABB Config::size() const
	{
		if( m_vm.count("size") )
			return parseSize( m_vm["size"].as<std::string>() );
		else
			return maxSize();
	}

	bool Config::fullscreen() const
	{
		return m_vm.count("fullscreen");
	}

	path_t Config::sounds() const
	{
		if( m_vm.count("sounds") )
			return m_vm["sounds"].as<path_t>();
		else
			return default_path_sounds;
	}

	void Config::setOpts()
	{
		std::string configDesc( _i("Precise a configuration file (default = ") );
		configDesc += default_config_path;
		configDesc += ").";

		m_opts.add_options()
			("size,s", opt::value<std::string>()->composing(), _i("Set the size of the window, like '800x600'."))
			("fullscreen,f", _i("Launch the program in fullscreen mode."))
			("sounds,S", opt::value<path_t>(), _i("Set the path the a sounds theme (file with .muscancer extension)."))
			("config,c", opt::value<path_t>(), configDesc.c_str() )
			;
	}

	sdl::AABB Config::maxSize() const
	{
		if( !SDL_WasInit(SDL_INIT_VIDEO) )
			return sdl::makeRect(0, 0, 1600, 900);

		SDL_Rect** modes = SDL_ListModes(NULL, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

		if( modes == (SDL_Rect**)0 )
			return sdl::makeRect(0, 0, 0, 0);
		else if( modes == (SDL_Rect**)-1 )
			return sdl::makeRect(0, 0, 1600, 900);
		else
			return *modes[0];
	}

	sdl::AABB Config::parseSize(const std::string& size) const
	{
		std::string tmp(""); // sert à stocker la string d'un nombre
		bool cont = true, first = true;
		SDL_Rect rect;
		rect.x = rect.y = 0;
		rect.w = 800;
		rect.h = 600;
		for(size_t k = 0; cont; ++k)
		{
			if(k >= size.size())
			{
				unsigned int nb = 0;
				nb = sdl::atoi(tmp);
				if(first)
					rect.w = nb;
				else
					rect.h = nb;
				cont = false;
			}
			else if(size[k] >= '0' 
					&& size[k] <= '9')
				tmp += size[k];
			else if(size[k] == '*' 
					|| size[k] == 'x')
			{
				unsigned int nb = 0;
				nb = sdl::atoi(tmp);
				if(first)
				{
					rect.w = nb;
					tmp = "";
					first = false;
				}
				else
				{
					rect.h = nb;
					cont = false;
				}
			}
			else // Peu probable
			{
				unsigned int nb = 0;
				std::istringstream(tmp) >> nb;
				if(first)
					rect.w = nb;
				else
					rect.h = nb;
				cont=false;
			}
		}

		if( rect.w < 800 )
			rect.w = 800;
		if( rect.h < 600 )
			rect.h = 600;

		return rect;
	}

};//namespace core


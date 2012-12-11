
#ifndef DEF_CORE_CONFIG
#define DEF_CORE_CONFIG

#include <SDLP_position.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>
#include <cstdlib>

class ConfigEditor;

namespace core
{
	class Config
	{
		friend ConfigEditor;

		public:
			Config();
			Config(int argc, char *argv[]);
			~Config();

			void load(int argc, char *argv[]);

			sdl::AABB size() const;
			bool fullscreen() const;
			boost::filesystem::path sounds() const;
			boost::filesystem::path gtheme() const;
			boost::filesystem::path gamedir() const;
			unsigned short int volume(bool sounds = false) const;

			boost::filesystem::path getPath(boost::filesystem::path end) const;
		private:
			boost::program_options::options_description m_opts;
			boost::program_options::options_description m_desc;

			boost::filesystem::path m_home;
			boost::filesystem::path m_config; // Chemin vers le fichier de conf

			unsigned short int m_music, m_sounds;

			boost::filesystem::path m_dsounds;
			boost::filesystem::path m_gtheme;
			boost::filesystem::path m_gamedir;

			sdl::AABB m_size;
			bool m_fullscreen;

			void setOpts();
			void processOpts(const boost::program_options::variables_map& vm);
			sdl::AABB maxSize() const;
			sdl::AABB parseSize(const std::string& size) const;

			Config(const Config& cp);
			Config& operator=(const Config& cp);
	};//class Config

	extern Config* cfg;
};//namespace core

#endif//DEF_CORE_CONFIG


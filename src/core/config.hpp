
#ifndef DEF_CORE_CONFIG
#define DEF_CORE_CONFIG

#include <SDLP_position.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

namespace core
{
	extern const char* default_path_sounds;
	extern const char* default_config_path;
	extern const char* default_path_gtheme;

	class Config
	{
		public:
			Config();
			Config(int argc, char *argv[]);

			void load(int argc, char *argv[]);

			sdl::AABB size() const;
			bool fullscreen() const;
			boost::filesystem::path sounds() const;
			boost::filesystem::path gtheme() const;

		private:
			boost::program_options::variables_map m_vm;
			boost::program_options::options_description m_opts;

			void setOpts();
			sdl::AABB maxSize() const;
			sdl::AABB parseSize(const std::string& size) const;

			Config(const Config& cp);
			Config& operator=(const Config& cp);
	};//class Config
};//namespace core

#endif//DEF_CORE_CONFIG


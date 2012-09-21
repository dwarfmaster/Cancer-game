
#ifndef DEF_GRAPHICS_ICON
#define DEF_GRAPHICS_ICON

#include <guichan.hpp>
#include <string>
#include <boost/filesystem/path.hpp>

class SDL_Surface;

namespace graphics
{
	class Icon : public gcn::Icon
	{
		public:
			Icon();
			Icon(SDL_Surface* surf);
			Icon(const boost::filesystem::path& path);

			void setImage(SDL_Surface* surf);

	};//class Icon
};//namespace graphics

#endif//DEF_GRAPHICS_ICON


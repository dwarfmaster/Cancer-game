
#ifndef DEF_GRAPHICS_ANIM
#define DEF_GRAPHICS_ANIM

#include <vector>
#include <cstdint>

struct SDL_Surface;
typedef uint32_t Uint32;
namespace boost
{
	namespace filesystem
	{
		class path;
	}
}
using std::size_t;

namespace graphics
{
	class Anim
	{
		public:
			Anim();
			Anim(const Anim& cp);
			Anim(const boost::filesystem::path& path);
			~Anim();

			Anim& operator=(const Anim& cp);
			void set(const Anim& cp);
			void load(const boost::filesystem::path& path); // Lance une exception en cas d'erreur
			void clear();

			void play();
			void pause();
			void toggle();

			SDL_Surface* update();
			SDL_Surface* getActSurf();

			void forward(Uint32 time);
			void backward(Uint32 time);
			void next();
			void prev();

			bool isLoaded() const;

		private:
			std::vector<SDL_Surface*> m_frames;
			size_t* m_count; // Nombre d'animation utilisant ces images
			Uint32 m_time;

			size_t m_act;
			bool m_paused;
			Uint32 m_lastTime;
			Uint32 m_timeStay;

	};//class Anim
};//namespace graphics

#endif//DEF_GRAPHICS_ANIM

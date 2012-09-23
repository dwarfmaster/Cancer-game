
#ifndef DEF_CORE_SOUNDS
#define DEF_CORE_SOUNDS

/*
ok : Son est joué lors des validations.
cancel : Son joué lors des annulations.
select : Son joué lors d'une sélection.
wincel : Son joué lors de la mutation d'une cellule.
winorg : Son joué lors de l'obtention du contrôle d'un organe.
alert : Son joué lors d'un problème.
hibern : Son joué lors de la mise en hibernation d'une cellule.
*/

#include <boost/array.hpp>

struct Mix_Chunk;
struct _Mix_Music;
typedef struct _Mix_Music Mix_Music;
namespace boost
{
	namespace filesystem
	{
		class path;
	};
};

namespace core
{
	class Sounds
	{
		public:
			enum Sound{ok, cancel, select, wincel, winorg, alert, hibern, ALL_SOUNDS}; // ALL_SOUNDS sert juste pour un usage interne
			enum Music{menu, game, ALL_MUSICS}; // ALL_MUSIC sert juste pour un usge interne

			Sounds(const boost::filesystem::path& sounds);
			~Sounds();

			void playMusic(const Music& mus);
			void playSound(const Sound& sound);
			void setVolume(unsigned char nvol); // 255 = max et 0 = mute : La fonction convertit toute seule vers les valeurs de SDL_Mixer

			void pause();
			void unpause();
			void togglepause();

			void mute();
			void unmute();
			void toggleMute();

			bool isPaused() const;
			bool isMute() const;
			unsigned char volume() const;

		private:
			struct SSound
			{
				size_t channel;
				Mix_Chunk* sound;
			};
			boost::array<SSound, ALL_SOUNDS> m_sounds;
			boost::array<Mix_Music*, ALL_MUSICS> m_musics;

			bool m_paused;
			bool m_mute;
			unsigned char m_volume;
			size_t m_nchannels;

			void load(const boost::filesystem::path& sounds);
			int mixVol(unsigned char vol) const;
			void clear();

			Sounds();
			Sounds(const Sounds&);
			Sounds& operator=(const Sounds&);
	};//class Sounds

	extern Sounds* sounds;
};//namespace core

#endif//DEF_CORE_SOUNDS


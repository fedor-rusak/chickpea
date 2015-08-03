#if defined(_MSC_VER)
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep(x)
#endif

#include <AL/alut.h>

#include <engine/openal_wrapper.hpp>

namespace openal {

	void testAlut(char* fileName) {
		//> Sound buffer variable
		ALuint buffer = 0;

		//> Sound source varialbe
		ALuint source = 0;

		//> Initialize ALUT
		alutInit(NULL, NULL);

		//> Generate sound buffer
		alGenBuffers(NUM_BUFFERS, &buffer);

		//> Load WAV file
		buffer = alutCreateBufferFromFile(fileName);

		//> Generate sound source (sound position in 3D space)
		alGenSources(NUM_SOURCES, &source);

		//> Associate source with sound buffer
		alSourcei(source, AL_BUFFER, buffer);

		//> Play the sound
		alSourcePlay(source);
		// std::cout << "sound async" << std::endl;
		//> Wait for playing sound
		alutSleep(1);

		//> Exit from ALUT
		alutExit();
	}

}
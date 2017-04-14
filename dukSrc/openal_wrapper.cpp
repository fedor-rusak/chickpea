#include <iostream>

#include <AL/alut.h>

namespace openal {

	const unsigned int NUM_BUFFERS = 1;
	const unsigned int NUM_SOURCES = 1;

	static ALuint source = 0;


	bool init() {
		return alutInit(NULL, NULL);
	}

	void load(char* fileName) {
		//> Sound buffer variable
		ALuint buffer = 0;

		//> Sound source varialbe
		source = 0;

		//> Generate sound buffer
		alGenBuffers(NUM_BUFFERS, &buffer);

		//> Load WAV file
		buffer = alutCreateBufferFromFile(fileName);

		//> Generate sound source (sound position in 3D space)
		alGenSources(NUM_SOURCES, &source);

		//> Associate source with sound buffer
		alSourcei(source, AL_BUFFER, buffer);
	}

	void play() {
		alSourcePlay(source);
	}

	void terminate() {
		alutExit();
	}

}
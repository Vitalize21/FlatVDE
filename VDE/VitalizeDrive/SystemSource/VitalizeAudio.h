#pragma once

#include "VitalizeSystem.h"

#include "../External/ogg/OggDecoder.h"
#include "../External/ogg/OggVorbisMemory.h"
#include "../External/ogg/OggVorbisFile.h"
#include "../External/ogg/PCMPlayer.h"
#include "../External/ogg/DixSmartPtr.h"
#include "../External/ogg/DixComPtr.h"
#include <dsound.h>

#pragma region _AudioSystem

class VitalizeAudioSystem final
{
public:
	static void Initialize();
	static void Release();

	//DirectSound
	static Dix::Com_ptr< IDirectSound8 > directSoundUnit;

private:
	//new ‚³‚¹‚È‚¢
	VitalizeAudioSystem() {}
	~VitalizeAudioSystem() {}

};

#pragma endregion

class VitalizeAudio
{
public:
	VitalizeAudio(const char* fName);
	~VitalizeAudio();

	//Ä¶
	void play(bool isLoop);
	//’â~
	void stop();
	//ˆê’â~iÄŠJ‚É‚ÍresumeŠÖ”‚ğIj
	void pause();
	//ˆê’â~‚©‚çÄŠJ
	void resume();
	//‰¹—Êİ’è(0.0f`1.0f)
	void setVolume(float v);

private:
	VitalizeAudio() : player(nullptr) {}

	Dix::sp< Dix::OggVorbisMemory > spOggResource;
	//	Dix::sp< Dix::OggVorbisFile > spOggResource( new Dix::OggVorbisFile( "test.ogg" ) );
	Dix::sp< Dix::OggDecoder > spOggDecoder;
	Dix::PCMPlayer* player;

};


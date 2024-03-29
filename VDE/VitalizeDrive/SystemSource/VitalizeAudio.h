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
	//new させない
	VitalizeAudioSystem() {}
	~VitalizeAudioSystem() {}

};

#pragma endregion

class VitalizeAudio
{
public:
	VitalizeAudio(const char* fName);
	~VitalizeAudio();

	//再生
	void play(bool isLoop);
	//停止
	void stop();
	//一時停止（再開にはresume関数を！）
	void pause();
	//一時停止から再開
	void resume();
	//音量設定(0.0f〜1.0f)
	void setVolume(float v);

private:
	VitalizeAudio() : player(nullptr) {}

	Dix::sp< Dix::OggVorbisMemory > spOggResource;
	//	Dix::sp< Dix::OggVorbisFile > spOggResource( new Dix::OggVorbisFile( "test.ogg" ) );
	Dix::sp< Dix::OggDecoder > spOggDecoder;
	Dix::PCMPlayer* player;

};


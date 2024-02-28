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
	//new �����Ȃ�
	VitalizeAudioSystem() {}
	~VitalizeAudioSystem() {}

};

#pragma endregion

class VitalizeAudio
{
public:
	VitalizeAudio(const char* fName);
	~VitalizeAudio();

	//�Đ�
	void play(bool isLoop);
	//��~
	void stop();
	//�ꎞ��~�i�ĊJ�ɂ�resume�֐����I�j
	void pause();
	//�ꎞ��~����ĊJ
	void resume();
	//���ʐݒ�(0.0f�`1.0f)
	void setVolume(float v);

private:
	VitalizeAudio() : player(nullptr) {}

	Dix::sp< Dix::OggVorbisMemory > spOggResource;
	//	Dix::sp< Dix::OggVorbisFile > spOggResource( new Dix::OggVorbisFile( "test.ogg" ) );
	Dix::sp< Dix::OggDecoder > spOggDecoder;
	Dix::PCMPlayer* player;

};


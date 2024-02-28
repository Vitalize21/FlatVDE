// PCMPlayer.cpp
//

#include "PCMPlayer.h"
#include <process.h>
#include <stdio.h>
#include <thread>

#include "VitalizeFramework.h"

namespace {
	unsigned int playTime_g = 1;	// 1 sec.
}

namespace Dix {
	PCMPlayer::PCMPlayer() :
		isReady_		( false ),
		threadHandle_	( 0 ),
		isTerminate_	( false ),
		isLoop_			( true ),
		state_			( PCMPlayer::STATE::STATE_NONE )
	{
		clear();
	}

	PCMPlayer::PCMPlayer( Com_ptr< IDirectSound8 > cpDS8 ) :
		cpDS8_			( cpDS8 ),
		isReady_		( false ),
		threadHandle_	( 0 ),
		isTerminate_	( false ),
		isLoop_			( true ),
		state_			( PCMPlayer::STATE::STATE_NONE )
	{
		clear();
	}

	PCMPlayer::PCMPlayer( Com_ptr< IDirectSound8 > cpDS8, sp< PCMDecoder > spDecoder ) :
		cpDS8_			( cpDS8 ),
		isReady_		( false ),
		threadHandle_	( 0 ),
		isTerminate_	( false ),
		isLoop_			( true ),
		state_			( PCMPlayer::STATE::STATE_NONE )
	{
		clear();
		setDecoder( spDecoder );
	}

	PCMPlayer::~PCMPlayer() {
		terminateThread();
	}

	//! �N���A
	void PCMPlayer::clear() {
		terminateThread();
		memset( &DSBufferDesc_, 0, sizeof( DSBufferDesc_ ) );
		memset( &waveFormat_, 0, sizeof( waveFormat_ ) );
		cpDSBuffer_ = 0;
		isReady_ = false;
		state_ = PCMPlayer::STATE::STATE_NONE;
	}

	//! �Đ����̃X���b�h���~
	void PCMPlayer::terminateThread() {
		isTerminate_ = true;
		if ( threadHandle_ != 0 ) {
			bool end = false;
			while( !end ) {
				DWORD flag = WaitForSingleObject( (HANDLE)(__int64)threadHandle_, 100 );
				switch( flag ) {
				case WAIT_OBJECT_0:
					// �X���b�h���I�����
					end = true;
					break;
				case WAIT_TIMEOUT:
					// �܂��I�����Ă��Ȃ��̂őҋ@
					break;
				case WAIT_FAILED:
					// ���s���Ă���悤�ł�
					end = true;
					break;
				}
			}
		}
		isTerminate_ = false;
		threadHandle_ = 0;
	}

	//! �f�o�C�X�ݒ�
	void PCMPlayer::setDevice( Com_ptr< IDirectSound8 > cpDS8 ) {
		cpDS8_ = cpDS8;
	}

	//! PCM�f�R�[�_��ݒ�
	bool PCMPlayer::setDecoder( sp< PCMDecoder > pcmDecoder ) {
		if ( cpDS8_.GetPtr() == 0 || pcmDecoder.GetPtr() == 0 || pcmDecoder->isReady() == false ) {
			isReady_ = false;
			return false;
		}

		state_ = PCMPlayer::STATE::STATE_STOP;

		if ( !pcmDecoder->getWaveFormatEx( waveFormat_ ) ) {
			return false;
		}

		DSBufferDesc_.dwSize = sizeof( DSBUFFERDESC );
		DSBufferDesc_.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY;
		DSBufferDesc_.dwBufferBytes = waveFormat_.nAvgBytesPerSec * playTime_g;
		DSBufferDesc_.dwReserved = 0;
		DSBufferDesc_.lpwfxFormat = &waveFormat_;
		DSBufferDesc_.guid3DAlgorithm = GUID_NULL;

		// �N���[����ۑ�
		spPCMDecoder_ = pcmDecoder->createClone();

		// �Z�J���_���o�b�t�@���܂������ꍇ�͍쐬
		if ( cpDSBuffer_.GetPtr() == 0 ) {
			IDirectSoundBuffer*	 ptmpBuf = 0;
			if ( SUCCEEDED( cpDS8_->CreateSoundBuffer( &DSBufferDesc_, &ptmpBuf, NULL ) ) ) {
				ptmpBuf->QueryInterface( IID_IDirectSoundBuffer8 , (void**)cpDSBuffer_.ToCreator() );
			}
			else {
				clear();
				return false;
			}
			ptmpBuf->Release();
		}

		// �o�b�t�@��������
		if ( initializeBuffer() == false ) {
			return false;
		}

		// �o�b�t�@�R�s�[�X���b�h����
		if ( threadHandle_ == 0 ) {
			threadHandle_ = (unsigned int)_beginthread( PCMPlayer::streamThread, 0, (void*)this );
		}

		isReady_ = true;

		return true;
	}

	//! �o�b�t�@������������
	bool PCMPlayer::initializeBuffer() {
		if ( spPCMDecoder_.GetPtr() == 0 ) {
			return false;
		}

		spPCMDecoder_->setHead();	// ���o��
		cpDSBuffer_->SetCurrentPosition( 0 );

		// �o�b�t�@�����b�N���ď����f�[�^��������
		void* AP1 = 0, *AP2 = 0;
		DWORD AB1 = 0, AB2  = 0;
		if ( SUCCEEDED( cpDSBuffer_->Lock( 0, 0, &AP1, &AB1, &AP2, &AB2, DSBLOCK_ENTIREBUFFER ) ) ) {
			spPCMDecoder_->getSegment( (char*)AP1, AB1, 0, 0 );
			cpDSBuffer_->Unlock( AP1, AB1, AP2, AB2 );
		}
		else {
			clear();
			return false;
		}

		return true;
	}

	//! �X�g���[���Đ��X���b�h����
	void PCMPlayer::streamThread( void* playerPtr ) {
		PCMPlayer* player = (PCMPlayer*)playerPtr;
		unsigned int size = player->DSBufferDesc_.dwBufferBytes / 2;
		unsigned int flag = 0;
		DWORD point = 0;
		void* AP1 = 0, *AP2 = 0;
		DWORD AB1 = 0, AB2  = 0;

		DWORD finishPos = 0;
		DWORD prePlayPos = 0;

		unsigned int writeSize = 0;
		bool isEnd = false;
		bool waitFinish = false;

#if DEVELOP_DEBUG_PC
		bool vitalizeScenePause = !RuntimeSystem::isMainSceneUpdate();
		bool vitalizeScenePrevFlag = vitalizeScenePause;
#endif

		while( player->isTerminate_ == false ) 
		{
#if DEVELOP_DEBUG_PC
			vitalizeScenePrevFlag = vitalizeScenePause;
			vitalizeScenePause = !RuntimeSystem::isMainSceneUpdate();
			//V�V�[�����s���͉����ꎞ��~���邽�߂̏���
			if (vitalizeScenePrevFlag != vitalizeScenePause)
			{
				if (vitalizeScenePause)
				{
					player->pause();
				}
				else
				{
					player->resume();
				}
			}
#endif

			switch ( player->getState() ) {
			case PCMPlayer::STATE::STATE_PLAY:	// �Đ���
				// �X�g���[���Đ�
				// ���݈ʒu���`�F�b�N
				player->cpDSBuffer_->GetCurrentPosition( &point, 0 );
				if ( flag == 0 && point >= size ) {
					// �O���ɏ�������
					if ( SUCCEEDED( player->cpDSBuffer_->Lock( 0, size, &AP1, &AB1, &AP2, &AB2, 0 ) ) ) {
						player->spPCMDecoder_->getSegment( (char*)AP1, AB1, &writeSize, &isEnd );
						player->cpDSBuffer_->Unlock( AP1, AB1, AP2, AB2 );
						flag = 1;
					}

					// �ŏI�������݂̏ꍇ�͏I���ʒu�����
					if ( isEnd == true && waitFinish == false ) {
						finishPos = writeSize;
						player->cpDSBuffer_->GetCurrentPosition( &prePlayPos, 0 );
						waitFinish = true;
					}
				}
				else if ( flag == 1 && point < size ) {
					// �㔼�ɏ�������
					if ( SUCCEEDED( player->cpDSBuffer_->Lock( size, size * 2, &AP1, &AB1, &AP2, &AB2, 0 ) ) ) {
						player->spPCMDecoder_->getSegment( (char*)AP1, AB1, &writeSize, &isEnd );
						player->cpDSBuffer_->Unlock( AP1, AB1, AP2, AB2 );
						flag = 0;
					}

					// �ŏI�������݂̏ꍇ��Norify��ݒ�
					if ( isEnd == true && waitFinish == false ) {
						finishPos = size + writeSize;
						player->cpDSBuffer_->GetCurrentPosition( &prePlayPos, 0 );
						waitFinish = true;
					}
				}
				break;

			case PCMPlayer::STATE::STATE_STOP:
				flag = 0;	// �~�߂�ƑO���������݂���n�܂邽��
				isEnd = false;
				finishPos = 0;
				prePlayPos = 0;
				break;

			case PCMPlayer::STATE::STATE_PAUSE:
				break;

			default:
				break;
			}

			// �I���ʒu����`�F�b�N
			if ( isEnd == true ) {
				DWORD curPlayPos;
				player->cpDSBuffer_->GetCurrentPosition( &curPlayPos, 0 );
				if ( curPlayPos < prePlayPos ) {
					// �o�b�t�@�����[�v�����u��
					if ( prePlayPos <= finishPos || finishPos <= curPlayPos ) {
						// �I���錾
						player->stop();
					}
				} else {
					if ( prePlayPos <= finishPos && finishPos <= curPlayPos ) {
						// �I���錾
						player->stop();
					}
				}
				prePlayPos = curPlayPos;
			}

			//Sleep( 100 );
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}

	//! �Đ�
	bool PCMPlayer::play( bool isLoop ) {
		if ( isReady() == false ) {
			return false;
		}
		isLoop_ = isLoop;
#if DEVELOP_DEBUG_PC
		if (!RuntimeSystem::isMainSceneUpdate())
		{
			state_ = PCMPlayer::STATE::STATE_PAUSE;
			return true;
		}
#endif
		// �Đ�
		spPCMDecoder_->setLoop( isLoop );
		cpDSBuffer_->Play( 0, 0, DSBPLAY_LOOPING );
		state_ = PCMPlayer::STATE::STATE_PLAY;
		return true;
	}

	//! �ꎞ��~
	void PCMPlayer::pause() {
		if ( state_ != PCMPlayer::STATE::STATE_PLAY ) 
		{
			return;
		}
		// �����Ă�����~�߂�
		cpDSBuffer_->Stop();
		state_ = PCMPlayer::STATE::STATE_PAUSE;
	}

	//! �ĊJ
	void PCMPlayer::resume() {
		if (state_ != PCMPlayer::STATE::STATE_PAUSE) 
		{
			return;
		}
		// �~�܂��Ă�����Đ�
		play(isLoop_);
	}

	//! ��~
	void PCMPlayer::stop() {
		if ( isReady() == false ) {
			return;
		}
		state_ = PCMPlayer::STATE::STATE_STOP;
		cpDSBuffer_->Stop();

		// �o�b�t�@�̓��o��
		initializeBuffer();
	}

	//! ���ʂ�ς���
	void PCMPlayer::setVolume( int volume ) {
		if ( isReady() == true ) {
			cpDSBuffer_->SetVolume( volume );
		}
	}

	//! �p���̈ʒu��ς���
	void PCMPlayer::setPan( int pan ) {
		if ( isReady() == true ) {
			cpDSBuffer_->SetPan( pan );
		}
	}

	//! �����ł����H
	bool PCMPlayer::isReady() {
		return isReady_;
	}

	//! �Đ����H
	bool PCMPlayer::isPlaying() {
		return ( state_ == PCMPlayer::STATE::STATE_PLAY );
	}

	//! ��Ԃ��擾
	PCMPlayer::STATE PCMPlayer::getState() {
		return state_;
	}

}
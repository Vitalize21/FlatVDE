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

	//! クリア
	void PCMPlayer::clear() {
		terminateThread();
		memset( &DSBufferDesc_, 0, sizeof( DSBufferDesc_ ) );
		memset( &waveFormat_, 0, sizeof( waveFormat_ ) );
		cpDSBuffer_ = 0;
		isReady_ = false;
		state_ = PCMPlayer::STATE::STATE_NONE;
	}

	//! 再生中のスレッドを停止
	void PCMPlayer::terminateThread() {
		isTerminate_ = true;
		if ( threadHandle_ != 0 ) {
			bool end = false;
			while( !end ) {
				DWORD flag = WaitForSingleObject( (HANDLE)(__int64)threadHandle_, 100 );
				switch( flag ) {
				case WAIT_OBJECT_0:
					// スレッドが終わった
					end = true;
					break;
				case WAIT_TIMEOUT:
					// まだ終了していないので待機
					break;
				case WAIT_FAILED:
					// 失敗しているようです
					end = true;
					break;
				}
			}
		}
		isTerminate_ = false;
		threadHandle_ = 0;
	}

	//! デバイス設定
	void PCMPlayer::setDevice( Com_ptr< IDirectSound8 > cpDS8 ) {
		cpDS8_ = cpDS8;
	}

	//! PCMデコーダを設定
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

		// クローンを保存
		spPCMDecoder_ = pcmDecoder->createClone();

		// セカンダリバッファがまだ無い場合は作成
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

		// バッファを初期化
		if ( initializeBuffer() == false ) {
			return false;
		}

		// バッファコピースレッド生成
		if ( threadHandle_ == 0 ) {
			threadHandle_ = (unsigned int)_beginthread( PCMPlayer::streamThread, 0, (void*)this );
		}

		isReady_ = true;

		return true;
	}

	//! バッファを初期化する
	bool PCMPlayer::initializeBuffer() {
		if ( spPCMDecoder_.GetPtr() == 0 ) {
			return false;
		}

		spPCMDecoder_->setHead();	// 頭出し
		cpDSBuffer_->SetCurrentPosition( 0 );

		// バッファをロックして初期データ書き込み
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

	//! ストリーム再生スレッド生成
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
			//Vシーン実行時は音声一時停止するための処理
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
			case PCMPlayer::STATE::STATE_PLAY:	// 再生中
				// ストリーム再生
				// 現在位置をチェック
				player->cpDSBuffer_->GetCurrentPosition( &point, 0 );
				if ( flag == 0 && point >= size ) {
					// 前半に書き込み
					if ( SUCCEEDED( player->cpDSBuffer_->Lock( 0, size, &AP1, &AB1, &AP2, &AB2, 0 ) ) ) {
						player->spPCMDecoder_->getSegment( (char*)AP1, AB1, &writeSize, &isEnd );
						player->cpDSBuffer_->Unlock( AP1, AB1, AP2, AB2 );
						flag = 1;
					}

					// 最終書き込みの場合は終了位置を特定
					if ( isEnd == true && waitFinish == false ) {
						finishPos = writeSize;
						player->cpDSBuffer_->GetCurrentPosition( &prePlayPos, 0 );
						waitFinish = true;
					}
				}
				else if ( flag == 1 && point < size ) {
					// 後半に書き込み
					if ( SUCCEEDED( player->cpDSBuffer_->Lock( size, size * 2, &AP1, &AB1, &AP2, &AB2, 0 ) ) ) {
						player->spPCMDecoder_->getSegment( (char*)AP1, AB1, &writeSize, &isEnd );
						player->cpDSBuffer_->Unlock( AP1, AB1, AP2, AB2 );
						flag = 0;
					}

					// 最終書き込みの場合はNorifyを設定
					if ( isEnd == true && waitFinish == false ) {
						finishPos = size + writeSize;
						player->cpDSBuffer_->GetCurrentPosition( &prePlayPos, 0 );
						waitFinish = true;
					}
				}
				break;

			case PCMPlayer::STATE::STATE_STOP:
				flag = 0;	// 止めると前半書き込みから始まるため
				isEnd = false;
				finishPos = 0;
				prePlayPos = 0;
				break;

			case PCMPlayer::STATE::STATE_PAUSE:
				break;

			default:
				break;
			}

			// 終了位置判定チェック
			if ( isEnd == true ) {
				DWORD curPlayPos;
				player->cpDSBuffer_->GetCurrentPosition( &curPlayPos, 0 );
				if ( curPlayPos < prePlayPos ) {
					// バッファをループした瞬間
					if ( prePlayPos <= finishPos || finishPos <= curPlayPos ) {
						// 終了宣言
						player->stop();
					}
				} else {
					if ( prePlayPos <= finishPos && finishPos <= curPlayPos ) {
						// 終了宣言
						player->stop();
					}
				}
				prePlayPos = curPlayPos;
			}

			//Sleep( 100 );
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}

	//! 再生
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
		// 再生
		spPCMDecoder_->setLoop( isLoop );
		cpDSBuffer_->Play( 0, 0, DSBPLAY_LOOPING );
		state_ = PCMPlayer::STATE::STATE_PLAY;
		return true;
	}

	//! 一時停止
	void PCMPlayer::pause() {
		if ( state_ != PCMPlayer::STATE::STATE_PLAY ) 
		{
			return;
		}
		// 動いていたら止める
		cpDSBuffer_->Stop();
		state_ = PCMPlayer::STATE::STATE_PAUSE;
	}

	//! 再開
	void PCMPlayer::resume() {
		if (state_ != PCMPlayer::STATE::STATE_PAUSE) 
		{
			return;
		}
		// 止まっていたら再生
		play(isLoop_);
	}

	//! 停止
	void PCMPlayer::stop() {
		if ( isReady() == false ) {
			return;
		}
		state_ = PCMPlayer::STATE::STATE_STOP;
		cpDSBuffer_->Stop();

		// バッファの頭出し
		initializeBuffer();
	}

	//! 音量を変える
	void PCMPlayer::setVolume( int volume ) {
		if ( isReady() == true ) {
			cpDSBuffer_->SetVolume( volume );
		}
	}

	//! パンの位置を変える
	void PCMPlayer::setPan( int pan ) {
		if ( isReady() == true ) {
			cpDSBuffer_->SetPan( pan );
		}
	}

	//! 準備できた？
	bool PCMPlayer::isReady() {
		return isReady_;
	}

	//! 再生中？
	bool PCMPlayer::isPlaying() {
		return ( state_ == PCMPlayer::STATE::STATE_PLAY );
	}

	//! 状態を取得
	PCMPlayer::STATE PCMPlayer::getState() {
		return state_;
	}

}
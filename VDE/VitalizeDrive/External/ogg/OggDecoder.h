// OggDecoder.h
//

#ifndef IKD_DIX_OGGDECODER_H
#define IKD_DIX_OGGDECODER_H

#include "vorbisfile.h"
#include "PCMDecoder.h"
#include "OggVorbisResource.h"
#include "DixSmartPtr.h"

namespace Dix {
	class OggDecoder : public PCMDecoder {
	public:
		OggDecoder();
		OggDecoder( sp< OggVorbisResource > oggVorbisResource );
		virtual ~OggDecoder();

		//! �N���A
		virtual void clear();

		//! �Z�O�����g�擾
		virtual bool getSegment( char* buffer, unsigned int size, unsigned int* writeSize, bool* isEnd );

		//! ���o��
		virtual void setHead();

		//! ���S�ȃN���[���𐶐�
		virtual sp< PCMDecoder > createClone();

		//! �T�E���h���Z�b�g
		virtual bool setResource( sp< OggVorbisResource > oggVorbisResource );

	protected:
		sp< OggVorbisResource > oggVorbisResource_;
	};
}

#endif
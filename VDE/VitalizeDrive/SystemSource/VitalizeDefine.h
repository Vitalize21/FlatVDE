#pragma once

//�J������p(�e��f�o�b�O�������A�N�e�B�u�ɂȂ�܂�)
#define	DEVELOP_DEBUG (1)
#define	DEVELOP_DEBUG_PC (1)

//Vitalize Extend Frame ���g�p���܂�( 1:�͂� 0:������ )
//��2019�N����Extend Frame�v��͓������Ă���g�p�ł��܂���B
#define ENABLE_EXTEND_FRAME (0)

//�|�C���^�̉��
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#define SAFE_DELETE(p)  if( (p) != nullptr ){ delete (p);  (p) = nullptr; }

//�R���\�[���ւ̃��O�o��
#define LOG_MESSAGE(message, ...)

//���O�e�L�X�g�̏o�͂��s���� (0:������ 1:�͂�)
//�v�����F���O�o�͗L�����Ńq�[�v�j�󔭐�
#define OUTPUT_LOG_TEXT (0)



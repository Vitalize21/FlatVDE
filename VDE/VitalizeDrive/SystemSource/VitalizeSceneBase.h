#pragma once

#include "VitalizeDefine.h"

class VitalizeSceneBase
{
public:
	bool loadCompletion;

	//	�����E���
	VitalizeSceneBase() : loadCompletion(false) {}
	virtual ~VitalizeSceneBase() { loadCompletion = false; }
	//	������
	virtual bool Initialize() { loadCompletion = true; return true; }
	//	�X�V�E�`��
	virtual void Update() = 0;
	virtual void Render() = 0;
	//�@�J�n�E�I��
	virtual void Begining() {}
	virtual void Ending() {}

#if DEVELOP_DEBUG_PC
public:
	virtual const char* getName() const { return u8"Unnamed Scene"; }
	virtual void inspectorProperty() {}

	virtual void onUpdateStudioMenuMove(const float& easeRatio) {}
#endif

};


#pragma once

#include "VitalizeDefine.h"

class VitalizeSceneBase
{
public:
	bool loadCompletion;

	//	生成・解放
	VitalizeSceneBase() : loadCompletion(false) {}
	virtual ~VitalizeSceneBase() { loadCompletion = false; }
	//	初期化
	virtual bool Initialize() { loadCompletion = true; return true; }
	//	更新・描画
	virtual void Update() = 0;
	virtual void Render() = 0;
	//　開始・終了
	virtual void Begining() {}
	virtual void Ending() {}

#if DEVELOP_DEBUG_PC
public:
	virtual const char* getName() const { return u8"Unnamed Scene"; }
	virtual void inspectorProperty() {}

	virtual void onUpdateStudioMenuMove(const float& easeRatio) {}
#endif

};


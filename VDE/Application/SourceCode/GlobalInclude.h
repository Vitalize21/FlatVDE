#pragma once

//メインフレームワーク
#include "VitalizeFramework.h"

//シングルトン系
#include "ScreenManager.h"
#include "TouchControls.h"

//スタティック系
#include "VitalizeScene.h"
#include "staticUtility.h"

//基本システム
#include "Vitalize2DObj.h"
#include "VitalizeAudio.h"

//デバッグ用
#include "CheckMemoryLeak.h"

namespace GlobalDefinition
{
	static const char* saveDataRootPath = "../Application/SaveData/";

}


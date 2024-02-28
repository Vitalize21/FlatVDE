#pragma once

#include	"VitalizeSceneBase.h"
#include	"Vitalize2DObj.h"
#include	"VitalizeMath.h"
#include	"VitalizeAudio.h"

#include	"VitalizeDefine.h"

class VitalizeTile;
class homeTile;
class MovingTile;

class	sceneVitalizeSystem : public VitalizeSceneBase
{
public:
	static Vector2 pointerPos;				//マウスポインターの座標
	static Vector2 colorPickerPos;			//使用するカラーの座標
	static bool leftClick;					//マウスの左クリック（離すまでON）
	static bool rightClick;					//マウスの右クリック（離すまでON）
	static bool wheelClick;					//マウスの中クリック（離すまでON）
	static char DDFilePath[MAX_PATH];		//D&Dされたファイルパスが入ります
	static bool isDD;						//D&Dされた後true 読み込んだ後downTheDDFlgを呼んでください
	static int wheelValue;					//マウスホイールの回転量(値の使用には関数を使ってください)
	static float variableFrameSupport;		//実質固有デルタタイム

	//削除待ち→AppSystemManager::getWindowSizeFromTypeを利用してください
	static const Vector2 getOriginalScreenSize();

	//ポインター座標取得
	static Vector2 getPointerPos() { return pointerPos; }

	//マウスの左クリック検知（離すまでON）
	static bool getLeftClick() { return leftClick; }
	//マウスの右クリック検知（離すまでON）
	static bool getRightClick() { return rightClick; }

	//D&Dフラグをおろす
	static void downTheDDFlg() { isDD = false; }
	//マウスポインターが画面外に出たことを検知する
	static bool isScreenOutPointer();
	//マウスホイールの回転量を取得（使用後は初期化関数を呼ぶ必要あり）
	static int getWheelValue() { return wheelValue; }
	//マウスホイールの回転量を初期化（値取得後に呼んでください）
	static void resetWheelValue(){ wheelValue = 0; }
	//指定範囲内にマウスポインターがあるか
	static bool checkPointer(Vector2 leftUpPos, Vector2 Size);
	//マウス左クリックトリガー
	static bool leftClickTrigger();

#if DEVELOP_DEBUG

	//名前をつけて保存ダイアログを開く
	static char saveFileFullPath[MAX_PATH];	// フルパス+ファイル名を受け取る領域
	static char saveFileName[MAX_PATH];		// ファイル名を受け取る領域
	static bool openSaveFileNameDialog(const char* filter, const char* defExt);
	
#endif

private:
	
#if DEVELOP_DEBUG
	//ウィンドウサイズのリセット（ツールシーン用）
	void resetWindowSize();
#endif

public:
	sceneVitalizeSystem(){}
	virtual ~sceneVitalizeSystem() {}

};

class	VitalizeDriveSettings
{
public:
	static VitalizeDriveSettings& getInstance()
	{
		static VitalizeDriveSettings instance;
		return instance;
	}
	VitalizeDriveSettings(const VitalizeDriveSettings&) = delete;
	VitalizeDriveSettings& operator=(const VitalizeDriveSettings&) = delete;
	VitalizeDriveSettings(VitalizeDriveSettings&&) = delete;
	VitalizeDriveSettings& operator=(VitalizeDriveSettings&&) = delete;

	int lastStartDay;
	int lastExecutionSceneID;
	bool lastIsCreateScene;
	bool studioOpen;

	void LoadSettings();
	void SaveSettings();

private:
	VitalizeDriveSettings() { Init(); }
	~VitalizeDriveSettings() = default;

	void Init();
};

//インスタンス取得
#define VITALIZE_DRIVE_SETTINGS VitalizeDriveSettings::getInstance()





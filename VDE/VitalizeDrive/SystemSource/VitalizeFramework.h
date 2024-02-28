#pragma once

#include	"VitalizeSystem.h"
#include	"VitalizeMath.h"
#include	"Vitalize2DObj.h"
#include	"VitalizeDefine.h"
#include	"VitalizeSceneBase.h"
#include	"ImguiWrapper.h"
#include	<chrono>

class sceneVitalizeHome;

class VitalizeFramework final
{
public:
	static VitalizeFramework& getInstance()
	{
		static VitalizeFramework instance;
		return instance;
	}

	void Initialize();
	void Release();

	//メインループ開始前準備
	void MainLoopSetup();
	//メインループ処理
	void MainLoopUpdate();
	bool MainLoopRender();
	void MainLoopWait();
	
	//シーンの切り替え
	void ChangeScene(VitalizeSceneBase* newScene);
	//Utex互換 シーンの切り替え
	void ChapterChange(VitalizeSceneBase* newScene) { ChangeScene(newScene); }

	//メインシーンの解放処理
	void FinishScene();
	//メインシーンのリロード
	void SceneBeginingReload();
	void SceneForceReload();

	//メインシーンの停止(→解放までされる)
	void StopMainScene();

	// VitalizeSystemSceneのホームを外部切り替え
	//void VitalizeSystemSceneHomeMenuSwitch() { vitalizeScene->homeMenuSwitch(); }

	//設定FPSの取得
	int getSettingFPS() const { return settingFPS; }

	//実行FPSの取得
	float getExecutionFPS() const { return executionFPS; }

	//デルタタイム取得
	float getDeltaTime() const { return deltaTime; }

	//経過時間(秒単位)取得
	float getElapsedSecond() const { return (1.0f / executionFPS); }

#if DEVELOP_DEBUG_PC
	float getVariantDeltaTime() const { return variantDeltaTime; };
	bool isMainScenePlaying() const { return (mainScene != nullptr); }
	void switchHomeScene();
	void updateSceneController(const Vector2& leftup);
	void renderSceneController(const Vector2& leftup, const Color& buttonCol, bool title = true);
	bool isOpenHome() const;
	bool isExecutionCreaterScene();
	void closeHome();
	VitalizeSceneBase* getMainScene() const { return mainScene; }
	void onUpdateStudioMenuMove(const float& easeRatio);
#endif

#if DEVELOP_DEBUG
	//処理能力の取得
	unsigned int getProcessingPower() const { return processingPower; }
	//実行FPS履歴
	static const unsigned __int16 excutionFPSHistoryArrayMax = 300;
	unsigned __int8* getExcutionFPSHistoryArray() { return excutionFPSHistoryArray; }
	unsigned __int16 getExcutionFPSHistoryIterator() { return excutionFPSHistoryIterator; }
#endif

	VitalizeFramework(const VitalizeFramework&) = delete; 
	VitalizeFramework& operator=(const VitalizeFramework&) = delete; 
	VitalizeFramework(VitalizeFramework&&) = delete; 
	VitalizeFramework& operator=(VitalizeFramework&&) = delete; 

private:
	VitalizeFramework() : mainScene(nullptr), homeScene(nullptr) {}
	~VitalizeFramework() = default;

	VitalizeSceneBase* mainScene;
	sceneVitalizeHome* homeScene;

	//設定FPS（この設定を基準に回す）
	int settingFPS;
	//最終更新時刻
	unsigned long long int lastUpdateTime;
	//次回更新時刻
	unsigned long long int nextUpdateTime;
	//処理開始時間
	unsigned long long int startUpdateTime;

	//処理基準時間
	unsigned int baseProcessingTime;
	//待機時間
	int baseWaitTimeMax;

	//実行FPS
	float executionFPS;

	//delta time
	float deltaTime;

#if DEVELOP_DEBUG_PC
	//可変フレーム用 60F基準 delta time (ツール用。アプリ側では使わない想定)
	float variantDeltaTime;
#endif

#if DEVELOP_DEBUG
	//処理能力
	int processingPower;
	static const __int8 processingPowerAverageListMax = 60;
	unsigned __int16 processingPowerAverageList[processingPowerAverageListMax];
	__int8 processingPowerAverageCount;
	void debugMeasurementUpdate();

	//実行FPSグラフ用
	unsigned __int8 excutionFPSHistoryArray[excutionFPSHistoryArrayMax];
	unsigned __int16 excutionFPSHistoryIterator;
#endif

	//マイクロ秒で現在時刻を取得する
	unsigned long long getCurrentTimeMicro();

	//FPS設定の変更
	void changeFrameRateSetting(int fps);

	//システムシーンを友達に
	friend class sceneVitalizeSystem;
	friend class sceneVitalizeHome;
	//friend class StudioManager;
};

//フレームワークインスタンス取得
#define MAIN_FRAMEWORK_INSTANCE VitalizeFramework::getInstance()

//------------------------------
//メインフレーム系　汎用マクロ
//------------------------------
#pragma region _Macro
//シーンチェンジ
#define VMF_SCENE_CHANGE(next) VitalizeFramework::getInstance().ChangeScene(next);
//デルタタイム取得
#define VMF_DELTA_TIME VitalizeFramework::getInstance().getDeltaTime()
//フレーム経過時間(秒単位)取得
#define VMF_ELAPSED_SECOND VitalizeFramework::getInstance().getElapsedSecond()
//更新基準FPS取得
#define VMF_BASE_FPS VitalizeFramework::getInstance().getSettingFPS()

//秒→フレーム変換（float値を利用してください）
#define SEC_TO_FRAME(second) ((int)((float)VitalizeFramework::getInstance().getSettingFPS() * second))
//フレーム変換→秒
#define FRAME_TO_SEC(frame) ((float)frame / (float)VitalizeFramework::getInstance().getSettingFPS())

#if DEVELOP_DEBUG_PC
//ツール用デルタタイム(60FPS基準)
#define VMF_VARIANT_DELTA_TIME VitalizeFramework::getInstance().getVariantDeltaTime()
#endif

#pragma endregion

//------------------------------
// ランタイム管理者
//------------------------------
class RuntimeSystem final
{
public:
	static Vector2 getScreenSize();
	static void setScreenSize(const int& width, const int& height);
	static const int& getScreenWidth();
	static const int& getScreenHeight();
	static void setScreenPos(const int& x, const int& y);
	static Vector2 getScreenPos();

	static Vector2 getResolutionSetting();	//スワップチェーン生成時の解像度設定取得（デフォFULL_HD）

#if	DEVELOP_DEBUG_PC
	static bool update(bool isGameScenePlay);
	static bool isMainSceneUpdate();
	static bool isMainScenePause() { return mainScenePause; }
	static void switchMainScenePlayPause();
	static void changeMainScenePlayPause(bool play);
	static void framePlay();
	static bool onPointer();
#endif

private:
#if	DEVELOP_DEBUG_PC
	static Vector2 screenPos;
	static Vector2 screenSize;

	static bool mainScenePause;				//メインシーンのポーズ
	enum class FramePlayType : int { None, Self, External };
	static FramePlayType framePlayType;	//メインシーンのポーズ中コマ送り

#endif

#pragma region _Singleton
public:
	RuntimeSystem(const RuntimeSystem&) = delete;
	RuntimeSystem& operator=(const RuntimeSystem&) = delete;
	RuntimeSystem(RuntimeSystem&&) = delete;
	RuntimeSystem& operator=(RuntimeSystem&&) = delete;
private:
	RuntimeSystem() {}
	~RuntimeSystem() = default;
#pragma endregion

};

#if DEVELOP_DEBUG_PC

//------------------------------
// ウィンドウズアプリ関連まとめシステム
//------------------------------
class AppSystemManager final
{
public:
	enum class WindowMode : int { Free = 0, Limit, NotCoverd, OldWindow, };
	enum class MouseInput : int { Left = 0, Right, Wheel, Max };

	//------- ご自由にお使いください -------
	static VitalizeSystem::mainSystem::ScreenType initScreenDeviceResolution(int x, int y);
	static const Vector2& getScreenDeviceResolution() { return screenDeviceResolution; }
	static Vector2 getWindowSizeFromType(VitalizeSystem::mainSystem::ScreenType t);
	static Vector2 getCurrentWindowSize();
	static Vector2 getCurrentWindowCenterPos();
	static const Vector2& getWindowPos() { return windowPos; }
	static void setWindowPos(const Vector2& p) { windowPos = p; }
	static WindowMode getWindowMode() { return windowMode; }
	static void setWindowMode(WindowMode m) { windowMode = m; }
	static const Vector2& getMousePointerPos() { return mousePointerPos; }
	static bool isPointerInScreen();
	static int getWheelValue() { return wheelValue; }
	static bool getMouseClick(MouseInput t = MouseInput::Left) { return (mouseClickInfo[(int)t].isHold); }
	static bool getMouseClickTrigger(MouseInput t = MouseInput::Left) { return (mouseClickInfo[(int)t].frame == 1); }
	static bool isMouseKeyNotPressed();	//マウスすべてのボタンが押されていない状態がtrue
	static bool inPointer(const Vector2& leftUpPos, const Vector2& Size);
	static bool inPointerS(const Vector2& leftUpPos, const Vector2& Size);

	//------- システム側から呼び出す用 -------
	static void changeWindowSizeFromMenuButton();
	static void setMousePointerPos(int x, int y) { mousePointerPos = Vector2(x, y); }
	static void updateMouseClickInfo();
	static void setMouseClick(MouseInput t, bool click);
	static void setWheelValue(int val) { wheelValue = val; }
	static void startWindowMove();
	static void updateWindowMove();
	static void setMaximizeWindow();
	static void setMinimizeWindow();
	static void resetWindowPos();
	static bool isWindowActive();

private:
	static Vector2 screenDeviceResolution;
	static WindowMode windowMode;
	static Vector2 windowPos;
	static bool isMaximizeWindow;
	static Vector2 mousePointerPos;

	struct ClickInfo { bool isHold; int frame; };
	static ClickInfo mouseClickInfo[(int)MouseInput::Max];

	static int wheelValue;

	static Vector2 windowMoveStartPos;
	static Vector2 onMoveStartWindowPos;


#pragma region _Singleton
public:
	AppSystemManager(const AppSystemManager&) = delete;
	AppSystemManager& operator=(const AppSystemManager&) = delete;
	AppSystemManager(AppSystemManager&&) = delete;
	AppSystemManager& operator=(AppSystemManager&&) = delete;
private:
	AppSystemManager() {}
	~AppSystemManager() = default;
#pragma endregion
};

//------------------------------
// スタジオ
//------------------------------
class StudioManager final
{
public:

	static void Initialize();
	static void Begining();
	static void Release();
	static void Update();
	static void Render();

	static Vitalize2DObjResolution* getSystemInterfaceObj() { return interfaceObj; }
	static void onChangeWindowSize();
	static void onMainSceneInitializeAfter();
	static void onMainSceneReleaseAfter();

	enum class LogType : int { Info = 0, Caution, Error, Tool, Info_NoTag };
	static void addLog(LogType t, char const* const _logMessage, ...);

	static bool isOpen() { return menuMoveInfo.isOpen; }
	static void close();

	static bool isDrawTouchControllerDebug() { return drawTouchControllerDebug; }
	static bool isDrawRenderingArea() { return drawRenderingArea; }

private:
	struct MenuMoveInfo { bool isOpen; bool isMove; float openRate; float openEaseValue; };
	static MenuMoveInfo menuMoveInfo;

	static ImguiLogConsole* logConsole;

	static void updateMenuMove();
	static void changeMenuOpenClose();

	enum class MenuType : int { Top, Runtime, Left, Bottom, Right, Max };
	static Vector2 menuPos[(int)MenuType::Max];
	static Vector2 menuSize[(int)MenuType::Max];

	static void updateTopMenu();

	static void renderTopMenu();
	static void renderRuntimeMenu();
	static void renderLeftMenu();
	static void renderBottomMenu();
	static void renderRightMenu();

	static void updateMenuPos();
	static void updateMenuSize();

	static Vitalize2DObjResolution* interfaceObj;
	static Vitalize2DObj* logoObj;

	enum class ButtonSelect : int { 
		None = 0, Close, MaxMin, Minimize, Home, Save, 
		Move, Rotate, Scale, NewScene, BeginingReload, ForceReload,
		Stop, PlayPause, PlayFrame, Info, };
	static ButtonSelect buttonSelectType;
	static void updateMenuButton();

	static int toolCommonState;

	static void menuForceClose();

	static int sceneSelect;
	static int virtualObjectSelect;
	static int resourceListboxSelect;
	static void resetSelecter();
	
	enum class RightMenuType : int { Inspector = 0, SystemInfo };
	static RightMenuType rightMenuType;

	static void renderInspector();
	static void renderSystemInfo();

	static bool isOpenPopupMenu;
	static bool drawTouchControllerDebug;
	static bool drawRenderingArea;

	//static char runtimeInfoText[256];
	//static int runtimeResolutionType;

#pragma region _Singleton
public:
	StudioManager(const StudioManager&) = delete;
	StudioManager& operator=(const StudioManager&) = delete;
	StudioManager(StudioManager&&) = delete;
	StudioManager& operator=(StudioManager&&) = delete;
private:
	StudioManager() {}
	~StudioManager() = default;
#pragma endregion
};

#define LOG_INFO(log, ...) StudioManager::addLog(StudioManager::LogType::Info, log, __VA_ARGS__);
#define LOG_CAUTION(log, ...) StudioManager::addLog(StudioManager::LogType::Caution, log, __VA_ARGS__);
#define LOG_ERROR(log, ...) StudioManager::addLog(StudioManager::LogType::Error, log, __VA_ARGS__);
#define LOG_TOOL(log, ...) StudioManager::addLog(StudioManager::LogType::Tool, log, __VA_ARGS__);

#define LOG_INFO_NO_TAG(log, ...) StudioManager::addLog(StudioManager::LogType::Info_NoTag, log, __VA_ARGS__);

namespace PropertyUtil
{
	void publish(const char* name, int& value, bool readOnly = (false));
	void publish(const char* name, float& value, bool readOnly = (false), const char* format = "%.3f");
	void publish(const char* name, bool& value, bool readOnly = (false));
	void publish(const char* name, Vector2& value, bool readOnly = (false));
	void publish(const char* name, Vector2f& value, bool readOnly = (false), const char* format = "%.3f");

	bool group(const char* name);
	void groupEnd();

	bool button(const char* name, Vector2f size = Vector2f(350.0f, 30.0f));
	bool comboBox(const char* name, int* current_item, const char* const items[], int items_count);

}

#else
#define LOG_INFO(message, ...)
#define LOG_CAUTION(log, ...)
#define LOG_ERROR(log, ...)
#define LOG_TOOL(log, ...)

#endif
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

	//���C�����[�v�J�n�O����
	void MainLoopSetup();
	//���C�����[�v����
	void MainLoopUpdate();
	bool MainLoopRender();
	void MainLoopWait();
	
	//�V�[���̐؂�ւ�
	void ChangeScene(VitalizeSceneBase* newScene);
	//Utex�݊� �V�[���̐؂�ւ�
	void ChapterChange(VitalizeSceneBase* newScene) { ChangeScene(newScene); }

	//���C���V�[���̉������
	void FinishScene();
	//���C���V�[���̃����[�h
	void SceneBeginingReload();
	void SceneForceReload();

	//���C���V�[���̒�~(������܂ł����)
	void StopMainScene();

	// VitalizeSystemScene�̃z�[�����O���؂�ւ�
	//void VitalizeSystemSceneHomeMenuSwitch() { vitalizeScene->homeMenuSwitch(); }

	//�ݒ�FPS�̎擾
	int getSettingFPS() const { return settingFPS; }

	//���sFPS�̎擾
	float getExecutionFPS() const { return executionFPS; }

	//�f���^�^�C���擾
	float getDeltaTime() const { return deltaTime; }

	//�o�ߎ���(�b�P��)�擾
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
	//�����\�͂̎擾
	unsigned int getProcessingPower() const { return processingPower; }
	//���sFPS����
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

	//�ݒ�FPS�i���̐ݒ����ɉ񂷁j
	int settingFPS;
	//�ŏI�X�V����
	unsigned long long int lastUpdateTime;
	//����X�V����
	unsigned long long int nextUpdateTime;
	//�����J�n����
	unsigned long long int startUpdateTime;

	//���������
	unsigned int baseProcessingTime;
	//�ҋ@����
	int baseWaitTimeMax;

	//���sFPS
	float executionFPS;

	//delta time
	float deltaTime;

#if DEVELOP_DEBUG_PC
	//�σt���[���p 60F� delta time (�c�[���p�B�A�v�����ł͎g��Ȃ��z��)
	float variantDeltaTime;
#endif

#if DEVELOP_DEBUG
	//�����\��
	int processingPower;
	static const __int8 processingPowerAverageListMax = 60;
	unsigned __int16 processingPowerAverageList[processingPowerAverageListMax];
	__int8 processingPowerAverageCount;
	void debugMeasurementUpdate();

	//���sFPS�O���t�p
	unsigned __int8 excutionFPSHistoryArray[excutionFPSHistoryArrayMax];
	unsigned __int16 excutionFPSHistoryIterator;
#endif

	//�}�C�N���b�Ō��ݎ������擾����
	unsigned long long getCurrentTimeMicro();

	//FPS�ݒ�̕ύX
	void changeFrameRateSetting(int fps);

	//�V�X�e���V�[����F�B��
	friend class sceneVitalizeSystem;
	friend class sceneVitalizeHome;
	//friend class StudioManager;
};

//�t���[�����[�N�C���X�^���X�擾
#define MAIN_FRAMEWORK_INSTANCE VitalizeFramework::getInstance()

//------------------------------
//���C���t���[���n�@�ėp�}�N��
//------------------------------
#pragma region _Macro
//�V�[���`�F���W
#define VMF_SCENE_CHANGE(next) VitalizeFramework::getInstance().ChangeScene(next);
//�f���^�^�C���擾
#define VMF_DELTA_TIME VitalizeFramework::getInstance().getDeltaTime()
//�t���[���o�ߎ���(�b�P��)�擾
#define VMF_ELAPSED_SECOND VitalizeFramework::getInstance().getElapsedSecond()
//�X�V�FPS�擾
#define VMF_BASE_FPS VitalizeFramework::getInstance().getSettingFPS()

//�b���t���[���ϊ��ifloat�l�𗘗p���Ă��������j
#define SEC_TO_FRAME(second) ((int)((float)VitalizeFramework::getInstance().getSettingFPS() * second))
//�t���[���ϊ����b
#define FRAME_TO_SEC(frame) ((float)frame / (float)VitalizeFramework::getInstance().getSettingFPS())

#if DEVELOP_DEBUG_PC
//�c�[���p�f���^�^�C��(60FPS�)
#define VMF_VARIANT_DELTA_TIME VitalizeFramework::getInstance().getVariantDeltaTime()
#endif

#pragma endregion

//------------------------------
// �����^�C���Ǘ���
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

	static Vector2 getResolutionSetting();	//�X���b�v�`�F�[���������̉𑜓x�ݒ�擾�i�f�t�HFULL_HD�j

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

	static bool mainScenePause;				//���C���V�[���̃|�[�Y
	enum class FramePlayType : int { None, Self, External };
	static FramePlayType framePlayType;	//���C���V�[���̃|�[�Y���R�}����

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
// �E�B���h�E�Y�A�v���֘A�܂Ƃ߃V�X�e��
//------------------------------
class AppSystemManager final
{
public:
	enum class WindowMode : int { Free = 0, Limit, NotCoverd, OldWindow, };
	enum class MouseInput : int { Left = 0, Right, Wheel, Max };

	//------- �����R�ɂ��g���������� -------
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
	static bool isMouseKeyNotPressed();	//�}�E�X���ׂẴ{�^����������Ă��Ȃ���Ԃ�true
	static bool inPointer(const Vector2& leftUpPos, const Vector2& Size);
	static bool inPointerS(const Vector2& leftUpPos, const Vector2& Size);

	//------- �V�X�e��������Ăяo���p -------
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
// �X�^�W�I
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
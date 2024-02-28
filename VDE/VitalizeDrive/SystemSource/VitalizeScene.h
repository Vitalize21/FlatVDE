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
	static Vector2 pointerPos;				//�}�E�X�|�C���^�[�̍��W
	static Vector2 colorPickerPos;			//�g�p����J���[�̍��W
	static bool leftClick;					//�}�E�X�̍��N���b�N�i�����܂�ON�j
	static bool rightClick;					//�}�E�X�̉E�N���b�N�i�����܂�ON�j
	static bool wheelClick;					//�}�E�X�̒��N���b�N�i�����܂�ON�j
	static char DDFilePath[MAX_PATH];		//D&D���ꂽ�t�@�C���p�X������܂�
	static bool isDD;						//D&D���ꂽ��true �ǂݍ��񂾌�downTheDDFlg���Ă�ł�������
	static int wheelValue;					//�}�E�X�z�C�[���̉�]��(�l�̎g�p�ɂ͊֐����g���Ă�������)
	static float variableFrameSupport;		//�����ŗL�f���^�^�C��

	//�폜�҂���AppSystemManager::getWindowSizeFromType�𗘗p���Ă�������
	static const Vector2 getOriginalScreenSize();

	//�|�C���^�[���W�擾
	static Vector2 getPointerPos() { return pointerPos; }

	//�}�E�X�̍��N���b�N���m�i�����܂�ON�j
	static bool getLeftClick() { return leftClick; }
	//�}�E�X�̉E�N���b�N���m�i�����܂�ON�j
	static bool getRightClick() { return rightClick; }

	//D&D�t���O�����낷
	static void downTheDDFlg() { isDD = false; }
	//�}�E�X�|�C���^�[����ʊO�ɏo�����Ƃ����m����
	static bool isScreenOutPointer();
	//�}�E�X�z�C�[���̉�]�ʂ��擾�i�g�p��͏������֐����ĂԕK�v����j
	static int getWheelValue() { return wheelValue; }
	//�}�E�X�z�C�[���̉�]�ʂ��������i�l�擾��ɌĂ�ł��������j
	static void resetWheelValue(){ wheelValue = 0; }
	//�w��͈͓��Ƀ}�E�X�|�C���^�[�����邩
	static bool checkPointer(Vector2 leftUpPos, Vector2 Size);
	//�}�E�X���N���b�N�g���K�[
	static bool leftClickTrigger();

#if DEVELOP_DEBUG

	//���O�����ĕۑ��_�C�A���O���J��
	static char saveFileFullPath[MAX_PATH];	// �t���p�X+�t�@�C�������󂯎��̈�
	static char saveFileName[MAX_PATH];		// �t�@�C�������󂯎��̈�
	static bool openSaveFileNameDialog(const char* filter, const char* defExt);
	
#endif

private:
	
#if DEVELOP_DEBUG
	//�E�B���h�E�T�C�Y�̃��Z�b�g�i�c�[���V�[���p�j
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

//�C���X�^���X�擾
#define VITALIZE_DRIVE_SETTINGS VitalizeDriveSettings::getInstance()





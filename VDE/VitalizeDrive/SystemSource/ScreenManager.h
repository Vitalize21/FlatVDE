#pragma once

#include "VitalizeMath.h"
#include "VitalizeBitUtility.h"

class Vitalize2DObjResolution;
class Color;

class ScreenManager final
{
public:
	static ScreenManager& getInstance()
	{
		static ScreenManager instance;
		return instance;
	}
	ScreenManager(const ScreenManager&) = delete;
	ScreenManager& operator=(const ScreenManager&) = delete;
	ScreenManager(ScreenManager&&) = delete;
	ScreenManager& operator=(ScreenManager&&) = delete;

	//�w�i�摜�̐ݒ�
	void setBackgroundObj(const char* fileName);
	void setBackgroundObjVerify(const char* fileName);
	
	//�w�i�F�̐ݒ�
	void setBackgroundColor(const Color& col);

	//�w�i�̕`��
	void RenderBackground();

	//��ʐU���̍X�V
	void UpdateShakeScreen();

	//�␳���ꂽ�`��͈̓T�C�Y�̎擾
	//(������W��Vitalize2DObjResolution����擾���Ă�������)
	Vector2 getAdjustRenderSize() const;

	//�␳�Ȃ��̑S�`��͈̓T�C�Y�̎擾(���у}�X�N�͈͈ȊO�̕`��͈�)
	Vector2 getFullRenderSize() const;

	//��ʗ]���̔������擾
	//�����g���FX���`��͈͊O�̕Б��@�����g���FY�����ѕЕ��̃T�C�Y
	Vector2 getRenderMarginHalf() const;

	//�`��͈͂̈�Ԓ���W(���у}�X�N�͈͈ȊO�̕`��͈�)
	int getRenderBottom() const;

	//��ʒ������W�̎擾
	Vector2 getScreenCenter() const;

	//��ʃT�C�Y�擾�i���S�ȃE�B���h�E�T�C�Yor�[����ʃT�C�Y�j
	Vector2 getScreenSize() const;

	//��ʐU����
	bool isShakingScreen() const { return (shakeScreenData.lifeSec > 0.0f); }

	//---------------------------------------------

	//�I�Ăяo���֎~�B�V�X�e����p
	void SystemInit();
	//�I�Ăяo���֎~�B�V�X�e����p
	void SystemRelease();
	//�I�Ăяo���֎~�B�V�X�e����p
	void MaskDraw();

	//�I�Ăяo���֎~�B�V�X�e����p
	void ChangeDisplaySize();

#if DEVELOP_DEBUG
	//�`��G���A�̕\��
	void RenderRenderingArea(const Color& col);
#endif

#pragma region _��ʐU���֌W
	//�y�H�z�U���֌W�ۂ��ƕʃN���X�ɕ����邩�E�E�E�H

	class ShakeScreenInfo
	{
	public:
		//��enum�ɂ���Ǝg���Â炢�̂�Define�ɒ��ڏ�����
		//enum class DirBit : int { Up = 1, Down = (1 << 1), Left = (1 << 2), Righe = (1 << 3), };
		VitalizeBit dirBit;		//�U�������i�r�b�g�w��j
		float		lifeSecond;	//�U������
		Vector2f	amplitude;	//�U���i�U���̒��S����ő�ψʂ܂ł̋����j
		Vector2		division;	//�������i�傫���قǍׂ����Ȃ�j
		//TODO: �����̎���

		ShakeScreenInfo() { clear(); }
		void clear()
		{
			dirBit = 0; lifeSecond = 0.0f;
			amplitude = Vector2f(0); division = Vector2(0);
		}
	};

	//��ʐU���̐ݒ�
	void setShakeScreen(const ShakeScreenInfo& info);

#pragma endregion

private:
	ScreenManager(){}
	~ScreenManager() = default;

	Vitalize2DObjResolution* targetObj;
	Color targetColor;

	bool useScreenMask;		//�㉺���у}�X�N���p�i�c���X�N���[�����j
	bool useScaleAdjust;	//�w�i�g�嗘�p�i�����X�N���[�����j
	Vector2 scaleAdjustSize;
	int adjustCommonHeight;	//�}�X�N�Ȃ�}�X�N�����A�w�i�g��Ȃ�`�捂���I�t�Z�b�g�ɗ��p

	enum class RenderType : int
	{
		Tex, Color, 
	};
	RenderType renderType;

	//��ʐU���֘A
	struct ShakeScreenData
	{
		float lifeSec;
		Vector2 basePoint;
		Vector2f power;
		Vector2 rand;
		Vector2 orgRednerPosOffset;
	};
	ShakeScreenData shakeScreenData;

};

#ifndef SCREEN_MANAGER_HEADER
#define SCREEN_MANAGER_HEADER

//�C���X�^���X�擾
#define SCREEN_MANAGER ScreenManager::getInstance()

//��ʐU���w��p
#define SS_INFO ScreenManager::ShakeScreenInfo
#define SS_DIR_UP (1)
#define SS_DIR_DOWN (1 << 1)
#define SS_DIR_LEFT (1 << 2)
#define SS_DIR_RIGHT (1 << 3)
#define SS_DIR_VERTICAL (SS_DIR_UP | SS_DIR_DOWN)
#define SS_DIR_HORIZONTAL (SS_DIR_LEFT | SS_DIR_RIGHT)
#define SS_DIR_ALL (SS_DIR_VERTICAL | SS_DIR_HORIZONTAL)

//��ʃT�C�Y(Vec2)
#define SCREEN_SIZE ScreenManager::getInstance().getScreenSize()
#define SCREEN_WIDTH (ScreenManager::getInstance().getScreenSize().x)
#define SCREEN_HEIGHT (ScreenManager::getInstance().getScreenSize().y)

#endif


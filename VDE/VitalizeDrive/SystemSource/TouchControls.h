#pragma once

class TouchControls final
{
public:
	static TouchControls& getInstance()
	{
		static TouchControls instance;
		return instance;
	}
	TouchControls(const TouchControls&) = delete;
	TouchControls& operator=(const TouchControls&) = delete;
	TouchControls(TouchControls&&) = delete;
	TouchControls& operator=(TouchControls&&) = delete;

	enum class TouchResult : int 
	{
		//���K�̌���
		None, OnTap, Tap, Hold, Flick, Swipe,
#if DEVELOP_DEBUG
		//��O�F�~�j�t���b�N�i��O�Ȃ̂Ő��f�o�b�O�`��p�j
		MiniFlick
#endif
	};

	//�ÓI�֐� ----------------------
	static bool checkWithinRange(const Vector2& checkPos, const Vector2& rangeLeftUp, const Vector2& rangeSize);

	//���J�֐� ----------------------
	void Begining();
	TouchResult CheckUpdate();

#if DEVELOP_DEBUG
	void DebugUpdate();
	void DebugRender();
#endif

	void setPlayInfo(float judgeSecond, float swipeLength, float tapLimit)
	{
		holdJudgeSecond = judgeSecond;
		swipeJudgeLength = swipeLength;
		tapLimitLength = tapLimit;
	}

	Vector2 getTapPos() const { return onTouchPos; }
	Vector2 getPresentPos() const;
	Vector2 getHoldPos() const { return onTouchPos; }
	float getHoldSecond() const { return holdSecond; }
	Vector2 getFlickVec() const { return flickVec; }
	Vector2 getSwipeVec() const;

	Vector2 getTapPosR() const;
	Vector2 getFlickVecS() const;
	Vector2 getSwipeVecS() const;

	bool checkTapPos(const Vector2& leftup, const Vector2& size) const;
	//checkTapPos In Adjusted Rendering Area
	bool checkTapPosInARA(const Vector2& leftup, const Vector2& size) const;

	//�V�X�e���Ăяo���p�i�A�v��������Ă΂Ȃ��ł��������j
	void systemTouchUpdate(Vector2 p, bool isInput);
	//�V�X�e���Ăяo���p�i�A�v��������Ă΂Ȃ��ł��������j
	void systemEndUpdate() { isTouch = false; }

private:
	TouchControls() {}
	~TouchControls() = default;

	enum class State : int 
	{
		wait, preCheck, holdContinue, swipeContinue, goWait, max 
	};
	State state;

	TouchResult internaljudge;
	int updateUniqueID;
	int thisFrameID;

	float holdJudgeSecond;
	float holdSecond;
	float swipeJudgeLength;
	float tapLimitLength;
	
	Vector2 onTouchPos;
	Vector2 flickVec;

	//�V�X�e������ݒ肳���^�b�v���W
	Vector2 systemTouchPos;
	//�V�X�e���̃^�b�v���m�p�t���O�i�펞�X�V�B�G��Ă����ON�j
	bool isTouch;

#if DEVELOP_DEBUG
	float debugDrawAlpha;
	TouchResult debugResult;
	Vector2 debugDrawPos1;
	Vector2 debugDrawPos2;

	struct SwipeLine
	{
		Vector2 p1, p2;
		bool active;
		void Init()
		{
			p1 = p2 = Vector2(0);
			active = false;
		}
	};
	static const int debugSwipeLineMax = 10;
	SwipeLine debugSwipeLine[debugSwipeLineMax];
	int useDebugSwipeLine;

	void UpdateDebugSwipeLine();
	void ResetDebugSwipeLine();
#endif
};

//�C���X�^���X�擾
#define TOUCH_CONTROLER TouchControls::getInstance()


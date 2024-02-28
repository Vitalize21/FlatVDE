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
		//正規の結果
		None, OnTap, Tap, Hold, Flick, Swipe,
#if DEVELOP_DEBUG
		//例外：ミニフリック（例外なので専らデバッグ描画用）
		MiniFlick
#endif
	};

	//静的関数 ----------------------
	static bool checkWithinRange(const Vector2& checkPos, const Vector2& rangeLeftUp, const Vector2& rangeSize);

	//公開関数 ----------------------
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

	//システム呼び出し用（アプリ側から呼ばないでください）
	void systemTouchUpdate(Vector2 p, bool isInput);
	//システム呼び出し用（アプリ側から呼ばないでください）
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

	//システムから設定されるタップ座標
	Vector2 systemTouchPos;
	//システムのタップ検知用フラグ（常時更新。触れている間ON）
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

//インスタンス取得
#define TOUCH_CONTROLER TouchControls::getInstance()


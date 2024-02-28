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

	//背景画像の設定
	void setBackgroundObj(const char* fileName);
	void setBackgroundObjVerify(const char* fileName);
	
	//背景色の設定
	void setBackgroundColor(const Color& col);

	//背景の描画
	void RenderBackground();

	//画面振動の更新
	void UpdateShakeScreen();

	//補正された描画範囲サイズの取得
	//(左上座標はVitalize2DObjResolutionから取得してください)
	Vector2 getAdjustRenderSize() const;

	//補正なしの全描画範囲サイズの取得(黒帯マスク範囲以外の描画範囲)
	Vector2 getFullRenderSize() const;

	//画面余白の半分を取得
	//水平拡張：Xが描画範囲外の片側　垂直拡張：Yが黒帯片方のサイズ
	Vector2 getRenderMarginHalf() const;

	//描画範囲の一番底座標(黒帯マスク範囲以外の描画範囲)
	int getRenderBottom() const;

	//画面中央座標の取得
	Vector2 getScreenCenter() const;

	//画面サイズ取得（完全なウィンドウサイズor端末画面サイズ）
	Vector2 getScreenSize() const;

	//画面振動中
	bool isShakingScreen() const { return (shakeScreenData.lifeSec > 0.0f); }

	//---------------------------------------------

	//！呼び出し禁止。システム専用
	void SystemInit();
	//！呼び出し禁止。システム専用
	void SystemRelease();
	//！呼び出し禁止。システム専用
	void MaskDraw();

	//！呼び出し禁止。システム専用
	void ChangeDisplaySize();

#if DEVELOP_DEBUG
	//描画エリアの表示
	void RenderRenderingArea(const Color& col);
#endif

#pragma region _画面振動関係
	//【？】振動関係丸ごと別クラスに分けるか・・・？

	class ShakeScreenInfo
	{
	public:
		//↓enumにすると使いづらいのでDefineに直接書くわ
		//enum class DirBit : int { Up = 1, Down = (1 << 1), Left = (1 << 2), Righe = (1 << 3), };
		VitalizeBit dirBit;		//振動方向（ビット指定）
		float		lifeSecond;	//振動時間
		Vector2f	amplitude;	//振幅（振動の中心から最大変位までの距離）
		Vector2		division;	//分割数（大きいほど細かくなる）
		//TODO: 減衰の実装

		ShakeScreenInfo() { clear(); }
		void clear()
		{
			dirBit = 0; lifeSecond = 0.0f;
			amplitude = Vector2f(0); division = Vector2(0);
		}
	};

	//画面振動の設定
	void setShakeScreen(const ShakeScreenInfo& info);

#pragma endregion

private:
	ScreenManager(){}
	~ScreenManager() = default;

	Vitalize2DObjResolution* targetObj;
	Color targetColor;

	bool useScreenMask;		//上下黒帯マスク利用（縦長スクリーン時）
	bool useScaleAdjust;	//背景拡大利用（横長スクリーン時）
	Vector2 scaleAdjustSize;
	int adjustCommonHeight;	//マスクならマスク高さ、背景拡大なら描画高さオフセットに利用

	enum class RenderType : int
	{
		Tex, Color, 
	};
	RenderType renderType;

	//画面振動関連
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

//インスタンス取得
#define SCREEN_MANAGER ScreenManager::getInstance()

//画面振動指定用
#define SS_INFO ScreenManager::ShakeScreenInfo
#define SS_DIR_UP (1)
#define SS_DIR_DOWN (1 << 1)
#define SS_DIR_LEFT (1 << 2)
#define SS_DIR_RIGHT (1 << 3)
#define SS_DIR_VERTICAL (SS_DIR_UP | SS_DIR_DOWN)
#define SS_DIR_HORIZONTAL (SS_DIR_LEFT | SS_DIR_RIGHT)
#define SS_DIR_ALL (SS_DIR_VERTICAL | SS_DIR_HORIZONTAL)

//画面サイズ(Vec2)
#define SCREEN_SIZE ScreenManager::getInstance().getScreenSize()
#define SCREEN_WIDTH (ScreenManager::getInstance().getScreenSize().x)
#define SCREEN_HEIGHT (ScreenManager::getInstance().getScreenSize().y)

#endif


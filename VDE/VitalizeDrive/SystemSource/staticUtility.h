#pragma once

#include "number.h"
#include "../SystemSource/Vitalize2DObj.h"

struct Vector2;
class BattleFade;

class Utility
{
public:
	static bool Init();
	static void UnInit();

	static Number* num;

	enum class FADE_MODE : int
	{
		FADE_OUT = 0,
		FADE_IN,
		WHITE_OUT,
		WHITE_IN,
		RED_OUT,
		RED_IN,
		BLUE_OUT,
		BLUE_IN,
		//スライドフェード
		SLIDE_OUT,
		SLIDE_IN,
		//バトルフェード
		BATTLE_OUT,
		BATTLE_IN,

		INVALID = -1,
	};
	static void SetFade(FADE_MODE mode);
	static bool fadeUpdate(float speed = (0.01f));
	static void fadeRender();
	static void fadeForceFinish();
	static void fadeClear();

	static void colorRender(const Color& col);
	static void colorRender(int x, int y, int sizeX, int sizeY, const Color& col = Color::White);
	static void colorRenderS(int x, int y, int sizeX, int sizeY, const Color& col = Color::White);

	//Utility::stringRenderは廃止されますVitalizeStringRendererを利用してください
	static void stringRender(int x, int y, char* str, float scale = (1.0f));
	static void stringRender(int x, int y, const char* str, float scale = (1.0f));
	static void stringRender(const Vector2& pos, const char* str, float scale = (1.0f));
	static void stringRenderInCenter(int x, int y, const char* str, float scale = (1.0f), float alpha = (1.0f));
	static void stringRenderInCenter(int x, int y, const char* str, Vector2f scale, float alpha = (1.0f));

	//汎用数字描画
	static void numberRender(Vector2 rPos, int value, const Color& color = (Color::White),
		float scale = (0.5f), int beam = (3), bool beamf = (true))
	{
		num->RenderResolution(rPos, scale, beam, beamf, value, color);
	}
	static int numberRenderCS(Vector2 rPos, int value, float alpha = (1.0f),
		float scale = (0.5f), int beam = (3), bool beamf = (true))
	{
		return num->RenderConnextString(rPos, scale, beam, beamf, value, alpha);
	}

	static void shapeRender(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col)
	{
		shape->RenderColor(screen_lx, screen_ly, screen_SizeX, screen_SizeY, tex_Lx, tex_Ly, size_Rx, size_Ry, col);
	}
	static void shapeRenderVertex(const Vector2f& leftup, const Vector2f& rightup, const Vector2f& leftbottom, const Vector2f& rightbuttom,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col)
	{
		shape->RenderVertex(leftup, rightup, leftbottom, rightbuttom, tex_Lx, tex_Ly, size_Rx, size_Ry, col);
	}
	static void shapeRenderTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2,
		const Vector2& uv0, const Vector2& uv1, const Vector2& uv2, const Color& col)
	{
		shape->RenderTriangle(p0, p1, p2, uv0, uv1, uv2, col);
	}

#if DEVELOP_DEBUG
	static void debugRender(int x, int y, const char* name, int number, int activeNum, float scale = (0.5f));
	static void debugRender(int x, int y, const char* name, Vector2 vec, int activeNum, float scale = (0.5f));
	static void debugRender(int x, int y, const char* name, float number, int activeUpperNum, int activeUnderNum, float scale = (0.5f));
#endif

private:
	static Vitalize2DObj* fade;
	static float fadeAlpha;
	static FADE_MODE fadeMode;
	static BattleFade* battleFade;
	static Vitalize2DObj* shape;

	//new させない
	Utility() {}
	~Utility() {}

	friend class BattleFade;
};

class BattleFade
{
public:
	BattleFade() {}
	~BattleFade() {}

	void reset();
	void resume();

	bool update(float timespeed = (0.01f));
	void render();

	void forceFinith();

	int range;      //中央からの距離
	int size;			//バーサイズ
	int rotate;
	float alpha;

	float timer;

	enum class AnimState : int { In1 = 0, In2, In3,  Stay, Out1, END, };
	AnimState animState;

private:
	void renderCore(int x, int y, int sizeX, int sizeY,  float rotate, float alpha);

	
};


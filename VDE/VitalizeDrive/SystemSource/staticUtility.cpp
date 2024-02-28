#include	"staticUtility.h"
#include	"VitalizeSystem.h"
#include	"VitalizeFramework.h"
#include	"VitalizeMath.h"
#include	"ScreenManager.h"
#include	"VitalizeStringRenderer.h"

Number* Utility::num = nullptr;

Vitalize2DObj* Utility::fade = nullptr;
float Utility::fadeAlpha = 0.0f;
Utility::FADE_MODE Utility::fadeMode;
BattleFade* Utility::battleFade = nullptr;
Vitalize2DObj* Utility::shape = nullptr;

bool Utility::Init()
{
	num = new Number("tools/num3.png");
	fade = new Vitalize2DObj("tools/slideFade.png");
	battleFade = new BattleFade();
	shape = new Vitalize2DObj("../VitalizeDrive/SystemResource/ShapeRender.png", true);

	return true;
}

void Utility::UnInit()
{
	SAFE_DELETE(num);
	SAFE_DELETE(fade);
	SAFE_DELETE(battleFade);
	SAFE_DELETE(shape);
}

#pragma region FADE

void Utility::SetFade(Utility::FADE_MODE mode)
{
	fadeMode = mode;
	switch (fadeMode)
	{
	case Utility::FADE_MODE::FADE_OUT:
	case Utility::FADE_MODE::WHITE_OUT:
	case Utility::FADE_MODE::RED_OUT:
	case Utility::FADE_MODE::BLUE_OUT:
	case Utility::FADE_MODE::SLIDE_OUT:
		fadeAlpha = 0.0f;
		break;
	case Utility::FADE_MODE::FADE_IN:
	case Utility::FADE_MODE::WHITE_IN:
	case Utility::FADE_MODE::RED_IN:
	case Utility::FADE_MODE::BLUE_IN:
	case Utility::FADE_MODE::SLIDE_IN:
		fadeAlpha = 1.0f;
		break;
	case Utility::FADE_MODE::BATTLE_OUT:	battleFade->reset(); break;
	case Utility::FADE_MODE::BATTLE_IN:		battleFade->resume(); break;
	
	}
}

bool Utility::fadeUpdate(float speed)
{
	switch (fadeMode)
	{
	case Utility::FADE_MODE::FADE_OUT:
	case Utility::FADE_MODE::WHITE_OUT:
	case Utility::FADE_MODE::RED_OUT:
	case Utility::FADE_MODE::BLUE_OUT:
	case Utility::FADE_MODE::SLIDE_OUT:
		if (fadeAlpha >= 1.00f)return true;
		fadeAlpha += speed * VMF_DELTA_TIME;
		if (fadeAlpha >= 1.00f)
		{
			fadeAlpha = 1.00f;
			return true;
		}
		break;
	case Utility::FADE_MODE::FADE_IN:
	case Utility::FADE_MODE::WHITE_IN:
	case Utility::FADE_MODE::RED_IN:
	case Utility::FADE_MODE::BLUE_IN:
	case Utility::FADE_MODE::SLIDE_IN:
		if (fadeAlpha <= 0.00f)return true;
		fadeAlpha -= speed * VMF_DELTA_TIME;
		if (fadeAlpha <= 0.00f)
		{
			fadeAlpha = 0.00f;
			fadeMode = Utility::FADE_MODE::INVALID;	//フェードイン終了は画面見えてる状態なのでInvalidに移行していい
			return true;
		}
		break;
	case Utility::FADE_MODE::BATTLE_OUT:
	case Utility::FADE_MODE::BATTLE_IN:
		return battleFade->update(speed);
	}
	return false;
}

void Utility::fadeRender()
{
	switch (fadeMode)
	{
	case Utility::FADE_MODE::FADE_OUT:
	case Utility::FADE_MODE::FADE_IN:
		fade->RenderAlpha(0, 0, VitalizeSystem::mainSystem::ScreenWidth, VitalizeSystem::mainSystem::ScreenHeight, 1, 1, 12, 12, fadeAlpha);
		break;
	case Utility::FADE_MODE::WHITE_OUT:
	case Utility::FADE_MODE::WHITE_IN:
		fade->RenderAlpha(0, 0, VitalizeSystem::mainSystem::ScreenWidth, VitalizeSystem::mainSystem::ScreenHeight, 1, 33, 12, 12, fadeAlpha);
		break;
	case Utility::FADE_MODE::RED_OUT:
	case Utility::FADE_MODE::RED_IN:
		fade->RenderAlpha(0, 0, VitalizeSystem::mainSystem::ScreenWidth, VitalizeSystem::mainSystem::ScreenHeight, 1, 17, 12, 12, fadeAlpha);
		break;
	case Utility::FADE_MODE::BLUE_OUT:
	case Utility::FADE_MODE::BLUE_IN:
		fade->RenderAlpha(0, 0, VitalizeSystem::mainSystem::ScreenWidth, VitalizeSystem::mainSystem::ScreenHeight, 1, 49, 12, 12, fadeAlpha);
		break;
	case Utility::FADE_MODE::SLIDE_OUT:
		{
			const Vector2f rScale = Vitalize2DObjResolution::getSystemRednerPosScale();
			const int renderHeight = Vitalize2DObjResolution::getSystemRednerPosOffset().y;
			const Vector2 renderSize = Vector2(996, 1080) * rScale.y;	//モアモアのサイズ。スケールは高さに合わせる
			const Vector2 renderSpace = SCREEN_MANAGER.getFullRenderSize();
			const int move = (int)((renderSize.x + renderSpace.x) * fadeAlpha);

			fade->RenderAlpha(
				renderSpace.x - move, renderHeight - 2,
				renderSize.x, renderSize.y + 2,
				16, 0, 118, 128, fadeAlpha);//モアモア
			fade->RenderAlpha(
				renderSpace.x + renderSize.x - move, renderHeight,
				renderSpace.x, renderSpace.y,
				1, 1, 12, 12, fadeAlpha);	//黒
		}
		break;
	case Utility::FADE_MODE::SLIDE_IN:
		{
			const Vector2f rScale = Vitalize2DObjResolution::getSystemRednerPosScale();
			const int renderHeight = Vitalize2DObjResolution::getSystemRednerPosOffset().y;
			const Vector2 renderSize = Vector2(996, 1080) * rScale.y;	//モアモアのサイズ。スケールは高さに合わせる
			const Vector2 renderSpace = SCREEN_MANAGER.getFullRenderSize();
			const int move = (int)((renderSize.x + renderSpace.x) * (1.0f - fadeAlpha));

			fade->RenderAlpha(
				-move, renderHeight, renderSpace.x, renderSpace.y,
				1, 1, 12, 12, fadeAlpha);	//黒
			fade->RenderAlpha(
				renderSpace.x - move, renderHeight - 2, renderSize.x, renderSize.y + 2,
				138, 0, 118, 128, fadeAlpha);//モアモア
		}
		break;
	case Utility::FADE_MODE::BATTLE_OUT:
	case Utility::FADE_MODE::BATTLE_IN:
		battleFade->render();
		break;
	}
}

void Utility::fadeForceFinish()
{
	switch (fadeMode)
	{
	case Utility::FADE_MODE::FADE_OUT:
	case Utility::FADE_MODE::WHITE_OUT:
	case Utility::FADE_MODE::RED_OUT:
	case Utility::FADE_MODE::BLUE_OUT:
	case Utility::FADE_MODE::SLIDE_OUT:
		fadeAlpha = 1.00f;
		break;
	case Utility::FADE_MODE::FADE_IN:
	case Utility::FADE_MODE::WHITE_IN:
	case Utility::FADE_MODE::RED_IN:
	case Utility::FADE_MODE::BLUE_IN:
	case Utility::FADE_MODE::SLIDE_IN:
		fadeClear();
		break;
	case Utility::FADE_MODE::BATTLE_OUT:
	case Utility::FADE_MODE::BATTLE_IN:
		battleFade->forceFinith();
		break;
	}
}

void Utility::fadeClear()
{
	fadeAlpha = 0.00f;
	fadeMode = Utility::FADE_MODE::INVALID;
}

#pragma endregion

#pragma region General_Render

void Utility::colorRender(const Color& col)
{
	const Vector2 windowSize = AppSystemManager::getCurrentWindowSize();
	fade->RenderColor(0, 0, windowSize.x, windowSize.y, 5, 35, 4, 4, col);
}

void Utility::colorRender(int x, int y, int sizeX, int sizeY, const Color& col)
{
	fade->RenderColor(x, y, sizeX, sizeY, 5, 35, 4, 4, col);
}

void Utility::colorRenderS(int x, int y, int sizeX, int sizeY, const Color& col)
{
	const Vector2 of = Vitalize2DObjS::getSystemRednerPosOffset();
	const Vector2f rs = Vitalize2DObjS::getSystemRednerPosScale();
	fade->RenderColor(
		of.x + (int)(x * rs.x), of.y + (int)(y * rs.y),
		(int)(sizeX * rs.x), (int)(sizeY * rs.y),
		5, 35, 4, 4, col);
}

#pragma endregion

void Utility::stringRender(int x, int y, char* str, float scale) 
{ 
	VitalizeStringRenderer::Render(x, y, str, scale);
}
void Utility::stringRender(int x, int y, const char* str, float scale)
{
	VitalizeStringRenderer::Render(x, y, str, scale);
}
void Utility::stringRender(const Vector2& pos, const char* str, float scale)
{ 
	VitalizeStringRenderer::Render(pos.x, pos.y, str, scale);
}
void Utility::stringRenderInCenter(int x, int y, const char* str, float scale, float alpha) 
{ 
	VitalizeStringRenderer::RenderInCenter(x, y, str, scale, alpha);
}
void Utility::stringRenderInCenter(int x, int y, const char* str, Vector2f scale, float alpha) 
{ 
	VitalizeStringRenderer::RenderInCenter(x, y, str, scale, alpha);
}

#if DEVELOP_DEBUG

void Utility::debugRender(int x, int y, const char* name, int number, int activeNum, float scale)
{
	int numberRenderPosX = VitalizeStringRenderer::Render(x, y, name, scale);
	num->Render(numberRenderPosX + (int)(24.0f*scale), y, scale * 0.5f, activeNum, true, number);
}

void Utility::debugRender(int x, int y, const char* name, Vector2 vec, int activeNum, float scale)
{
	int numberRenderPosX = VitalizeStringRenderer::Render(x, y, name, scale);
	num->Render(numberRenderPosX + (int)(24.0f*scale), y, scale * 0.5f, activeNum, true, vec.x);
	num->Render(numberRenderPosX + (int)((activeNum + 1)*(16.0f*scale)) + (int)(32.0f*scale), y, scale * 0.5f, activeNum, true, vec.y);
}

void Utility::debugRender(int x, int y, const char* name, float number, int activeUpperNum, int activeUnderNum, float scale)
{
	int numberRenderPosX = VitalizeStringRenderer::Render(x, y, name, scale);
	num->Render(numberRenderPosX + (int)(24.0f * scale), y, scale * 0.5f, activeUpperNum, activeUnderNum, number);
}

#endif

#pragma region BattleFade

void BattleFade::reset()
{
	range = 900;        //中央からの距離
	size = 50;			//バーサイズ
	rotate = 0;
	alpha = 1.0f;

	timer = 0;

	animState = AnimState::In1;
}

void BattleFade::resume()
{
	animState = AnimState::Out1;
	timer = .0f;
}	

namespace BattleFadeData
{
	const int image_height = 5000;
	const Vector2 center_pos = Vector2(960, 540);
	const int in1_range_start = 900;
	const int in1_range_end = 100;
	const int in1_rotate_start = 0;
	const int in1_rotate_end = 70;
	const int in2_range_start = in1_range_end;
	const int in2_range_end = 1200;
	const int in2_size_start = 50;
	const int in2_size_end = 500;
	const int in3_size_start = in2_size_end;
	const int in3_size_end = 1200;
	const int out1_range_start = in2_range_end;
	const int out1_range_end = 50;
	const int out1_size_start = in3_size_end;
	const int out1_size_end = 0;
	const int out1_rotate_start = in1_rotate_end;
	const int out1_rotate_end = 90;
	const float out1_alpha_start = 1.0f;
	const float out1_alpha_end = .1f;
}

bool BattleFade::update(float timespeed)
{
	using namespace BattleFadeData;

	switch (animState)
	{

	case AnimState::In1:
		//細いのが寄ってくる
		//時計回転
		//range　900　→　100　まで
		//rotate 0 →　70　まで

		range = (int)(((in1_range_end - in1_range_start) * timer) + in1_range_start);
		rotate = (int)(((in1_rotate_end - in1_rotate_start) * timer) + in1_rotate_start);

		timer += timespeed * VMF_DELTA_TIME;
		if (timer > 1.0f) {
			animState = AnimState::In2;
			timer = .0f;
		}
		break;
	case AnimState::In2:
		//太くなりながら離れる

		//range 100→900
		//size　50→300

		range = (int)(((in2_range_end - in2_range_start) * timer) + in2_range_start);
		size = ((int)((in2_size_end - in2_size_start) * timer) + in2_size_start);

		timer += timespeed * VMF_DELTA_TIME;
		if (timer > 1.0f) {
			animState = AnimState::In3;
			timer = .0f;
		}

		break;
	case AnimState::In3:
		//太くなる
		//size 300→900

		size = (int)(((in3_size_end - in3_size_start) * timer) + in3_size_start);

		timer += timespeed * VMF_DELTA_TIME;
		if (timer > 1.0f) {
			animState = AnimState::Stay;
			timer = .0f;
			return true;
		}


		break;
	case AnimState::Stay:

		//timer += timespeed * VMF_DELTA_TIME;
		//if (timer > 1.0f) {
		//	animState = AnimState::Out1;
		//	timer = .0f;
		//}
		//break;
		return true;

	case AnimState::Out1:
		//薄くなりながらほそくなる
		//時計回転

		range = (int)(((out1_range_end - out1_range_start) * timer) + out1_range_start);
		rotate = (int)(((out1_rotate_end - out1_rotate_start) * timer) + out1_rotate_start);
		size = (int)(((out1_size_end - out1_size_start) * timer) + out1_size_start);
		alpha = ((out1_alpha_end - out1_alpha_start) * timer) + out1_alpha_start;


		timer += timespeed * VMF_DELTA_TIME;
		if (timer > 1.0f) {
			animState = AnimState::END;
			timer = .0f;
			return true;
		}
		break;
	case AnimState::END:
		return true;
	}

	return false;
}

void BattleFade::render()
{
	using namespace BattleFadeData;

	renderCore(range + center_pos.x - (int)(size*0.5f), center_pos.y - (int)(image_height*0.5f), size, image_height, DEGREE_TO_RADIAN(rotate), alpha);
	renderCore(-range + center_pos.x - (int)(size*0.5f), center_pos.y - (int)(image_height*0.5f), size, image_height, DEGREE_TO_RADIAN(rotate), alpha);

}

void BattleFade::renderCore(int x, int y, int sizeX, int sizeY, float rotate, float alpha)
{
	const Vector2 of = Vitalize2DObjS::getSystemRednerPosOffset();

	switch (animState)
	{
	case AnimState::Stay:
	case AnimState::END:
		{
			Utility::fade->Render(of.x, of.y, RuntimeSystem::getScreenWidth(), RuntimeSystem::getScreenHeight(), 5, 5, 4, 4);
		}
		return;
	}

	const Vector2f rs = Vitalize2DObjS::getSystemRednerPosScale();
	Utility::fade->RenderAlpha(
		of.x + (int)(x * rs.x), of.y + (int)(y * rs.y), (int)(sizeX * rs.x), (int)(sizeY * rs.y),
		5, 5, 4, 4, rotate, alpha);
}

void BattleFade::forceFinith()
{
	if (animState < AnimState::Stay)
	{
		animState = AnimState::Stay;
		return;
	}
	if (animState > AnimState::Stay)
	{
		animState = AnimState::END;
		return;
	}
}

#pragma endregion


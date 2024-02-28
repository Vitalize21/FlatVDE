#pragma once

#include	"Vitalize2DObj.h"
#include	"VitalizeMath.h"
#include	"VitalizeBitUtility.h"

#pragma region Data

class Particle2DInfo
{
public:
	int			objIndex;
	Color		startColor;
	Color		endColor;
	Vector2		pos;
	Vector2f	move;
	Vector2f	power;
	float		startScale;
	float		endScale;
	float		lifeSecond;
	Vitalize2DObjDrive::BlendState blendType;

	Particle2DInfo() { clear(); }
	virtual void clear()
	{
		objIndex = 0;
		startColor = endColor = Color::White;
		pos = Vector2();
		move = power = Vector2f();
		lifeSecond = 1.0f;
		blendType = Vitalize2DObjDrive::BlendState::ADD;
	}
};

#pragma endregion

class VitalizeParticle2DSystem
{
public:
	static void Clear();
	static void Update();
	static void Render();
	static void Set(const Particle2DInfo& info);
	
#if DEVELOP_DEBUG
	static int getActiveNum() { return updateCount; }
#endif

private:
	//class new させない
	VitalizeParticle2DSystem() {}
	~VitalizeParticle2DSystem() {}

	static Vitalize2DObjResolution* mainObj;

	enum class ImageType : int
	{
		Default = -1,
	};

	class UpdateInfo : public Particle2DInfo
	{
	public:
		bool isRunning;
		Color currentColor;
		float currentScale;

		//毎更新で割り算したくないのでメモリで解決
		int objIndexH;
		float progressRate;
		float updateRate;

		//絵の種類
		ImageType imageType;

		void clear() override final
		{
			Particle2DInfo::clear();
			isRunning = false;
			currentColor = Color::White;
			objIndexH = 0;
			currentScale = progressRate = lifeSecond = 0.0f;
			imageType = ImageType::Default;
		}
	};

	static const int elementsMax = 500;
	static UpdateInfo* infoArray;
	static int updateCount;
	static int arrayCirculationCount;

	static bool SetCore(const Particle2DInfo& info, int& settedIndex);

public:
	//※システムから自動で呼ばれています。この関数は呼び出さないでください。
	static void SystemInit();
	//※システムから自動で呼ばれています。この関数は呼び出さないでください。
	static void SystemRelease();

};


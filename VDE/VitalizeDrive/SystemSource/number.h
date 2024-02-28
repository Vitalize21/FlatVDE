#ifndef		_NUMBER_H_
#define		_NUMBER_H_

#include	"../SystemSource/Vitalize2DObj.h"
#include	"../SystemSource/VitalizeMath.h"

#define NUM_WIDTH 45
#define NUM_HEIGHT 64

class Number
{
private:
	Vitalize2DObj* num;
public:
	Number(){ num = new Vitalize2DObj("DATA/tools/num3.png"); }
	Number(const char* fname){ num = new Vitalize2DObj(fname); }
	~Number(){ delete num; }

	Vitalize2DObj* getObj() const { return num; }

	//通常描画
	void Render(int x, int y, float scale, int beam, bool beamf, int n, const Color& col = Color::White) const
	{
		float srcY = NUM_HEIGHT * scale;
		float scaleX = NUM_WIDTH * scale;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;

		//マイナスの時
		if (workN < 0)
		{
			//正の値に
			workN = -n;
		}

		//桁越えガード(最大桁数を９で埋める)
		if (workN >= workB * 10)
		{
			workN = (workB * 10) - 1;
		}

		bool cutFlg = true;

		for (int i = 0; i < beam; i++)
		{
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			if (cutFlg && beamf && i < beam - 1 && workNum <= 0)continue;
			num->RenderColor(x + ((int)scaleX * i), y, (int)scaleX, (int)srcY,
				NUM_WIDTH * workNum, 0, NUM_WIDTH, NUM_HEIGHT, col);
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
			if (cutFlg)
			{
				if (n < 0)
				{
					//マイナス表示
					num->RenderColor(x + ((int)scaleX * (i-1)), y, (int)scaleX, (int)srcY,
						0, 64, NUM_WIDTH, NUM_HEIGHT, col);
#if DEVELOP_DEBUG
					Vitalize2DObjDrive::stringDrawCallCount++;
#endif
				}
				cutFlg = false;
			}
		}
	}
	//通常描画Vec2版
	void Render(Vector2 rPos, float scale, int beam, bool beamf, int n) const
	{
		Render(rPos.x, rPos.y, scale, beam, beamf, n);
	}
	//通常描画アルファあり
	void RenderAlpha(int x, int y, float scale, int beam, bool beamf, int n, float alpha) const
	{
		float srcY = NUM_HEIGHT * scale;
		float scaleX = NUM_WIDTH * scale;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;
		int renderX = x;

		//マイナスの時
		if (workN < 0)
		{
			//正の値に
			workN = -n;
			//マイナス表示
			num->Render(renderX, y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
			renderX += (int)scaleX;
		}

		//桁越えガード(最大桁数を９で埋める)
		if (workN >= workB * 10)workN = (workB * 10) - 1;

		bool cutFlg = true;

		for (int i = 0; i < beam; i++){
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			if (cutFlg && beamf && i < beam - 1 && workNum <= 0)continue;
			num->RenderAlpha(renderX + ((int)scaleX * i), y, (int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0, NUM_WIDTH, NUM_HEIGHT, 0.0f, alpha);
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
			if (cutFlg)cutFlg = false;
		}
	}
	//通常描画アルファありVec2版
	void RenderAlpha(Vector2 rPos, float scale, int beam, bool beamf, int n, float alpha) const
	{
		RenderAlpha(rPos.x, rPos.y, scale, beam, beamf, n, alpha);
	}
	
	//浮動小数点数描画
	void Render(int x, int y, float scale, int dotUpper, int dotUnder, float n) const
	{
		int srcY = (int)(NUM_HEIGHT * scale);
		int scaleX = (int)(NUM_WIDTH * scale);
		int workB = 1;
		for (int i = 1; i < dotUpper; i++)workB *= 10;
		int workNum = 0;
		int renderX = x;
		float baseNum = n;

		//マイナスの時
		if (baseNum < 0.0f)
		{
			//マイナス表示
			num->Render(renderX, y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
			renderX += scaleX;
			//正の値に
			baseNum *= -1.0f;
		}

		int workN = (int)baseNum;

		//桁越えガード(最大桁数を９で埋める)
		if (workN >= workB * 10)
		{
			workN = (workB * 10) - 1;
		}

		//小数点より上を描画
		if (workN <= 0)
		{
			//０の時は専用描画
			num->Render(renderX, y, (int)scaleX, (int)srcY, 0, 0, NUM_WIDTH, NUM_HEIGHT);
			renderX += scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
		else
		{
			//通常描画
			bool skipFlag = true;
			for (int i = 0; i < dotUpper; i++)
			{
				workNum = workN / workB;
				if (skipFlag && workNum <= 0)
				{
					workB /= 10;
					continue;
				}
				workN -= workNum * workB;
				workB /= 10;//↑の後で割る
				num->Render(renderX, y, (int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0, NUM_WIDTH, NUM_HEIGHT);
				renderX += scaleX;
				if (skipFlag)skipFlag = false;
#if DEVELOP_DEBUG
				Vitalize2DObjDrive::stringDrawCallCount++;
#endif
			}
		}

		//小数点表示
		num->Render(renderX, y, (int)scaleX, (int)srcY, NUM_WIDTH, 64, NUM_WIDTH, NUM_HEIGHT);
		renderX += scaleX;
#if DEVELOP_DEBUG
		Vitalize2DObjDrive::stringDrawCallCount++;
#endif

		//小数点より上を消す
		workN = (int)baseNum;
		float upperCut = baseNum - (float)workN;

		//小数点以下を描画する準備
		workB = 1;
		for (int i = 1; i < dotUnder; i++)workB *= 10;
		workN = (int)(upperCut * (workB * 10)); //小数点以下を整数にするのでさらに１桁上げる必要ある

		//小数点より下を描画
		for (int i = 0; i < dotUnder; i++)
		{
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			num->Render(renderX, y, (int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0, NUM_WIDTH, NUM_HEIGHT);
			renderX += scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

	}
	
	//スケール縦横指定版
	void RenderScale(Vector2 rPos, Vector2f rScale, int beam, bool beamf, int n, float alpha) const
	{
		float srcY = NUM_HEIGHT * rScale.x;
		float scaleX = NUM_WIDTH * rScale.y;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;
		int renderX = rPos.x;
		
		//マイナスの時
		if (workN < 0)
		{
			//正の値に
			workN = -n;
			//マイナス表示
			num->Render(renderX, rPos.y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		//桁越えガード(最大桁数を９で埋める)
		if (workN >= workB * 10)workN = (workB * 10) - 1;

		bool cutFlg = true;

		for (int i = 0; i < beam; i++) {
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			if (cutFlg && beamf && i < beam - 1 && workNum <= 0)continue;
			num->RenderAlpha(renderX + ((int)scaleX * i), rPos.y, (int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0, NUM_WIDTH, NUM_HEIGHT, 0.0f, alpha);
			if (cutFlg)cutFlg = false;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
	}
	void RenderScale(Vector2 rPos, Vector2f rScale, int beam, bool beamf, int n, const Color& col = Color::White) const
	{
		float srcY = NUM_HEIGHT * rScale.x;
		float scaleX = NUM_WIDTH * rScale.y;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;
		int renderX = rPos.x;

		//マイナスの時
		if (workN < 0)
		{
			//正の値に
			workN = -n;
			//マイナス表示
			num->RenderColor(renderX, rPos.y, (int)scaleX, (int)srcY,
				0, 64, NUM_WIDTH, NUM_HEIGHT, col);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		//桁越えガード(最大桁数を９で埋める)
		if (workN >= workB * 10)workN = (workB * 10) - 1;

		bool cutFlg = true;

		for (int i = 0; i < beam; i++) {
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			if (cutFlg && beamf && i < beam - 1 && workNum <= 0)continue;
			num->RenderColor(renderX + ((int)scaleX * i), rPos.y,
				(int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0,
				NUM_WIDTH, NUM_HEIGHT, col);
			if (cutFlg)cutFlg = false;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
	}

	//2Dスケール適用描画
	void RenderScalable(Vector2 rPos, float scale, int beam, bool beamf, int n, float alpha) const
	{
		const Vector2f posScale = Vitalize2DObjScalable::getSystemRednerPosScale();
		float srcY = NUM_HEIGHT * scale * posScale.x;
		float scaleX = NUM_WIDTH * scale * posScale.y;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;
		int renderX = (int)(rPos.x * posScale.x);
		const int renderY = (int)(rPos.y * posScale.y);

		//マイナスの時
		if (workN < 0)
		{
			//正の値に
			workN = -n;
			//マイナス表示
			num->Render(renderX, rPos.y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		//桁越えガード(最大桁数を９で埋める)
		if (workN >= workB * 10)workN = (workB * 10) - 1;

		bool cutFlg = true;

		for (int i = 0; i < beam; i++) {
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			if (cutFlg && beamf && i < beam - 1 && workNum <= 0)continue;
			num->RenderAlpha(renderX + ((int)scaleX * i), renderY, (int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0, NUM_WIDTH, NUM_HEIGHT, 0.0f, alpha);
			if (cutFlg)cutFlg = false;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
	}
	//2Dスケール&座標補正適用描画
	void RenderResolution(Vector2 rPos, float scale, int beam, bool beamf,
		int n, const Color& color = (Color::White), bool plusSign = (false)) const
	{
		const Vector2 posOffset = Vitalize2DObjResolution::getSystemRednerPosOffset();
		const Vector2f posScale = Vitalize2DObjScalable::getSystemRednerPosScale();
		float srcY = NUM_HEIGHT * scale * posScale.x;
		float scaleX = NUM_WIDTH * scale * posScale.y;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;
		int renderX = posOffset.x + (int)(rPos.x * posScale.x);
		const int renderY = posOffset.y + (int)(rPos.y * posScale.y);

		//マイナスの時
		if (workN < 0)
		{
			//正の値に
			workN = -n;
			//マイナス表示
			num->RenderColor(renderX, renderY, (int)scaleX, (int)srcY,
				0, 64, NUM_WIDTH, NUM_HEIGHT, color);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
		else if (plusSign)
		{
			//プラス表示
			if (workN != 0)
			{
				num->RenderColor(renderX, renderY, (int)scaleX, (int)srcY,
					90, 64, NUM_WIDTH, NUM_HEIGHT, color);
#if DEVELOP_DEBUG
				Vitalize2DObjDrive::stringDrawCallCount++;
#endif
			}
			//表示してなくても＋表示あり設定なら進める
			renderX += (int)scaleX;
		}

		//桁越えガード(最大桁数を９で埋める)
		if (workN >= workB * 10)workN = (workB * 10) - 1;

		bool cutFlg = true;

		for (int i = 0; i < beam; i++) {
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			if (cutFlg && beamf && i < beam - 1 && workNum <= 0)continue;
			num->RenderColor(renderX + ((int)scaleX * i), renderY, 
				(int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0,
				NUM_WIDTH, NUM_HEIGHT, color);
			if (cutFlg)cutFlg = false;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
	}

	//2Dスケール&座標補正適用描画
	int RenderConnextString(Vector2 rPos, float scale, int beam, bool beamf, int n, float alpha) const
	{
		const Vector2 posOffset = Vitalize2DObjResolution::getSystemRednerPosOffset();
		const Vector2f posScale = Vitalize2DObjScalable::getSystemRednerPosScale();
		float srcY = NUM_HEIGHT * scale * posScale.x;
		float scaleX = NUM_WIDTH * scale * posScale.y;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;
		int renderX = rPos.x;
		const int renderY = posOffset.y + (int)(rPos.y * posScale.y);

		//マイナスの時
		if (workN < 0)
		{
			//正の値に
			workN = -n;
			//マイナス表示
			num->Render(renderX, rPos.y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		//桁越えガード(最大桁数を９で埋める)
		if (workN >= workB * 10)workN = (workB * 10) - 1;

		bool cutFlg = true;

		for (int i = 0; i < beam; i++) {
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			if (cutFlg && beamf && i < beam - 1 && workNum <= 0)continue;
			num->RenderAlpha(renderX, renderY, (int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0, NUM_WIDTH, NUM_HEIGHT, 0.0f, alpha);
			renderX += (int)scaleX;
			if (cutFlg)cutFlg = false;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		return renderX;
	}


	//廃止予定
	//数字テクスチャ指定版(負の値は表示できません)
	void Render(Vitalize2DObj* numTex, int lx, int ly, int rx, int ry, int x, int y, float scale, int beam, bool beamf, int n)
	{
		int workX = (rx - lx) / 10;
		int workY = ry - ly;
		float srcY = workY * scale;
		float scaleX = workX * scale;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;

		if (workN >= workB * 10)workN = (workB * 10) - 1;

		bool cutFlg = true;

		for (int i = 0; i < beam; i++){
			workNum = workN / workB;
			workN -= workNum * workB;
			workB /= 10;
			if (cutFlg && beamf && i < beam - 1 && workNum <= 0)continue;
			numTex->Render(x + ((int)scaleX * i), y, (int)scaleX, (int)srcY, lx + (workX * workNum), ly, workX, workY, 0.0f);
			if (cutFlg)cutFlg = false;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
	}


};


#endif

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

	//�ʏ�`��
	void Render(int x, int y, float scale, int beam, bool beamf, int n, const Color& col = Color::White) const
	{
		float srcY = NUM_HEIGHT * scale;
		float scaleX = NUM_WIDTH * scale;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;

		//�}�C�i�X�̎�
		if (workN < 0)
		{
			//���̒l��
			workN = -n;
		}

		//���z���K�[�h(�ő包�����X�Ŗ��߂�)
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
					//�}�C�i�X�\��
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
	//�ʏ�`��Vec2��
	void Render(Vector2 rPos, float scale, int beam, bool beamf, int n) const
	{
		Render(rPos.x, rPos.y, scale, beam, beamf, n);
	}
	//�ʏ�`��A���t�@����
	void RenderAlpha(int x, int y, float scale, int beam, bool beamf, int n, float alpha) const
	{
		float srcY = NUM_HEIGHT * scale;
		float scaleX = NUM_WIDTH * scale;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;
		int renderX = x;

		//�}�C�i�X�̎�
		if (workN < 0)
		{
			//���̒l��
			workN = -n;
			//�}�C�i�X�\��
			num->Render(renderX, y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
			renderX += (int)scaleX;
		}

		//���z���K�[�h(�ő包�����X�Ŗ��߂�)
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
	//�ʏ�`��A���t�@����Vec2��
	void RenderAlpha(Vector2 rPos, float scale, int beam, bool beamf, int n, float alpha) const
	{
		RenderAlpha(rPos.x, rPos.y, scale, beam, beamf, n, alpha);
	}
	
	//���������_���`��
	void Render(int x, int y, float scale, int dotUpper, int dotUnder, float n) const
	{
		int srcY = (int)(NUM_HEIGHT * scale);
		int scaleX = (int)(NUM_WIDTH * scale);
		int workB = 1;
		for (int i = 1; i < dotUpper; i++)workB *= 10;
		int workNum = 0;
		int renderX = x;
		float baseNum = n;

		//�}�C�i�X�̎�
		if (baseNum < 0.0f)
		{
			//�}�C�i�X�\��
			num->Render(renderX, y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
			renderX += scaleX;
			//���̒l��
			baseNum *= -1.0f;
		}

		int workN = (int)baseNum;

		//���z���K�[�h(�ő包�����X�Ŗ��߂�)
		if (workN >= workB * 10)
		{
			workN = (workB * 10) - 1;
		}

		//�����_�����`��
		if (workN <= 0)
		{
			//�O�̎��͐�p�`��
			num->Render(renderX, y, (int)scaleX, (int)srcY, 0, 0, NUM_WIDTH, NUM_HEIGHT);
			renderX += scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
		else
		{
			//�ʏ�`��
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
				workB /= 10;//���̌�Ŋ���
				num->Render(renderX, y, (int)scaleX, (int)srcY, NUM_WIDTH * workNum, 0, NUM_WIDTH, NUM_HEIGHT);
				renderX += scaleX;
				if (skipFlag)skipFlag = false;
#if DEVELOP_DEBUG
				Vitalize2DObjDrive::stringDrawCallCount++;
#endif
			}
		}

		//�����_�\��
		num->Render(renderX, y, (int)scaleX, (int)srcY, NUM_WIDTH, 64, NUM_WIDTH, NUM_HEIGHT);
		renderX += scaleX;
#if DEVELOP_DEBUG
		Vitalize2DObjDrive::stringDrawCallCount++;
#endif

		//�����_���������
		workN = (int)baseNum;
		float upperCut = baseNum - (float)workN;

		//�����_�ȉ���`�悷�鏀��
		workB = 1;
		for (int i = 1; i < dotUnder; i++)workB *= 10;
		workN = (int)(upperCut * (workB * 10)); //�����_�ȉ��𐮐��ɂ���̂ł���ɂP���グ��K�v����

		//�����_��艺��`��
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
	
	//�X�P�[���c���w���
	void RenderScale(Vector2 rPos, Vector2f rScale, int beam, bool beamf, int n, float alpha) const
	{
		float srcY = NUM_HEIGHT * rScale.x;
		float scaleX = NUM_WIDTH * rScale.y;
		int workB = 1;
		for (int i = 1; i < beam; i++)workB *= 10;
		int workN = n;
		int workNum = 0;
		int renderX = rPos.x;
		
		//�}�C�i�X�̎�
		if (workN < 0)
		{
			//���̒l��
			workN = -n;
			//�}�C�i�X�\��
			num->Render(renderX, rPos.y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		//���z���K�[�h(�ő包�����X�Ŗ��߂�)
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

		//�}�C�i�X�̎�
		if (workN < 0)
		{
			//���̒l��
			workN = -n;
			//�}�C�i�X�\��
			num->RenderColor(renderX, rPos.y, (int)scaleX, (int)srcY,
				0, 64, NUM_WIDTH, NUM_HEIGHT, col);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		//���z���K�[�h(�ő包�����X�Ŗ��߂�)
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

	//2D�X�P�[���K�p�`��
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

		//�}�C�i�X�̎�
		if (workN < 0)
		{
			//���̒l��
			workN = -n;
			//�}�C�i�X�\��
			num->Render(renderX, rPos.y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		//���z���K�[�h(�ő包�����X�Ŗ��߂�)
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
	//2D�X�P�[��&���W�␳�K�p�`��
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

		//�}�C�i�X�̎�
		if (workN < 0)
		{
			//���̒l��
			workN = -n;
			//�}�C�i�X�\��
			num->RenderColor(renderX, renderY, (int)scaleX, (int)srcY,
				0, 64, NUM_WIDTH, NUM_HEIGHT, color);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}
		else if (plusSign)
		{
			//�v���X�\��
			if (workN != 0)
			{
				num->RenderColor(renderX, renderY, (int)scaleX, (int)srcY,
					90, 64, NUM_WIDTH, NUM_HEIGHT, color);
#if DEVELOP_DEBUG
				Vitalize2DObjDrive::stringDrawCallCount++;
#endif
			}
			//�\�����ĂȂ��Ă��{�\������ݒ�Ȃ�i�߂�
			renderX += (int)scaleX;
		}

		//���z���K�[�h(�ő包�����X�Ŗ��߂�)
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

	//2D�X�P�[��&���W�␳�K�p�`��
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

		//�}�C�i�X�̎�
		if (workN < 0)
		{
			//���̒l��
			workN = -n;
			//�}�C�i�X�\��
			num->Render(renderX, rPos.y, (int)scaleX, (int)srcY, 0, 64, NUM_WIDTH, NUM_HEIGHT);
			renderX += (int)scaleX;
#if DEVELOP_DEBUG
			Vitalize2DObjDrive::stringDrawCallCount++;
#endif
		}

		//���z���K�[�h(�ő包�����X�Ŗ��߂�)
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


	//�p�~�\��
	//�����e�N�X�`���w���(���̒l�͕\���ł��܂���)
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

#pragma once

#include "VitalizeSystem.h"
#include "VitalizeMath.h"
#include "VitalizeShapes2D.h"

struct Vertex2D
{
	float pos[3];	//x-y-z
	float col[4];	//r-g-b-a
	float tex[2];	//x-y
};

class Color
{
public:
	float r, g, b, a;
	Color() { r = g = b = a = 1.0f; }
	Color(float rgb, float alpha = (1.0f)) { Init(rgb, rgb, rgb, alpha); }
	Color(int rgb) 
	{ 
		const float val = (float)rgb / 255.0f;
		Init(val, val, val, 1.0f);
	}
	Color(float red, float green, float blue) { Init(red, green, blue, 1.0f); }
	Color(float red, float green, float blue, float alpha){ Init(red, green, blue, alpha); }
	Color(int red, int green, int blue, float alpha = (1.0f))
	{ 
		Init((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f, alpha);
	}

	void Init(float red, float green, float blue, float alpha)
	{
		auto clamp = [this](float val) -> float
		{
			if (val > 1.0f)return 1.0f;
			if (val < 0.0f)return 0.0f;
			return val;
		};
		r = clamp(red); g = clamp(green); b = clamp(blue); a = clamp(alpha);
	}

	//�A���t�@��ύX�����F���擾�i�������o�[�ϐ��̒l�͕ς��Ȃ��j
	Color getChangeAlphaColor(float alpha) const { return Color(r, g, b, alpha); }

	//�F�̌v�Z�i�����x�͌v�Z�Ɋ܂߂Ȃ����́j
	inline Color operator + (const Color& c) const { return Color(r + c.r, g + c.g, b + c.b, a); }
	inline Color operator - (const Color& c) const { return Color(r - c.r, g - c.g, b - c.b, a); }
	//�F�̌v�Z�i�����x���v�Z�Ɋ܂ނ��́j
	inline Color operator * (const float& v) const { return Color(r * v, g * v, b * v, a * v); }

	inline Color& operator = (const Color& c) 
	{
		this->r = c.r; this->g = c.g; this->b = c.b; this->a = c.a; return *this;
	}

	//���`�⊮
	static Color lerp(const Color& from, const Color& to, float rate, float alpha = (1.0f))
	{
		const float reverseRate = 1.0f - rate;
		return Color(
			(from.r * reverseRate) + (to.r * rate), 
			(from.g * reverseRate) + (to.g * rate),
			(from.b * reverseRate) + (to.b * rate), alpha);
	}
	inline Color lerp(const Color& target, float rate) const
	{
		return (Color(r, g, b, 1.0f) * (1.0f - rate)).add(Color(target.r, target.g, target.b, 1.0f) * rate);
	}

	//���`�⊮(�A���t�@����)
	inline Color lerpA(const Color& target, float rate) const
	{
		return (Color(r,g,b,a) * (1.0f - rate)).add(target * rate);
	}
	//���Z�i+�ƈႢalpha�����Z�����j
	inline Color add(const Color& target) const
	{
		return Color(r + target.r, g + target.g, b + target.b, a + target.a);
	}

	//�Œ�F��`
#pragma region __BaseColor
	static const Color Red;
	static const Color Pink;
	static const Color DeepPink;
	static const Color Fuchsia;
	static const Color Purple;
	static const Color Maroon;
	static const Color Orange;

	static const Color Green;
	static const Color Lime;
	static const Color Olive;
	static const Color Teal;
	
	static const Color Blue;
	static const Color Aqua;
	static const Color Navy;
	
	static const Color Cyan;
	static const Color Magenta;
	static const Color Yellow;

	static const Color Curry;
	static const Color Cafe;

	static const Color White;
	static const Color Black;
	static const Color Gray;
	static const Color Silver;
#pragma endregion

};

class Vitalize2DObjDrive
{
public:
	static bool Initialize();
	static void Release();

	static ID3D11Buffer*				IndexBuffer;
	static ID3D11SamplerState*			SampleLinear;	//�e�N�X�`���[�̃T���v���[

	//�f�o�b�O�p
#if DEVELOP_DEBUG
	static unsigned int totalDrawCallCount;
	static unsigned int stringDrawCallCount;
	static unsigned int shapes2DDrawCallCount;
	static unsigned int particleDrawCallCount;
	static void resetDrawCallCount()
	{
		totalDrawCallCount = 0;
		stringDrawCallCount = 0;
		shapes2DDrawCallCount = 0;
		particleDrawCallCount = 0;
	}
#endif

	enum class BlendState : UINT
	{
		NONE = 0,
		DEFAULT,
		ALIGNMENT,
		ADD,
		SUB,
		MULTIPLE,

		STENCIL_MASK,
		STENCIL_TEST,

		MAX,
	};

	// �u�����h�X�e�[�g��ݒ肷��
	static void SetBlendState(BlendState state = (BlendState::DEFAULT));

private:
	static HRESULT SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC BlendStateArray[], UINT NumBlendState, BOOL AlphaToCoverageEnable);

};

class Vitalize2DObj
{
private:
	Vitalize2DObj() : VertexBuffer(nullptr), pSRView(nullptr) {}

public:				  
	Vitalize2DObj(char* fname) : Vitalize2DObj() { Initialize(fname); }
	Vitalize2DObj(const char* fname) : Vitalize2DObj() { Initialize(fname); }
	Vitalize2DObj(char* fname, bool fullPath) : Vitalize2DObj() 
	{
		if (fullPath)	InitializeFullPath(fname);
		else			Initialize(fname);
	}
	Vitalize2DObj(const char* fname, bool fullPath) : Vitalize2DObj() 
	{
		if (fullPath)	InitializeFullPath(fname);
		else			Initialize(fname);
	}
	virtual ~Vitalize2DObj(){ Release(); }

	virtual void Render(const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT));
	virtual void Render(int screen_x, int screen_y, float scale,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT));
	
	//���폜�\��@(��������FORA�Ή�������)
	virtual void Render(int screen_x, int screen_y, int tex_Lx, int tex_Ly, int tex_sizeX, int tex_sizeY,
		float scale, const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT));
	
	virtual void Render(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT));
	virtual void Render(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float rotate, 
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT));

	virtual void RenderAlpha(float alpha);
	virtual void RenderAlpha(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float alpha);
	virtual void RenderAlpha(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float rotate, float alpha);

	virtual void RenderColor(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col);
	virtual void RenderColor(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col, const Vitalize2DObjDrive::BlendState& RenderState);

	virtual VitalizeShapes2D::FixedRectangle RenderGetRectangle(
		int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT));

	virtual void RenderVertex(
		const Vector2f& leftup, const Vector2f& rightup, const Vector2f& leftbottom, const Vector2f& rightbuttom,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col);

	virtual void RenderTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2,
		const Vector2& uv0, const Vector2& uv1, const Vector2& uv2, const Color& col);

	void RenderBasic(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry)
	{
		Vitalize2DObj::Render(screen_lx, screen_ly, screen_SizeX, screen_SizeY, tex_Lx, tex_Ly, size_Rx, size_Ry);
	}
	void RenderBasicAlpha(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float alpha)
	{
		Vitalize2DObj::RenderAlpha(screen_lx, screen_ly, screen_SizeX, screen_SizeY, tex_Lx, tex_Ly, size_Rx, size_Ry, alpha);
	}
	void RenderBasicColor(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col)
	{
		Vitalize2DObj::RenderColor(screen_lx, screen_ly, screen_SizeX, screen_SizeY, tex_Lx, tex_Ly, size_Rx, size_Ry, col);
	}
	VitalizeShapes2D::FixedRectangle RenderBasicGetRectangle(
		int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT))
	{
		return Vitalize2DObj::RenderGetRectangle(
			screen_lx, screen_ly, screen_SizeX, screen_SizeY,
			tex_Lx, tex_Ly, size_Rx, size_Ry, col, RenderState);
	}

	struct RenderInfo final
	{
		Vector2 screen_leftup; Vector2 screen_size;
		Vector2 tex_leftup; Vector2 tex_size;
		Color color; Vitalize2DObjDrive::BlendState RenderState;
		float rotateRad; Vector2f pivot;
		RenderInfo()
		{
			screen_leftup = tex_leftup = Vector2(0);
			screen_size = tex_size = Vector2(100); color = Color::White;
			RenderState = Vitalize2DObjDrive::BlendState::DEFAULT;
			rotateRad = 0.0f; pivot = Vector2f(0.0f);
		}
	};
	virtual void Render(const RenderInfo& info);

	int getWidth() const { return (int)textureWidth; }
	int getHeight() const { return (int)textureHeight; }
	Vector2 getTextureSize() const { return Vector2((int)textureWidth, (int)textureHeight); }

protected:
	Vertex2D vertices[4];
	ID3D11Buffer*				VertexBuffer;
	ID3D11ShaderResourceView*	pSRView;

	UINT textureWidth;
	UINT textureHeight;
	Vector2f texRate;

private:
	bool Initialize(const char* fname);
	bool InitializeFullPath(const char* fname);
	void Release();

};

class Vitalize2DObjScalable : public Vitalize2DObj
{
public:
	Vitalize2DObjScalable(const char* fname) : Vitalize2DObj(fname) { InitVariable(); }
	Vitalize2DObjScalable(const char* fname, bool fullPath) : Vitalize2DObj(fname, fullPath) { InitVariable(); }
	virtual ~Vitalize2DObjScalable() {}

	virtual void Render(int screen_x, int screen_y, float scale,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;
	virtual void Render(int screen_x, int screen_y, int tex_Lx, int tex_Ly, int tex_sizeX, int tex_sizeY,
		float scale, const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;
	virtual void Render(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;
	virtual void Render(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float rotate,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;

	virtual void RenderAlpha(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float alpha) override;
	virtual void RenderAlpha(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float rotate, float alpha) override;

	virtual void RenderColor(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col) override;
	virtual void RenderColor(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col, 
		const Vitalize2DObjDrive::BlendState& RenderState) override;

	virtual VitalizeShapes2D::FixedRectangle RenderGetRectangle(
		int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;

	virtual void RenderVertex(
		const Vector2f& leftup, const Vector2f& rightup, const Vector2f& leftbottom, const Vector2f& rightbuttom,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col) override;
	
	virtual void RenderTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2,
		const Vector2& uv0, const Vector2& uv1, const Vector2& uv2, const Color& col) override;

	//�`��X�P�[����ݒ�
	virtual void setRednerPosScale(Vector2f scale);
	//�`��X�P�[���̎擾
	virtual const Vector2f& getRednerPosScale() const;

private:

	//�ŗL�ϐ��̏�����
	void InitVariable();

protected:

	//�`��X�P�[��
	Vector2f renderPosScale;

	//�O�������p�ŃX�P�[���ݒ肳��Ă��邩
	bool haveSelfScale;

	//�V�X�e������ݒ肳���`��X�P�[���irenderPosScale�̏����l�ɂȂ�܂��j
	static Vector2f systemRednerPosScale;

public:

	//�V�X�e���`��X�P�[���̐ݒ�
	//���V�X�e�����玩���ŌĂ΂�Ă��܂��B���̊֐��͌Ăяo���Ȃ��ł��������B
	//�@�ʂɕ`��X�P�[����ω����������ꍇ��setRednerPosScale�֐����g�p���Ă��������B
	static void setSystemRednerPosScale(Vector2f scale) { systemRednerPosScale = scale; }

	//�V�X�e���`��X�P�[���̎擾
	static Vector2f getSystemRednerPosScale() { return systemRednerPosScale; }

	//���݂̃X�N���[���T�C�Y����g�k�䗦���v�Z(16:9�X�N���[���O��)
	static Vector2f calcScreenScaleRate(Vector2f baseScreenSize);
	static float calcScreenScaleRate( float baseScreenSizeX );

};

class Vitalize2DObjResolution : public Vitalize2DObjScalable
{
public:
	Vitalize2DObjResolution(const char* fname)
		: Vitalize2DObjScalable(fname) {}
	Vitalize2DObjResolution(const char* fname, bool fullPath)
		: Vitalize2DObjScalable(fname, fullPath) {}
	virtual ~Vitalize2DObjResolution() {}

	void Render(int screen_x, int screen_y, float scale,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;
	void Render(int screen_x, int screen_y, int tex_Lx, int tex_Ly, int tex_sizeX, int tex_sizeY,
		float scale, const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;
	void Render(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;
	void Render(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float rotate,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;

	void RenderAlpha(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float alpha) override;
	void RenderAlpha(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, float rotate, float alpha) override;

	virtual void RenderColor(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col) override;
	virtual void RenderColor(int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col,
		const Vitalize2DObjDrive::BlendState& RenderState) override;

	virtual VitalizeShapes2D::FixedRectangle RenderGetRectangle(
		int screen_lx, int screen_ly, int screen_SizeX, int screen_SizeY,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col,
		const Vitalize2DObjDrive::BlendState& RenderState = (Vitalize2DObjDrive::BlendState::DEFAULT)) override;

	virtual void RenderVertex(
		const Vector2f& leftup, const Vector2f& rightup, const Vector2f& leftbottom, const Vector2f& rightbuttom,
		int tex_Lx, int tex_Ly, int size_Rx, int size_Ry, const Color& col) override;

	virtual void RenderTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2,
		const Vector2& uv0, const Vector2& uv1, const Vector2& uv2, const Color& col) override;


private:

	//�V�X�e������ݒ肳���`��J�n�I�t�Z�b�g�ʒu
	static Vector2 systemRednerPosOffset;

public:

	//�V�X�e���`��J�n�I�t�Z�b�g�̐ݒ�
	//���V�X�e�����玩���ŌĂ΂�Ă��܂��B���̊֐��͌Ăяo���Ȃ��ł��������B
	static void setSystemRednerPosOffset(Vector2 offset) { systemRednerPosOffset = offset; }

	//�V�X�e���`��J�n�I�t�Z�b�g�̎擾
	//���A�v�����ł̌Ăяo���͑z�肵�Ă��܂���B
	static Vector2 getSystemRednerPosOffset() { return systemRednerPosOffset; }

};

#if DEVELOP_DEBUG_PC

class ManagedResource
{
public:
	ManagedResource();
	virtual ~ManagedResource();

	virtual const char* getName() = 0;
	virtual void inspectorProperty() = 0;

private:

};

class Vitalize2DObjR : public Vitalize2DObjResolution, public ManagedResource
{
public:
	Vitalize2DObjR(const char* fname);
	Vitalize2DObjR(const char* fname, bool fullPath);
	virtual ~Vitalize2DObjR() {}

	const char* getName() override;
	void inspectorProperty() override;

private:
	static const int NameBufferSize = 128;
	char nameBuffer[NameBufferSize];
	void createName(const char* fname);

};

class ResourceManager
{
public:
	static void registerResource(ManagedResource* obj);
	static void unregisterResource(ManagedResource* obj);

	static std::vector<ManagedResource*>& getResourceList() { return _ResourceList; }
	static ManagedResource* getResource(int id);
	static const char** getNameList() { return _NameList; }
	static int getNameListSize() { return _NameListSize; }

	static void createNameList();
	static void clearNameList();

private:
	friend class VitalizeFramework;

	static void Initialize();
	static void Release();
	//static void Update();

	static std::vector<ManagedResource*> _ResourceList;
	static const char** _NameList;
	static int _NameListSize;

#pragma region _Singleton
public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
private:
	ResourceManager() {}
	~ResourceManager() = default;
#pragma endregion
};

//2DObj�N���X�Z���N�^
using Vitalize2DObjS = Vitalize2DObjR;

#else

//2DObj�N���X�Z���N�^
using Vitalize2DObjS = Vitalize2DObjResolution;

//PC�J���p�Ȃ̂ŏ���
#define VIRTUAL_OBJ
#define VIRTUAL_OBJ_MULTI

#endif





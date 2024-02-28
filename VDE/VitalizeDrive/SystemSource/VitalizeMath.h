#pragma once

#include	<string.h>
#include	<math.h>
#include	<random>

//*****************************************************************************************************************************
//
//	ゲーム数学関連
//
//*****************************************************************************************************************************
#pragma region Math

#define	PI	((float)3.141592654f)
#define RADIAN_TO_DEGREE(RAD) ((float)((RAD)*(180.0f/PI)))
#define DEGREE_TO_RADIAN(DEG) ((float)((DEG)*(PI/180.0f)))
#define ABSOLUTE_VALUE_F(VAL) (((VAL) >= 0.0f) ? (VAL) : -(VAL))
#define ABSOLUTE_VALUE_I(VAL) (((VAL) >= 0) ? (VAL) : -(VAL))

//2Dベクトル拡張**********************************************************************************************************
typedef struct baseVec2i
{
	int x;
	int y;

}baseVec2i;

typedef struct baseVec2f
{
	float x;
	float y;
}baseVec2f;

struct Vector2f;//プロトタイプ宣言

typedef struct Vector2 : public baseVec2i
{
public:
	Vector2(){};

	inline Vector2(int x, int y){ this->x = x, this->y = y; }
	inline Vector2(int xy) { this->x = xy, this->y = xy; }
	inline Vector2(const Vector2& v){ this->x = v.x, this->y = v.y; }
	inline Vector2(const baseVec2f& v){ this->x = (int)v.x, this->y = (int)v.y; }

	// 距離計算
	inline float length() const { return sqrtf((float)(x*x + y*y)); }
	inline float lengthSq() const { return (float)(x*x + y*y); }
	//昔はなぜか先頭大文字関数だった
	inline float Length() const { return sqrtf((float)(x * x + y * y)); }
	inline float LengthSq() const { return (float)(x * x + y * y); }

	// 垂直ベク(左)を取得（法線方向・非正規化）
	inline Vector2 getLeftVec() const { return Vector2(y, -x); }
	inline Vector2 getRightVec() const { return Vector2(-y, x); }

	inline Vector2& operator = (const baseVec2i& v){ x = v.x; y = v.y; return *this; }
	inline Vector2& operator += (const Vector2& v){ x += v.x; y += v.y; return *this; }
	inline Vector2& operator -= (const Vector2& v){ x -= v.x; y -= v.y; return *this; }
	inline Vector2& operator *= (int v){ x *= v; y *= v; return *this; }
	inline Vector2& operator /= (int v){ x /= v; y /= v; return *this; }
	inline Vector2 operator + () const { return Vector2(x, y); }
	inline Vector2 operator - () const { return Vector2(-x, -y); }
	inline Vector2 operator + (const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	inline Vector2 operator - (const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	inline Vector2 operator * (const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
	inline Vector2 operator * (int v) const { return Vector2(x*v, y*v); }
	inline Vector2 operator * (float v) const { return Vector2((int)(x*v), (int)(y*v)); }
	inline Vector2 operator / (int v) const { return Vector2(x / v, y / v); }

	//Vec2（整数）だけ特別
	inline bool operator == (const Vector2& v) const { return (x == v.x && y == v.y); }

	static const Vector2 Zero;
	static const Vector2 Up;
	static const Vector2 Down;
	static const Vector2 Left;
	static const Vector2 Right;

	//float互換
	inline Vector2& operator = (const baseVec2f& v){ x = (int)v.x; y = (int)v.y; return *this; }
	inline Vector2& operator += (const baseVec2f& v){ x += (int)v.x; y += (int)v.y; return *this; }
	inline Vector2& operator -= (const baseVec2f& v){ x -= (int)v.x; y -= (int)v.y; return *this; }
	inline Vector2 operator + (const baseVec2f& v) const { return Vector2(x + (int)v.x, y + (int)v.y); }
	inline Vector2 operator - (const baseVec2f& v) const { return Vector2(x - (int)v.x, y - (int)v.y); }
	inline Vector2 operator * (const baseVec2f& v) const { return Vector2((int)((float)x*v.x), (int)((float)y*v.y)); }

}Vector2;

typedef struct Vector2i : public Vector2{
public:
	Vector2i(){};
}Vector2i;

typedef struct Vector2f : public baseVec2f
{
public:
	Vector2f(){};

	inline Vector2f(int x, int y) { this->x = (float)x, this->y = (float)y; }
	inline Vector2f(int xy) { this->x = (float)xy, this->y = (float)xy; }
	inline Vector2f(float x, float y){ this->x = x, this->y = y; }
	inline Vector2f(float xy) { this->x = xy, this->y = xy; }
	inline Vector2f(const Vector2f& v){ this->x = v.x, this->y = v.y; }
	inline Vector2f(const Vector2& v){ this->x = (float)v.x, this->y = (float)v.y; }

	//	距離計算
	inline float length() const { return sqrtf(x * x + y * y); }
	inline float lengthSq() const { return x * x + y * y; }
	//昔はなぜか先頭大文字関数だった
	inline float Length() const { return sqrtf(x*x + y*y); }
	inline float LengthSq() const { return x*x + y*y; }

	//	正規化
	inline void Normalize()
	{
		float l = Length();
		if (l != .0f){ x /= l; y /= l; }
	}
	inline void Normalize(float length)
	{
		if (length != .0f){ x /= length; y /= length; }
	}
	inline Vector2f NormalizeReturn() const
	{
		float l = Length();
		return (l != .0f) ? Vector2f(x, y) / l : Vector2f(0);
	}
	inline Vector2f NormalizeReturn(float length) const
	{
		return (length != .0f) ? Vector2f(x, y) / length : Vector2f(0);
	}

	// 垂直ベクを取得（法線方向・非正規化）
	inline Vector2f getLeftVec() const { return Vector2f(y, -x); }
	inline Vector2f getRightVec() const { return Vector2f(-y, x); }

	inline float Dot(const Vector2f &vec2) const { return (this->x*vec2.x + this->y*vec2.y); }
	inline float Cross(const Vector2f &vec2) const { return (this->x*vec2.y - this->y*vec2.x); }

	inline Vector2f& operator = (const baseVec2f& v){ x = v.x; y = v.y; return *this; }
	inline Vector2f& operator += (const Vector2f& v){ x += v.x; y += v.y; return *this; }
	inline Vector2f& operator -= (const Vector2f& v){ x -= v.x; y -= v.y; return *this; }
	inline Vector2f& operator *= (float v){ x *= v; y *= v; return *this; }
	inline Vector2f& operator /= (float v){ x /= v; y /= v; return *this; }
	inline Vector2f operator + () const { return Vector2f(x, y); }
	inline Vector2f operator - () const { return Vector2f(-x, -y); }
	inline Vector2f operator + (const Vector2f& v) const { return Vector2f(x + v.x, y + v.y); }
	inline Vector2f operator - (const Vector2f& v) const { return Vector2f(x - v.x, y - v.y); }
	inline Vector2f operator * (const Vector2f& v) const { return Vector2f(x * v.x, y * v.y); }
	inline Vector2f operator * (int v) const { return Vector2f(x*(float)v, y*(float)v); }
	inline Vector2f operator * (float v) const { return Vector2f(x*v, y*v); }
	inline Vector2f operator / (float v) const { return Vector2f(x / v, y / v); }

	//int互換
	inline Vector2f& operator = (const Vector2& v){ x = (float)v.x; y = (float)v.y; return *this; }
	inline Vector2f& operator += (const Vector2& v){ x += (float)v.x; y += (float)v.y; return *this; }
	inline Vector2f& operator -= (const Vector2& v){ x -= (float)v.x; y -= (float)v.y; return *this; }
	inline Vector2f operator + (const Vector2& v) const { return Vector2f(x + (float)v.x, y + (float)v.y); }
	inline Vector2f operator - (const Vector2& v) const { return Vector2f(x - (float)v.x, y - (float)v.y); }
	inline Vector2f operator * (const Vector2& v) const { return Vector2f(x*(float)v.x, y*(float)v.y); }

	static const Vector2f Zero;
	static const Vector2f Up;
	static const Vector2f Down;
	static const Vector2f Left;
	static const Vector2f Right;

}Vector2f;

inline float Vector2fDot(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

inline float Vector2fCross(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x*v2.y - v1.y*v2.x;
}

//------------------------------------------------------
//	３Ｄベクトル基本構造体
//------------------------------------------------------
typedef struct Vector {
	float	x, y, z;
} Vector;

//------------------------------------------------------
//	３Ｄベクトル構造体
//------------------------------------------------------
typedef struct Vector3 : public Vector
{
public:
	//	コンストラクタ
	Vector3() {};
	inline Vector3(float x, float y, float z){ this->x = x, this->y = y, this->z = z; }
	inline Vector3(const Vector& v){ this->x = v.x, this->y = v.y, this->z = v.z; }

	//	距離計算
	inline float Length(){ return sqrtf(x*x + y*y + z*z); }
	inline float LengthSq(){ return x*x + y*y + z*z; }

	//	正規化
	inline void Normalize()
	{
		float l = Length();
		if (l != .0f){ x /= l; y /= l; z /= l; }
	}

	//	オペレーター
	inline Vector3& operator = (const Vector& v){ x = v.x; y = v.y; z = v.z; return *this; }
	inline Vector3& operator += (const Vector3& v){ x += v.x; y += v.y; z += v.z; return *this; }
	inline Vector3& operator -= (const Vector3& v){ x -= v.x; y -= v.y; z -= v.z; return *this; }
	inline Vector3& operator *= (float v){ x *= v; y *= v; z *= v; return *this; }
	inline Vector3& operator /= (float v){ x /= v; y /= v; z /= v; return *this; }

	inline Vector3 operator + () const { Vector3 ret(x, y, z); return ret; }
	inline Vector3 operator - () const { Vector3 ret(-x, -y, -z); return ret; }

	inline Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	inline Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	inline Vector3 operator * (float v) const { Vector3 ret(x*v, y*v, z*v); return ret; }
	inline Vector3 operator / (float v) const { Vector3 ret(x / v, y / v, z / v); return ret; }

	inline bool operator == (const Vector3& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
	inline bool operator != (const Vector3& v) const { return (x != v.x) || (y != v.y) || (z != v.z); }

} Vector3, *LPVECTOR3;

//------------------------------------------------------
//	外積
//------------------------------------------------------
inline void Vector3Cross(Vector& out, const Vector& v1, const Vector& v2)
{
	out.x = v1.y*v2.z - v1.z*v2.y;
	out.y = v1.z*v2.x - v1.x*v2.z;
	out.z = v1.x*v2.y - v1.y*v2.x;
}

//------------------------------------------------------
//	内積
//------------------------------------------------------
inline float Vector3Dot(const Vector& v1, const Vector& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

//------------------------------------------------------
//	４Ｄベクトル構造体
//------------------------------------------------------
typedef struct Vector4 {
	float	x, y, z, w;
} Vector4;

//*****************************************************************************
//		行列
//*****************************************************************************
typedef struct _MATRIX {
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
		float _m[16];
	};
} MATRIX;

typedef struct Matrix : public MATRIX
{
public:
	Matrix() {};
	Matrix(const float* p){ memcpy(&_11, p, sizeof(MATRIX)); }
	Matrix(const MATRIX& mat){ memcpy(&_11, &mat, sizeof(MATRIX)); }
	Matrix(float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44)
	{
		_11 = f11; _12 = f12; _13 = f13; _14 = f14;
		_21 = f21; _22 = f22; _23 = f23; _24 = f24;
		_31 = f31; _32 = f32; _33 = f33; _34 = f34;
		_41 = f41; _42 = f42; _43 = f43; _44 = f44;
	}

	//配列のオーバーフロー問題があるため封印
	//void Inverse();

	float& operator () (int r, int c){ return m[r][c]; }
	float  operator () (int r, int c) const { return m[r][c]; }

	operator float* (){ return (float *)&_11; }
	operator const float* () const { return (const float *)&_11; }

	Matrix& operator *= (const Matrix&);
	inline Matrix& operator += (const Matrix&);
	inline Matrix& operator -= (const Matrix&);
	inline Matrix& operator *= (float);
	inline Matrix& operator /= (float);

	// unary operators
	inline Matrix operator + () const;
	inline Matrix operator - () const;

	// binary operators
	Matrix operator * (const Matrix&) const;
	inline Matrix operator + (const Matrix&) const;
	inline Matrix operator - (const Matrix&) const;
	inline Matrix operator * (float) const;
	inline Matrix operator / (float) const;

	inline bool operator == (const Matrix&) const;
	inline bool operator != (const Matrix&) const;

} Matrix;

//		変換行列
void	SetTransformMatrixZXY(Matrix *Mat, float x, float y, float z, float ax, float ay, float az);
void	SetTransformMatrixXYZ(Matrix *Mat, float x, float y, float z, float ax, float ay, float az);
//		ビュー変換表列
void	LookAtLH(Matrix& mat, Vector& Eye, Vector& At, Vector& Up);
//		投影変換表列
void	PerspectiveLH(Matrix& mat, float fovY, float aspect, float zn, float zf);
void	OrthoLH(Matrix& mat, float w, float h, float zn, float zf);


//*****************************************************************************
//		クォータニオン
//*****************************************************************************
typedef struct  Quaternion
{
public:
	//------------------------------------------------------
	//	パラメータ
	//------------------------------------------------------
	float	x, y, z, w;

	//------------------------------------------------------
	//	コンストラクタ
	//------------------------------------------------------
	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(float sx, float sy, float sz, float sw) : x(sx), y(sy), z(sz), w(sw) {}
	Quaternion(Vector3& v, float t){ v.Normalize(); x = v.x*sinf(t*0.5f); y = v.y*sinf(t*0.5f); z = v.z*sinf(t*0.5f); w = cosf(t*0.5f); }

	//------------------------------------------------------
	//	生成
	//------------------------------------------------------
	//	単位クォータニオン生成
	void Identity(){ x = y = z = 0; w = 1; }

	//	正規化
	inline void normalize()
	{
		float legnth = getLength();
		if (legnth == .0f) return;
		float invL = 1.0f / legnth;
		(*this) *= invL;
	}

	//------------------------------------------------------
	//	情報取得
	//------------------------------------------------------
	inline float getLength() const{ return sqrtf(x*x + y*y + z*z + w*w); }
	void toMatrix(Matrix& m);
	void fromMatrix(Matrix& mat);

	//------------------------------------------------------
	//	オペレーター
	//------------------------------------------------------
	inline Quaternion operator + () const { Quaternion ret(x, y, z, w); return ret; }
	inline Quaternion operator - () const { Quaternion ret(-x, -y, -z, -w); return ret; }

	//	VS クォータニオン
	inline Quaternion& operator +=(const Quaternion& v){ x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	inline Quaternion& operator -=(const Quaternion& v){ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	inline Quaternion& operator *=(const Quaternion& v)
	{
		x = y * v.z - z * v.y + x * v.w + w * v.x;
		y = z * v.x - x * v.z + y * v.w + w * v.y;
		z = x * v.y - y * v.x + z * v.w + w * v.z;
		w = w * v.w - x * v.x - y * v.y - z * v.z;
		return *this;
	}

	inline Quaternion operator +(const Quaternion& v) const{ return Quaternion(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline Quaternion operator -(const Quaternion& v) const{ return Quaternion(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline Quaternion operator *(const Quaternion& v) const
	{
		return Quaternion(
			y * v.z - z * v.y + x * v.w + w * v.x,
			z * v.x - x * v.z + y * v.w + w * v.y,
			x * v.y - y * v.x + z * v.w + w * v.z,
			w * v.w - x * v.x - y * v.y - z * v.z
			);
	}

	//	VS 値
	inline Quaternion& operator *=(float v){ x *= v; y *= v; z *= v; w *= v; return *this; }
	inline Quaternion& operator /=(float v){ x /= v; y /= v; z /= v; w /= v; return *this; }

	inline Quaternion operator *(float v) const{ return Quaternion(x*v, y*v, z*v, w*v); }
	inline Quaternion operator /(float v) const{ return Quaternion(x / v, y / v, z / v, w / v); }

} Quaternion;

//------------------------------------------------------
//		球面線形補間
//------------------------------------------------------
Quaternion QuaternionSlerp(Quaternion& q, Quaternion& r, float a);


//*****************************************************************************************************************************
//		演算
//*****************************************************************************************************************************
inline void Transform(Vector& out, const Vector& in, const Matrix& mat)
{
	Vector work;
	work.x = in.x * mat._11 + in.y * mat._21 + in.z * mat._31 + mat._41;
	work.y = in.x * mat._12 + in.y * mat._22 + in.z * mat._32 + mat._42;
	work.z = in.x * mat._13 + in.y * mat._23 + in.z * mat._33 + mat._43;

	out = work;
}

inline void Transform3x3(Vector& out, const Vector& in, const Matrix& mat)
{
	Vector work;

	work.x = in.x * mat._11 + in.y * mat._21 + in.z * mat._31;
	work.y = in.x * mat._12 + in.y * mat._22 + in.z * mat._32;
	work.z = in.x * mat._13 + in.y * mat._23 + in.z * mat._33;

	out = work;
}

#pragma endregion Math


namespace VitalizeMathUtility
{
	float getAngleRad(const Vector2f& v1_normalized, const Vector2f& v2_normalized);

	//正規化された法線を元にbaseの反射ベクトルを取得
	inline Vector2f getReflectVec(const Vector2f& base, const Vector2f& normalNormalized)
	{
		return (base - (normalNormalized * (base.Dot(normalNormalized) * 2.0f)));
	}

	inline float lerp(const float& start, const float& end, const float& rate)
	{
		return (start * (1.0f - rate)) + (end * rate);
	}

	inline Vector2f lerp(const Vector2f& start, const Vector2f& end, const float& rate)
	{
		return (start * (1.0f - rate)) + (end * rate);
	}

}

//***********************************************************************
//	乱数
//***********************************************************************
class VitalizeRandomUtility final
{
public:
	static VitalizeRandomUtility& getInstance()
	{
		static VitalizeRandomUtility instance;
		return instance;
	}

	void Initialize();
	//void Release();
	uint32_t get();
	int getRange100();

	VitalizeRandomUtility(const VitalizeRandomUtility&) = delete;
	VitalizeRandomUtility& operator=(const VitalizeRandomUtility&) = delete;
	VitalizeRandomUtility(VitalizeRandomUtility&&) = delete;
	VitalizeRandomUtility& operator=(VitalizeRandomUtility&&) = delete;

private:
	VitalizeRandomUtility() {}
	~VitalizeRandomUtility() = default;
	std::mt19937 randGenerator;
};

//32ビット乱数取得
#define V_RAND VitalizeRandomUtility::getInstance().get()
//０～９９の乱数取得
#define V_RAND100 VitalizeRandomUtility::getInstance().getRange100()

//***********************************************************************
//	シャッフル
//***********************************************************************
template <unsigned int MaxSize>
class VitalizeFixedShuffleUnit final
{
public:
	VitalizeFixedShuffleUnit();
	~VitalizeFixedShuffleUnit();

	int get();

	int* valueArray;
	int arraySize;
};

class VitalizeShuffleUnit final
{
public:
	VitalizeShuffleUnit(int size);
	~VitalizeShuffleUnit();

	int get();

	int* valueArray;
	int remainingArraySize;
	int arraySizeMax;

private:
	VitalizeShuffleUnit() {}
	
};
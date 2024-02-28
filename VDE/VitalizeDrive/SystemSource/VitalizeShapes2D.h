#pragma once

#include	"VitalizeMath.h"

class Color;

namespace VitalizeShapes2D
{
#pragma region _図形

	class Line
	{
	public:
		Line() : pos1(Vector2f(0.0f)), pos2(Vector2f(0.0f)) {}
		Line(Vector2f p1, Vector2f p2) : pos1(p1), pos2(p2) {}

		Vector2f pos1, pos2;
	};

	class Circle
	{
	public:
		Circle() : pos(Vector2f(0.0f)), r(0.0f) {}
		Circle(Vector2f center, float radius) : pos(center), r(radius) {}

		Vector2f pos;
		float r;
	};

	class Rectangle
	{
	public:
		Rectangle()
		{
			pos[0] = Vector2f(0.0f);		pos[1] = Vector2f(1.0f, 0.0f);
			pos[2] = Vector2f(0.0f, 1.0f);	pos[3] = Vector2f(1.0f);
		}
		Rectangle(Vector2f leftup, Vector2f rightup, Vector2f leftbottom, Vector2f rightbuttom)
		{
			pos[0] = leftup;		pos[1] = rightup;
			pos[2] = leftbottom;	pos[3] = rightbuttom;
		}
		Rectangle(const Line& line, float thickness = (1.0f));
		Vector2f getCenterPos() const;

		Vector2f pos[4];

		Vector2f leftUp() const { return pos[0]; }
		Vector2f rightBottom() const { return pos[3]; }
		Vector2f leftBottom() const { return pos[2]; }
		Vector2f rightUp() const { return pos[1]; }
	};

	//Rectangleは矩形→４角が直角という意味なのでFreeSquareへ
	using FreeSquare = Rectangle;

	class FixedRectangle
	{
	public:
		FixedRectangle()
		{
			leftUp = Vector2(0);
			rightBottom = Vector2(1);
		}
		FixedRectangle(Vector2 leftup, Vector2 size)
		{
			leftUp = leftup;
			rightBottom = leftup + size;
		}
		Vector2 getCenterPos() const;
		Rectangle getRectangle() const
		{
			return Rectangle(leftUp, Vector2f(rightBottom.x, leftUp.y),
				Vector2f(leftUp.x, rightBottom.y), rightBottom);
		}
		
		Vector2 leftUp;
		Vector2 rightBottom;
		Vector2 leftBottom() const { return Vector2(leftUp.x, rightBottom.y); }
		Vector2 rightUp() const { return Vector2(rightBottom.x, leftUp.y); }
		
		Vector2 getSize() const { return Vector2(rightBottom.x - leftUp.x, rightBottom.y - leftUp.y); }
		int getWidth() const { return rightBottom.x - leftUp.x; }
		int getHeight() const { return rightBottom.y - leftUp.y; }
		
		//面積
		int getArea() const { return (getWidth() * getHeight()); }

		//引数の２点から自動で左上と右下を算出して矩形を作成する
		//(※左上と右下が定まっている場合は通常の引数付きコンストラクタを利用してください）
		static FixedRectangle createFixedRectangle(const Vector2& point1, const Vector2& point2);
	};

	class Capsule
	{
	public:
		Capsule() : pos1(Vector2f(0.0f)), pos2(Vector2f(0.0f)), r(0.0f) {}
		Capsule(Vector2f p1, Vector2f p2, float radius) : pos1(p1), pos2(p2), r(radius) {}

		Vector2f pos1, pos2;
		float r;
	};

#pragma endregion

#pragma region _衝突情報

	class HitRectInfo
	{
	public:
		HitRectInfo() : hitPos(Vector2f(0.0f)), normal(Vector2f(0.0f)) {}
		Vector2f hitPos;
		Vector2f normal;
	};

#pragma endregion

#pragma region _判定関数

	//点と円の当たり判定
	bool hitCheck(const Vector2f& point, const Circle& circle);

	//線と円の当たり判定
	bool hitCheck(const Line& line, const Circle& circle);

	//円と円の当たり判定
	bool hitCheck(const Circle& circle1, const Circle& circle2);

	//円と四角の当たり判定(判定のみ)
	bool hitCheck(const Circle& circle, const Rectangle& rect);

	//点とカプセルの当たり判定
	bool hitCheck(const Vector2& point, const Capsule& capsule);

	//円とカプセルの当たり判定
	bool hitCheck(const Circle& circle, const Capsule& capsule);

	//点と四角の当たり判定
	bool hitCheck(const Vector2& point, const FixedRectangle& rect);
	bool hitCheck(const Vector2& point, const Rectangle& rect);

	//四角と四角の当たり判定
	bool hitCheck(const FixedRectangle& rect1, const FixedRectangle& rect2);

	//点と三角の当たり判定
	bool hitCheck(const Vector2& point, const Vector2& tri_v1, const Vector2& tri_v2, const Vector2& tri_v3);

	//衝突情報あり

	//円と四角の当たり判定(Normalize使用の為、低速)
	bool hitCheck(const Circle& circle, const Rectangle& rect, HitRectInfo& outInfo);

	//線と線の当たり判定
	bool hitCheck(const Line& line1, const Line& line2, Vector2f& outCrossPoint);

	//線と四角の当たり判定
	bool hitCheck(const Line& line, const Rectangle& rect, Vector2f& outCrossPoint);
	bool hitCheck(const Line& line, const FixedRectangle& rect, Vector2f& outCrossPoint);

#pragma endregion

#pragma region _描画

	//円描画
	void renderCircle(const Vector2f& center, const float& radius, const Color& col);
	void renderCircle(const Circle& circle, const Color& col);

	//矩形描画（※頂点は'Z'の順番で指定）
	void renderRectangle(const Vector2f& leftup, const Vector2f& rightup,
						 const Vector2f& leftbottom, const Vector2f& rightbuttom,
						 const Color& col);
	void renderRectangle(const Rectangle& rect, const Color& col);
	void renderRectangle(const FixedRectangle& rect, const Color& col);

	//カプセル描画
	void renderCapsule(const Vector2f& p1, const Vector2f& p2, const float& radius, const Color& col);

	//ライン描画
	void renderLine(const Vector2f& p1, const Vector2f& p2, const Color& col);
	void renderLine(const Line& line, const Color& col);

	//ライン描画 (ライン左方向指定要求で高速)
	//※水平や垂直なライン描画時や、すでに左方向計算済みの場合に使用
	//※線の太さはLeftベクトル依存。矩形になりそうならrenderRectangle使用を推奨
	void renderLine(const Vector2f& p1, const Vector2f& p2, const Vector2f& left, const Color& col);

	//矢印描画（三角付きライン描画）
	void renderArrow(const Vector2f& start, const Vector2f& end, const Color& col, float triangleSize = (20.0f));
	void renderArrow(const Line& line, const Color& col, float triangleSize = (20.0f));

	//枠（フレーム）描画
	void renderFrame(const FixedRectangle& rect, float thickness, const Color& col);
	void renderFrame(const Rectangle& rect, float thickness, const Color& col);

	//三角形描画
	void renderTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Color& col);

#pragma endregion

}



#pragma once

#include	"VitalizeMath.h"

class Color;

namespace VitalizeShapes2D
{
#pragma region _�}�`

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

	//Rectangle�͋�`���S�p�����p�Ƃ����Ӗ��Ȃ̂�FreeSquare��
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
		
		//�ʐ�
		int getArea() const { return (getWidth() * getHeight()); }

		//�����̂Q�_���玩���ō���ƉE�����Z�o���ċ�`���쐬����
		//(������ƉE������܂��Ă���ꍇ�͒ʏ�̈����t���R���X�g���N�^�𗘗p���Ă��������j
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

#pragma region _�Փˏ��

	class HitRectInfo
	{
	public:
		HitRectInfo() : hitPos(Vector2f(0.0f)), normal(Vector2f(0.0f)) {}
		Vector2f hitPos;
		Vector2f normal;
	};

#pragma endregion

#pragma region _����֐�

	//�_�Ɖ~�̓����蔻��
	bool hitCheck(const Vector2f& point, const Circle& circle);

	//���Ɖ~�̓����蔻��
	bool hitCheck(const Line& line, const Circle& circle);

	//�~�Ɖ~�̓����蔻��
	bool hitCheck(const Circle& circle1, const Circle& circle2);

	//�~�Ǝl�p�̓����蔻��(����̂�)
	bool hitCheck(const Circle& circle, const Rectangle& rect);

	//�_�ƃJ�v�Z���̓����蔻��
	bool hitCheck(const Vector2& point, const Capsule& capsule);

	//�~�ƃJ�v�Z���̓����蔻��
	bool hitCheck(const Circle& circle, const Capsule& capsule);

	//�_�Ǝl�p�̓����蔻��
	bool hitCheck(const Vector2& point, const FixedRectangle& rect);
	bool hitCheck(const Vector2& point, const Rectangle& rect);

	//�l�p�Ǝl�p�̓����蔻��
	bool hitCheck(const FixedRectangle& rect1, const FixedRectangle& rect2);

	//�_�ƎO�p�̓����蔻��
	bool hitCheck(const Vector2& point, const Vector2& tri_v1, const Vector2& tri_v2, const Vector2& tri_v3);

	//�Փˏ�񂠂�

	//�~�Ǝl�p�̓����蔻��(Normalize�g�p�ׁ̈A�ᑬ)
	bool hitCheck(const Circle& circle, const Rectangle& rect, HitRectInfo& outInfo);

	//���Ɛ��̓����蔻��
	bool hitCheck(const Line& line1, const Line& line2, Vector2f& outCrossPoint);

	//���Ǝl�p�̓����蔻��
	bool hitCheck(const Line& line, const Rectangle& rect, Vector2f& outCrossPoint);
	bool hitCheck(const Line& line, const FixedRectangle& rect, Vector2f& outCrossPoint);

#pragma endregion

#pragma region _�`��

	//�~�`��
	void renderCircle(const Vector2f& center, const float& radius, const Color& col);
	void renderCircle(const Circle& circle, const Color& col);

	//��`�`��i�����_��'Z'�̏��ԂŎw��j
	void renderRectangle(const Vector2f& leftup, const Vector2f& rightup,
						 const Vector2f& leftbottom, const Vector2f& rightbuttom,
						 const Color& col);
	void renderRectangle(const Rectangle& rect, const Color& col);
	void renderRectangle(const FixedRectangle& rect, const Color& col);

	//�J�v�Z���`��
	void renderCapsule(const Vector2f& p1, const Vector2f& p2, const float& radius, const Color& col);

	//���C���`��
	void renderLine(const Vector2f& p1, const Vector2f& p2, const Color& col);
	void renderLine(const Line& line, const Color& col);

	//���C���`�� (���C���������w��v���ō���)
	//�������␂���ȃ��C���`�掞��A���łɍ������v�Z�ς݂̏ꍇ�Ɏg�p
	//�����̑�����Left�x�N�g���ˑ��B��`�ɂȂ肻���Ȃ�renderRectangle�g�p�𐄏�
	void renderLine(const Vector2f& p1, const Vector2f& p2, const Vector2f& left, const Color& col);

	//���`��i�O�p�t�����C���`��j
	void renderArrow(const Vector2f& start, const Vector2f& end, const Color& col, float triangleSize = (20.0f));
	void renderArrow(const Line& line, const Color& col, float triangleSize = (20.0f));

	//�g�i�t���[���j�`��
	void renderFrame(const FixedRectangle& rect, float thickness, const Color& col);
	void renderFrame(const Rectangle& rect, float thickness, const Color& col);

	//�O�p�`�`��
	void renderTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Color& col);

#pragma endregion

}



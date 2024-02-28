#pragma once

#include	"Vitalize2DObj.h"

class VitalizeStringRenderer;

class VitalizeSwitchChar
{
public:
	const char* ja;
	const char* en;

	VitalizeSwitchChar() : ja(nullptr), en(nullptr) {}
	VitalizeSwitchChar(const char* japanese, const char* english)
	{
		ja = japanese;
		en = english;
	}

	bool operator == (std::nullptr_t null) const;
	bool operator != (std::nullptr_t null) const;


};

using V_String = VitalizeSwitchChar;

class VitalizeStringRenderer final
{
private:
	static Vitalize2DObj*	m_data;
	static unsigned short*	m_table;

	static Vitalize2DObj* alphabetImage;
	static bool switchFlag;

	static Vitalize2DObj::RenderInfo cacheInfo;

public:
	static void	SystemInit();
	static void	SystemRelease();
	static void SystemSetType(bool type);
	static void SystemSwitchType();
	static bool getSwitchFlag() { return switchFlag; }

	//������`��
	static int	Render(const int& x, const int& y, const char* text);
	static int	Render(const int& x, const int& y, const char* text, const Vector2f& scale);
	static int	Render(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);
	static int	Render(const int& x, const int& y, const char* text, const Vector2f& scale, const int& rlength);
	static int	Render(const int& x, const int& y, const char* text, const Vector2f& scale, const int& rlength, const Color& col);
	static int	RenderBasic(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);
	static void	RenderInCenter(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);
	static void	RenderBasicInCenter(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);

	//�A���t�@�x�b�g�`��
	static int	RenderA(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);
	static int	RenderBasicA(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);
	static void	RenderAInCenter(const int& x, const int& y, const char* text, const Vector2f& scale = Vector2f(1.0f), const Color& col = Color::White);

	//�A���p
	static int	RenderCS(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);
	static int	RenderACS(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);

	//�����s�p
	static void RenderMultiLine(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);
	static void RenderMultiLineA(const int& x, const int& y, const char* text, const Vector2f& scale, const Color& col);

	//����؂�ւ��p�`��
	static int Render(const int& x, const int& y, const V_String& text, const Vector2f& scale = (1.0f), const Color& col = Color::White);
	static int RenderBasic(const int& x, const int& y, const V_String& text, const Vector2f& scale = (1.0f), const Color& col = Color::White);
	static int RenderCS(const int& x, const int& y, const V_String& text, const Vector2f& scale = (1.0f), const Color& col = Color::White);
	static void	RenderInCenter(const int& x, const int& y, const V_String& text, const Vector2f& scale = Vector2f(1.0f), const Color& col = Color::White);
	static void	RenderBasicInCenter(const int& x, const int& y, const V_String& text, const Vector2f& scale = Vector2f(1.0f), const Color& col = Color::White);
	static void RenderMultiLine(const int& x, const int& y, const V_String& text, const Vector2f& scale = Vector2f(1.0f), const Color& col = Color::White);

	//Rutec����
	static void RenderRotation(const Vector2& pos, const char* text, float rotateRad, const Vector2f& scale = (1.0f), const Color& col = Color::White);
	static void RenderRotationInCenter(const Vector2& pos, const char* text, float rotateRad, const Vector2f& scale = (1.0f), const Color& col = Color::White);
	static void RenderRotationMultiLine(const Vector2& pos, const char* text, float rotateRad, const Vector2f& scale = (1.0f), const Color& col = Color::White);
	static void RenderRotationMultiLineInCenter(const Vector2& pos, const char* text, float rotateRad, const Vector2f& scale = (1.0f), const Color& col = Color::White);

private:
	//new �֎~
	VitalizeStringRenderer() {}
	~VitalizeStringRenderer() {}

	//�����`��
	static bool	FontRenderCore(
		const int& x, const int& y, const unsigned short& font,
		const Vector2f& scale, const float& rotate, const Color& col);
	//�A���t�@�x�b�g�`��
	static bool AlphabetRenderCore(
		Vector2& pos, const unsigned char& font,
		const Vector2f& scale, const float& rotate, const Color& col);
	//�A���t�@�x�b�g�����`�敝���Z�o
	static int calcAlphabetRenderLength(const unsigned char& font);

};


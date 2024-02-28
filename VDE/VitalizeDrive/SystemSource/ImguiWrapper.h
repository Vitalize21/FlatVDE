#pragma once

#include "imgui.h"

namespace ImguiWrapper
{
#pragma region GlobalSystem
	//������
	void Init(void* hwnd);

	//���
	void Release();

	//�P��̍X�V�łP�񂾂��Ăяo��(�V�[���̐e���ŌĂԂ��Ƃ𐄏�)
	void RenderBegin();
	
	//�P��̍X�V�łP�񂾂��Ăяo��(�V�[���̐e���ŌĂԂ��Ƃ𐄏�)
	void RenderEnd();
	
	//�c�[���p�E�B���h�E�ݒ�t���O���擾
	int getToolBasicWindowFlags();

#pragma endregion
}

//���O�E�B���h�E
class ImguiLogConsole
{
public:
	ImGuiTextBuffer Buf;
	ImGuiTextFilter Filter;
	ImVector<int>	LineOffsets;
	ImVector<int>	LineColorType;
	bool			AutoScroll;

	ImguiLogConsole()
	{
		AutoScroll = true;
		Clear();
	}

	void Clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
		LineColorType.clear();
	}

	void AddLog(int colorType, const char* fmt, ...);

	void Draw(const char* title, bool* p_open = NULL);

	ImVec4 getLineColor(int line_no);

};


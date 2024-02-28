#pragma once

#include "imgui.h"

namespace ImguiWrapper
{
#pragma region GlobalSystem
	//初期化
	void Init(void* hwnd);

	//解放
	void Release();

	//１回の更新で１回だけ呼び出す(シーンの親元で呼ぶことを推奨)
	void RenderBegin();
	
	//１回の更新で１回だけ呼び出す(シーンの親元で呼ぶことを推奨)
	void RenderEnd();
	
	//ツール用ウィンドウ設定フラグを取得
	int getToolBasicWindowFlags();

#pragma endregion
}

//ログウィンドウ
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


#pragma region Include
#include "VitalizeSceneBase.h"
#include "sceneMain.h"
#include "VitalizeStringRenderer.h"
#include "ScreenManager.h"
#include "KeySet.h"
#pragma endregion

bool sceneMain::Initialize()
{
	SCREEN_MANAGER.setBackgroundColor(Color::Gray);

	return true;
}

void sceneMain::Begining()
{
}

sceneMain::~sceneMain()
{
}

void sceneMain::Update()
{
}//Update

void sceneMain::Render()
{
	SCREEN_MANAGER.RenderBackground();

	VitalizeStringRenderer::RenderAInCenter(960, 540, "Hello World");

}//Render



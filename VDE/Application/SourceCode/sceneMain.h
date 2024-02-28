#pragma once

class VitalizeSceneBase;
class Vitalize2DObj;
class GameMain;
class GameTitle;

class	sceneMain : public VitalizeSceneBase
{

public:
	sceneMain() {}
	virtual ~sceneMain();
	//	初期化
	bool Initialize() override;
	//	更新・描画
	void Update() override;
	void Render() override;
	//　開始・終了
	void Begining() override;
	//void Ending() override;

private:
	int posX;
	int posY;

};

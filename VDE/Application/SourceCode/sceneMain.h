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
	//	������
	bool Initialize() override;
	//	�X�V�E�`��
	void Update() override;
	void Render() override;
	//�@�J�n�E�I��
	void Begining() override;
	//void Ending() override;

private:
	int posX;
	int posY;

};

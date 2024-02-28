#pragma once

class VitalizeSceneBase;
class Vitalize2DObj;

class	sceneVitalizeHome : public VitalizeSceneBase
{
public:
	sceneVitalizeHome() {}
	virtual ~sceneVitalizeHome();
	//	初期化
	bool Initialize() override;
	//	更新・描画
	void Update() override;
	void Render() override;
	//　開始・終了
	void Begining() override;
	void Ending() override;

	void switchHome();
	void renderTitleBar();

	void startScene();
	void startScene(int id);
	void finishScene();
	bool isOpenHome() const { return (innerState != InnerState::HomeClosed); }
	bool isExecutionCreaterScene() const { return executionCreaterScene; }
	void closeHome();

	void updateSceneController(const Vector2& leftup);
	void renderSceneController(const Vector2& leftup, const Color& buttonCol = Color::Gray, bool title = true);

private:
	Vitalize2DObjResolution* interfaceObj;
	
	int bgNum;
	int windowMoveState;
	float	titleBarButtonAlpha;
	int		titleBarButtonSelect;
	float	titleBarPickAlpha;
	//char searchText[256];
	Vector2 searchBarPosCenter;

	struct IconData
	{
		Vector2 pos;
		float alpha;
		char name[256];
		int iconNum;
	};
	IconData startIcon[5];
	static const int iconMax = 10;
	IconData iconData[iconMax];

	int		iconMoveState;
	float	iconMoveRatio;
	float	iconMoveEaseValue;
	int		iconSelectNum;
	float	iconSelectRatio;
	float	iconSelectEaseValue;

	enum class InnerState : int { Free = 0, FadeOut, SceneChange, HomeClosed, FinishScene, /*OpenAnim,*/ };
	InnerState innerState;

	int executionNum;
	float bgAlpha;

	enum class SearchBarSelect : int { None = 0, Bar, Translation, Youtube, Easing, };
	SearchBarSelect searchBarSelect;

	bool executionCreaterScene;

	enum class ButtonSelect : int {
		None = 0, BeginingReload, ForceReload, Close
	};
	ButtonSelect buttonSelectType;

	enum class ExtendWindowState : int { Close = 0, OpenAnim, Open, CloseAnim, };
	ExtendWindowState extendWindowState;
	float	exwMoveRatio;
	float	exwMoveEaseValue;
	int exwSelectNum;
	float	exwSelectRatio;
	float	exwSelectEaseValue;

	void setupIcon();

	void updateWindowMove();

	void updateTitleBar();
	void updateAppIcon();
	void renderAppIcon();

	void renderColor(const int& x, const int& y, const int& width, const int& height, const Color& col);

	void changeScene();
	bool tryOpenCreaterScene();

};
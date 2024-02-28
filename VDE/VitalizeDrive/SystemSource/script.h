#ifndef _SCRIPT_H_
#define	_SCRIPT_H_

#include	"TextLoader.h"
#include	"../SystemSource/Vitalize2DObj.h"

class Script :public TextLoader_S
{
private:
public:
	virtual ~Script(){}
	void Initialize();
	bool FileOpen(char* name);
	bool Practice(char* com, int* eventNo);
	bool PracticeGetInt(char* com, const char* name, int* eventNo);
};

//文字画像一行の文字数
#define	LINE_MAX	16

class Text
{
private:
	Vitalize2DObj*	m_data;

	int			m_size;
	int			m_num;
	WORD*		m_table;

public:
	Text() : m_data(nullptr), m_table(nullptr){}
	~Text();

	void	Initialize(const char* name);

	//文字描画
	void	FontRender(int x, int y, WORD font);
	void	FontRender(int x, int y, WORD font, Vector2f scale, float alpha = (1.0f));//拡張
	//文字列描画
	void	TextRender(int x, int y, char* text);
	void	TextRender(int x, int y, char* text, float scale);//文字列スケールあり版
	void	TextRender(int x, int y, const char* text, float scale);//const文字列スケールあり版
	void	TextRender(int x, int y, char* text, float scale, int rlength);//表示可能文字数指定版
	void	TextRenderInCenter(int x, int y, const char* text, float scale, float alpha);
	void	TextRenderInCenter(int x, int y, const char* text, Vector2f scale, float alpha);

#if DEVELOP_DEBUG
	int	DebugTextRender(int x, int y, const char* text, float scale);
#endif
};

class ScriptDrowManager : private Script
{
public:
	enum class Script_Color{ black = 0, white };

private:
	Text*	m_text;				//文字
	char	m_mesTex[256];

	int		m_eventNo;		//会話イベント番号
	int		m_stepNo;		//会話番号
	
	int		eventNum;		//実行するスクリプト番号
	bool	active;			//実行中なら

	int		renderLength;	//描画可能文字数
	int		lenTimer;		//描画文字数増加用

	int		uniqueNumber;	//スクリプトに登録される個別番号
	int		caseMax;		//文字ケースの最大

	void Init(char* fname, Script_Color colorType);
	bool Event();

public:
	//コンストラクタ(スクリプトファイルが読み込まれてません)
	ScriptDrowManager() : m_eventNo(0), m_stepNo(0), eventNum(0), active(false){
		m_text = new Text();
		m_text->Initialize("../Application/Resource/tools/Script/Strings_b");
		ZeroMemory(m_mesTex, sizeof(char) * 256);
		active = false;
	}
	ScriptDrowManager(Script_Color colorType) : m_eventNo(0), m_stepNo(0), eventNum(0), active(false){
		m_text = new Text();
		switch (colorType){
		case Script_Color::white:	m_text->Initialize("../Application/Resource/tools/Script/Strings_w");	break;
		default:					m_text->Initialize("../Application/Resource/tools/Script/Strings_b");	break;
		}
		ZeroMemory(m_mesTex, sizeof(char) * 256);
		active = false;
	}
	ScriptDrowManager(char* fname, Script_Color colorType = Script_Color::black){ Init(fname, colorType); }
	~ScriptDrowManager(){
		delete m_text;
		m_text = nullptr;
	}

	bool ReLoadFile(char* fname);

	void Update();
	void Render();

	void Render( int rx, int ry, float scale = (1.0f) );

	void Set(int EventNum);			//次セット
	bool SetNextCase();			//次セット
	void DirectSet(int EventNum);

	bool next();					//行、進めるよ (戻り値true文章全て終了)

	bool isProcess();	//文章表示中か？(false == next待ち)
	int	getUniqueNumber(){ return uniqueNumber; }

	//文字列そのまま描画
	void StringRender(int x, int y, char* name, float scale = (1.0f)){ m_text->TextRender(x, y, name, scale); }
	void StringRender(int x, int y, const char* name, float scale = (1.0f)){ m_text->TextRender(x, y, name, scale); }
	void StringRenderInCenter(int x, int y, const char* name, float scale, float alpha){ 
		m_text->TextRenderInCenter(x, y, name, scale, alpha); }
	void StringRenderInCenter(int x, int y, const char* name, Vector2f scale, float alpha) {
		m_text->TextRenderInCenter(x, y, name, scale, alpha);
	}

#if DEVELOP_DEBUG
	int	DebugTextRender(int x, int y, const char* text, float scale){ return m_text->DebugTextRender(x, y, text, scale); }
#endif

};

#endif // !_SCRIPT_H_


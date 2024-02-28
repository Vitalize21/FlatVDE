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

//�����摜��s�̕�����
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

	//�����`��
	void	FontRender(int x, int y, WORD font);
	void	FontRender(int x, int y, WORD font, Vector2f scale, float alpha = (1.0f));//�g��
	//������`��
	void	TextRender(int x, int y, char* text);
	void	TextRender(int x, int y, char* text, float scale);//������X�P�[�������
	void	TextRender(int x, int y, const char* text, float scale);//const������X�P�[�������
	void	TextRender(int x, int y, char* text, float scale, int rlength);//�\���\�������w���
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
	Text*	m_text;				//����
	char	m_mesTex[256];

	int		m_eventNo;		//��b�C�x���g�ԍ�
	int		m_stepNo;		//��b�ԍ�
	
	int		eventNum;		//���s����X�N���v�g�ԍ�
	bool	active;			//���s���Ȃ�

	int		renderLength;	//�`��\������
	int		lenTimer;		//�`�敶���������p

	int		uniqueNumber;	//�X�N���v�g�ɓo�^�����ʔԍ�
	int		caseMax;		//�����P�[�X�̍ő�

	void Init(char* fname, Script_Color colorType);
	bool Event();

public:
	//�R���X�g���N�^(�X�N���v�g�t�@�C�����ǂݍ��܂�Ă܂���)
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

	void Set(int EventNum);			//���Z�b�g
	bool SetNextCase();			//���Z�b�g
	void DirectSet(int EventNum);

	bool next();					//�s�A�i�߂�� (�߂�ltrue���͑S�ďI��)

	bool isProcess();	//���͕\�������H(false == next�҂�)
	int	getUniqueNumber(){ return uniqueNumber; }

	//�����񂻂̂܂ܕ`��
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


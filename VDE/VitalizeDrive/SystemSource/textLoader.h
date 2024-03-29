#ifndef	_TEXT_LOADER
#define	_TEXT_LOADER

#include	<windows.h>

//*****************************************************************************
//
//		テキストファイル操作クラス
//
//*****************************************************************************
class	textLoader {
private:
		bool	bInit;

		char	*FileBuf;		//	ファイルバッファ
		DWORD	TextSize;		//	ファイルサイズ
		DWORD	ReadIndex;		//	現在位置ポインタ
		
		DWORD	PushIndex;
		int		SearchTop();	//	頭出し

protected:
		bool	Open( LPSTR filename );

public:
		bool	EndCheck();
		void	LoadString( LPSTR str );	//	文字列取得
		void	LoadName( LPSTR str );		//	"名前"文字列取得
		float	LoadFloat();				//	小数点数
		int		LoadInt();					//	整数
		DWORD	LoadDWORD();				//	符号なし整数

		void	LoadStringTest( LPSTR str );//	文字列取得（ポインタ移動なし）

		void	SetIndex( int index );
		void	Skip( void );	//	{}スキップ
		void	In( void );		//	{侵入
		void	Out( void );	//	}外へ

		void	newLine();		//	次の行へ

		DWORD	GetPointer(){ return ReadIndex; }
		void	SetPointer( DWORD p ){ ReadIndex = p; }

		void	push();		//	現在位置保存
		void	pop();		//	位置復元

		//	検索
		bool	Search( LPSTR str );
		bool	Search2( LPSTR str );
		bool	SearchIndent( LPSTR str );
		bool	SearchLine( LPSTR str );

		//	初期化・解放
		textLoader( LPSTR filename );
		textLoader(){}

		~textLoader()
		{
			if( FileBuf ) delete[] FileBuf;		
		}

};

class	TextLoader_S{
private:
	bool	bInit;

	char	*FileBuf;		//ファイルバッファ
	DWORD	m_TextSize;		//ファイルサイズ
	DWORD	m_ReadIndex;	//現在位置ポインタ

	DWORD	m_PushIndex;
	int		SearchTop();	//頭出し

protected:
	bool	Open(LPSTR filename);
	
	void ReSetReadIndex(){ m_ReadIndex = 0; }
public:
	bool	EndCheck();
	void	LoadStringEx(LPSTR str);		//文字列取得
	void	LoadName(LPSTR str);			//"名前"文字列取得
	float	LoadFloat(void);				//小数点数
	int		LoadInt(void);					//整数

	void	LoadStringTest(LPSTR str);	//文字列取得（ポインタ移動なし）

	void	SetIndex(int index);
	void	Skip(void);					//{}スキップ
	void	In(void);						//{侵入
	void	Out(void);						//}外へ

	void	newLine();						//次の行へ

	DWORD	GetPointer(){ return m_ReadIndex; }
	void	SetPointer(DWORD p){ m_ReadIndex = p; }

	void	push();						//現在位置保存
	void	pop();							//位置復元

	//検索
	bool	Search(LPSTR str);
	bool	Search2(LPSTR str);
	bool	SearchIndent(LPSTR str);
	bool	SearchLine(LPSTR str);

	//初期化・解放
	TextLoader_S(LPSTR filename);
	TextLoader_S():FileBuf(nullptr){}

	virtual ~TextLoader_S()
	{
		if (FileBuf != nullptr){
			delete[] FileBuf;
			FileBuf = nullptr;
		}
	}

};

#endif


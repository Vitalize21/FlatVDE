#ifndef	_TEXT_LOADER
#define	_TEXT_LOADER

#include	<windows.h>

//*****************************************************************************
//
//		�e�L�X�g�t�@�C������N���X
//
//*****************************************************************************
class	textLoader {
private:
		bool	bInit;

		char	*FileBuf;		//	�t�@�C���o�b�t�@
		DWORD	TextSize;		//	�t�@�C���T�C�Y
		DWORD	ReadIndex;		//	���݈ʒu�|�C���^
		
		DWORD	PushIndex;
		int		SearchTop();	//	���o��

protected:
		bool	Open( LPSTR filename );

public:
		bool	EndCheck();
		void	LoadString( LPSTR str );	//	������擾
		void	LoadName( LPSTR str );		//	"���O"������擾
		float	LoadFloat();				//	�����_��
		int		LoadInt();					//	����
		DWORD	LoadDWORD();				//	�����Ȃ�����

		void	LoadStringTest( LPSTR str );//	������擾�i�|�C���^�ړ��Ȃ��j

		void	SetIndex( int index );
		void	Skip( void );	//	{}�X�L�b�v
		void	In( void );		//	{�N��
		void	Out( void );	//	}�O��

		void	newLine();		//	���̍s��

		DWORD	GetPointer(){ return ReadIndex; }
		void	SetPointer( DWORD p ){ ReadIndex = p; }

		void	push();		//	���݈ʒu�ۑ�
		void	pop();		//	�ʒu����

		//	����
		bool	Search( LPSTR str );
		bool	Search2( LPSTR str );
		bool	SearchIndent( LPSTR str );
		bool	SearchLine( LPSTR str );

		//	�������E���
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

	char	*FileBuf;		//�t�@�C���o�b�t�@
	DWORD	m_TextSize;		//�t�@�C���T�C�Y
	DWORD	m_ReadIndex;	//���݈ʒu�|�C���^

	DWORD	m_PushIndex;
	int		SearchTop();	//���o��

protected:
	bool	Open(LPSTR filename);
	
	void ReSetReadIndex(){ m_ReadIndex = 0; }
public:
	bool	EndCheck();
	void	LoadStringEx(LPSTR str);		//������擾
	void	LoadName(LPSTR str);			//"���O"������擾
	float	LoadFloat(void);				//�����_��
	int		LoadInt(void);					//����

	void	LoadStringTest(LPSTR str);	//������擾�i�|�C���^�ړ��Ȃ��j

	void	SetIndex(int index);
	void	Skip(void);					//{}�X�L�b�v
	void	In(void);						//{�N��
	void	Out(void);						//}�O��

	void	newLine();						//���̍s��

	DWORD	GetPointer(){ return m_ReadIndex; }
	void	SetPointer(DWORD p){ m_ReadIndex = p; }

	void	push();						//���݈ʒu�ۑ�
	void	pop();							//�ʒu����

	//����
	bool	Search(LPSTR str);
	bool	Search2(LPSTR str);
	bool	SearchIndent(LPSTR str);
	bool	SearchLine(LPSTR str);

	//�������E���
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


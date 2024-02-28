#pragma once

//開発時専用(各種デバッグ処理がアクティブになります)
#define	DEVELOP_DEBUG (1)
#define	DEVELOP_DEBUG_PC (1)

//Vitalize Extend Frame を使用します( 1:はい 0:いいえ )
//※2019年現在Extend Frame計画は凍結しており使用できません。
#define ENABLE_EXTEND_FRAME (0)

//ポインタの解放
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#define SAFE_DELETE(p)  if( (p) != nullptr ){ delete (p);  (p) = nullptr; }

//コンソールへのログ出力
#define LOG_MESSAGE(message, ...)

//ログテキストの出力を行うか (0:いいえ 1:はい)
//要調査：ログ出力有効化でヒープ破壊発生
#define OUTPUT_LOG_TEXT (0)



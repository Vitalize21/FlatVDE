#pragma once

#define KEY_ENABLE (1)

#if KEY_ENABLE

#define KEY_UP		( GetKeyState( 'W' ) < 0 )
#define KEY_DOWN	( GetKeyState( 'S' ) < 0 )
#define KEY_RIGHT	( GetKeyState( 'D' ) < 0 )
#define KEY_LEFT	( GetKeyState( 'A' ) < 0 )
#define KEY_A		( GetKeyState( 'K' ) < 0 )
#define KEY_B		( GetKeyState( 'L' ) < 0 )
#define KEY_X		( GetKeyState( 'O' ) < 0 )
#define KEY_Y		( GetKeyState( 'P' ) < 0 )

#define N_KEY_UP		(GetKeyState(VK_NUMPAD8) < 0)
#define N_KEY_DOWN		(GetKeyState(VK_NUMPAD2) < 0)
#define N_KEY_RIGHT		(GetKeyState(VK_NUMPAD6) < 0)
#define N_KEY_LEFT		(GetKeyState(VK_NUMPAD4) < 0)

#define KEY_ENTER		(GetKeyState( VK_RETURN ) < 0)

#define KEY_SPACE		(GetKeyState( VK_SPACE ) < 0)
#define KEY_SHIFT		(GetKeyState( VK_LSHIFT ) < 0)
#define KEY_ALT			(GetKeyState( VK_MENU ) < 0)

class KeyDownControl
{
private:
	static const __int8 keyMax = 16;

public:
	static __int8 keyState[keyMax];

	static void Init() { for (__int8 i = 0; i < keyMax; i++)keyState[i] = 0; }
	static void Update();

};
#define KEY_UP_PUSH			( KeyDownControl::keyState[0] == 1 || KeyDownControl::keyState[10] == 1 )
#define KEY_LEFT_PUSH		( KeyDownControl::keyState[1] == 1 || KeyDownControl::keyState[11] == 1 )
#define KEY_DOWN_PUSH		( KeyDownControl::keyState[2] == 1 || KeyDownControl::keyState[12] == 1 )
#define KEY_RIGHT_PUSH		( KeyDownControl::keyState[3] == 1 || KeyDownControl::keyState[13] == 1 )
#define KEY_SPACE_PUSH		( KeyDownControl::keyState[4] == 1 )
#define KEY_SHIFT_PUSH		( KeyDownControl::keyState[5] == 1 )
#define KEY_TAB_PUSH		( KeyDownControl::keyState[14] == 1 )
#define KEY_DELETE_PUSH		( KeyDownControl::keyState[15] == 1 )

#define KEY_UP_HOLD			( KeyDownControl::keyState[0] == 2 || KeyDownControl::keyState[10] == 2 )
#define KEY_LEFT_HOLD		( KeyDownControl::keyState[1] == 2 || KeyDownControl::keyState[11] == 2 )
#define KEY_DOWN_HOLD		( KeyDownControl::keyState[2] == 2 || KeyDownControl::keyState[12] == 2 )
#define KEY_RIGHT_HOLD		( KeyDownControl::keyState[3] == 2 || KeyDownControl::keyState[13] == 2 )
#define KEY_SPACE_HOLD		( KeyDownControl::keyState[4] == 2 )
#define KEY_SHIFT_HOLD		( KeyDownControl::keyState[5] == 2 )
#define KEY_TAB_HOLD		( KeyDownControl::keyState[14] == 2 )

#define KEY_UP_RELEASE		( KeyDownControl::keyState[0] == 3 || KeyDownControl::keyState[10] == 3 )
#define KEY_LEFT_RELEASE	( KeyDownControl::keyState[1] == 3 || KeyDownControl::keyState[11] == 3 )
#define KEY_DOWN_RELEASE	( KeyDownControl::keyState[2] == 3 || KeyDownControl::keyState[12] == 3 )
#define KEY_RIGHT_RELEASE	( KeyDownControl::keyState[3] == 3 || KeyDownControl::keyState[13] == 3 )
#define KEY_SPACE_RELEASE	( KeyDownControl::keyState[4] == 3 )
#define KEY_SHIFT_RELEASE	( KeyDownControl::keyState[5] == 3 )
#define KEY_TAB_RELEASE		( KeyDownControl::keyState[14] == 3 )


////xbox360コントローラー
////XINPUT_STATE型の構造体を定義して引数として渡してください	( XINPUT_STATE pad; )
////dwPacketNumberは0初期化推奨	( pad.dwPacketNumber = 0; )
////更新処理時にパッドのチェック	( XBOX_KEY_CHECK( pad ) )
//
//#define XBOX_KEY_CHECK(p_state)		XInputGetState( 0, &p_state );
//
//#define XBOX_KEY_UP(p_state)		( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP )
//#define XBOX_KEY_DOWN(p_state)		( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN )
//#define XBOX_KEY_RIGHT(p_state)		( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT )
//#define XBOX_KEY_LEFT(p_state)		( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT )
//#define XBOX_KEY_A(p_state)			( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_A )
//#define XBOX_KEY_B(p_state)			( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_B )
//#define XBOX_KEY_X(p_state)			( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_X )
//#define XBOX_KEY_Y(p_state)			( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_Y )
//#define XBOX_KEY_START(p_state)		( p_state.Gamepad.wButtons == XINPUT_GAMEPAD_START )

#endif


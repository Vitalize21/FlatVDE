#include "framework.h"
#include "VitalizeDevelopmentEnv.h"
#include <shellapi.h> 
#include <windowsx.h>

#include	"VitalizeSystem.h"
#include	"VitalizeScene.h"
#include	"Vitalize2DObj.h"
#include	"VitalizeFramework.h"
#include    "VitalizeAudio.h"

#include    "ImguiWrapper.h"
#include    "imgui_impl_win32.h"

HINSTANCE hInst;
HWND mainWindowHandle = nullptr;

#if DEVELOP_DEBUG
static FILE* DebugConsolePointer = nullptr;	//コンソールウィンドウのポインタ
#endif

ATOM                RegistClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL InitVitalizeDrive();
VOID ReleaseVitalizeDrive();
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    RegistClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VITALIZEDEVELOPMENTENV));
    MSG msg;

    if (!InitVitalizeDrive())
    {
        GetMessage(&msg, nullptr, 0, 0);
        goto EndPhase;
    }

    ImguiWrapper::Init(mainWindowHandle);

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0)) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            VitalizeSystem::mainSystem::mpDeviceContext->ClearDepthStencilView(
                VitalizeSystem::mainSystem::mpDepthStencilView,
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

            MAIN_FRAMEWORK_INSTANCE.MainLoopUpdate();
            if (MAIN_FRAMEWORK_INSTANCE.MainLoopRender())
            {
                VitalizeSystem::mainSystem::mpDXGISwpChain->Present(0, 0);
            }
            MAIN_FRAMEWORK_INSTANCE.MainLoopWait();

        }
    }

EndPhase:

    ImguiWrapper::Release();

    ReleaseVitalizeDrive();

    DestroyWindow(mainWindowHandle);

    return (int)msg.wParam;
}

ATOM RegistClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VITALIZEDEVELOPMENTENV));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "VitalizeDrive";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_VITALIZEDEVELOPMENTENV));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

	const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	VitalizeSystem::mainSystem::setScreenType(
		AppSystemManager::initScreenDeviceResolution(
			GetSystemMetrics(SM_CXSCREEN), ScreenHeight));

	Vector2 windowPos = AppSystemManager::getWindowPos();
    switch (AppSystemManager::getWindowMode())
    {
    case AppSystemManager::WindowMode::Limit:
        {
            mainWindowHandle = CreateWindowEx(
                (ScreenHeight >= (1080 + 100)) ? 0 : WS_EX_TOPMOST, 
                "VitalizeDrive", "Vitalize Development Env",
                WS_POPUP | WS_VISIBLE, windowPos.x, windowPos.y,
                VitalizeSystem::mainSystem::ScreenWidth,
                VitalizeSystem::mainSystem::ScreenHeight,
                NULL, NULL, hInstance, NULL);
        }
        break;
    default:
        {
            if (GetKeyState(VK_CONTROL) < 0)
            {
                mainWindowHandle = CreateWindowEx(
                    0, "VitalizeDrive", "Vitalize Development Env",
                    WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX | WS_VISIBLE,
                    windowPos.x, windowPos.y,
                    VitalizeSystem::mainSystem::ScreenWidth + 16,
                    VitalizeSystem::mainSystem::ScreenHeight + 39,
                    NULL, NULL, hInstance, NULL);
                AppSystemManager::setWindowMode(AppSystemManager::WindowMode::OldWindow);
            }
            else
            {
                mainWindowHandle = CreateWindowEx(
                    0, "VitalizeDrive", "Vitalize Development Env",
                    WS_POPUP | WS_VISIBLE, windowPos.x, windowPos.y,
                    VitalizeSystem::mainSystem::ScreenWidth,
                    VitalizeSystem::mainSystem::ScreenHeight,
                    NULL, NULL, hInstance, NULL);
            }
        }
        break;
    }
    

    if (!mainWindowHandle)
    {
        return FALSE;
    }

    DragAcceptFiles(mainWindowHandle, TRUE); //メインウィンドウにD&Dを許可する
    UpdateWindow(mainWindowHandle);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
        return true;
    }

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: HDC を使用する描画コードをここに追加してください...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
#pragma region _KEY_EVENT
    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:

            if (GetKeyState(VK_SHIFT) < 0)
            {
                PostMessage(hWnd, WM_CLOSE, 0, 0);
                return 0;
            }

            MAIN_FRAMEWORK_INSTANCE.switchHomeScene();
            return 0;

#if DEVELOP_DEBUG
        case VK_F1:		
            //デバッグ出力用コンソールウィンドウ作成
            if(DebugConsolePointer == nullptr)
            {
                AllocConsole();
                freopen_s(&DebugConsolePointer, "CON", "w", stdout);
                char TitleBuffer[512];
                HWND ConsoleWindow;
                RECT WindowRect;

                // コンソールウインドウのタイトルを取得	
                GetConsoleTitle(TitleBuffer, sizeof(TitleBuffer));
                // タイトルからウインドウを検索してウインドウハンドルを取得
                ConsoleWindow = FindWindow(NULL, TitleBuffer);
                // 現在のウインドウ矩形の位置を取得
                GetWindowRect(ConsoleWindow, &WindowRect);
                // ウインドウの位置サイズ変更
                const Vector2 ConsoleSize(600, 300);
                const Vector2 windowPos = AppSystemManager::getWindowPos();
                MoveWindow(ConsoleWindow, 
                    windowPos.x + VitalizeSystem::mainSystem::ScreenWidth,
                    windowPos.y,
                    ConsoleSize.x, ConsoleSize.y, TRUE);
                //閉じるボタンを消す
                HMENU consoleMenu = GetSystemMenu(ConsoleWindow, false);
                RemoveMenu(consoleMenu, SC_CLOSE, MF_BYCOMMAND);

            }
            return 0;
#endif

        }//switch wParam

        break;

#pragma endregion
        //ウィンドウ系------------------------
#pragma region _WINDOW
    case WM_MOVE:
        AppSystemManager::setWindowPos(Vector2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
        break;
    case WM_SIZE:
        VitalizeSystem::mainSystem::ScreenWidth = LOWORD(lParam);
        VitalizeSystem::mainSystem::ScreenHeight = HIWORD(lParam);

		//画面サイズが変わったらランタイムのサイズも変更になる
		RuntimeSystem::setScreenSize(
			VitalizeSystem::mainSystem::ScreenWidth,
			VitalizeSystem::mainSystem::ScreenHeight);
		break;
#pragma endregion
        //マウスイベント系	------------------
#pragma region _MOUSE
    case WM_LBUTTONDOWN:
		AppSystemManager::setMouseClick(AppSystemManager::MouseInput::Left, true);
        sceneVitalizeSystem::leftClick = true;
        SetCapture(hWnd);//マウスの画面外キャプチャを許可
        break;
    case WM_LBUTTONUP:
		AppSystemManager::setMouseClick(AppSystemManager::MouseInput::Left, false);
        sceneVitalizeSystem::leftClick = false;
		if (AppSystemManager::isMouseKeyNotPressed())
		{
			ReleaseCapture();//マウスの画面外キャプチャを解放
		}
        break;
    case WM_RBUTTONDOWN:
		AppSystemManager::setMouseClick(AppSystemManager::MouseInput::Right, true);
        sceneVitalizeSystem::rightClick = true;
        SetCapture(hWnd);//マウスの画面外キャプチャを許可
        break;
    case WM_RBUTTONUP:
		AppSystemManager::setMouseClick(AppSystemManager::MouseInput::Right, false);
        sceneVitalizeSystem::rightClick = false;
		if (AppSystemManager::isMouseKeyNotPressed())
		{
			ReleaseCapture();//マウスの画面外キャプチャを解放
		}
		break;
    case WM_MBUTTONDOWN:
		AppSystemManager::setMouseClick(AppSystemManager::MouseInput::Wheel, true);
        sceneVitalizeSystem::wheelClick = true;
        SetCapture(hWnd);//マウスの画面外キャプチャを許可
        break;
    case WM_MBUTTONUP:
		AppSystemManager::setMouseClick(AppSystemManager::MouseInput::Wheel, false);
        sceneVitalizeSystem::wheelClick = false;
		if (AppSystemManager::isMouseKeyNotPressed())
		{
			ReleaseCapture();//マウスの画面外キャプチャを解放
		}
		break;
    case WM_MOUSEWHEEL:
		AppSystemManager::setWheelValue(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
		sceneVitalizeSystem::wheelValue = AppSystemManager::getWheelValue();
        //LOG_MESSAGE("ホイール回転量[%d]\n", sceneVitalizeSystem::wheelValue);
        break;
    case WM_MOUSEMOVE:
		AppSystemManager::setMousePointerPos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		//↓互換用
		sceneVitalizeSystem::pointerPos = AppSystemManager::getMousePointerPos();
		//LOG_MESSAGE("pointerPos x[%d] y[%d]\n", sceneVitalizeSystem::pointerPos.x, sceneVitalizeSystem::pointerPos.y);
        break;
#pragma endregion
        //D&D系	------------------
#pragma region _DRAGDROP
    case WM_DROPFILES:
    {
        HDROP hdrop = (HDROP)wParam;
        // ドロップされたファイルの個数を得る
        int num = DragQueryFile(hdrop, -1, NULL, 0);

        // ファイルパスを格納
        for (int i = 0; i < num; i++) {
            DragQueryFile(hdrop, i, sceneVitalizeSystem::DDFilePath, sizeof(char) * MAX_PATH);
            sceneVitalizeSystem::isDD = true;
            break;
        }
        DragFinish(hdrop);
    }
    break;
#pragma endregion
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

#pragma region _VitalizeDrive

BOOL InitVitalizeDrive()
{
    //Vitalizeシステム構築
    if (!VitalizeSystem::mainSystem::Initialize(mainWindowHandle))
    {
        return FALSE;
    }
    
    //2D使用準備
    if (!Vitalize2DObjDrive::Initialize())
    {
        return FALSE;
    }

    //音声利用準備
    VitalizeAudioSystem::Initialize();

    //メインフレームワーク生成
    MAIN_FRAMEWORK_INSTANCE.Initialize();

    const float ScreenClearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    // ハルシェーダーを無効にする。
    VitalizeSystem::mainSystem::mpDeviceContext->HSSetShader(NULL, NULL, 0);
    // ドメインシェーダーを無効にする。
    VitalizeSystem::mainSystem::mpDeviceContext->DSSetShader(NULL, NULL, 0);
    // ジオメトリシェーダーを無効にする。
    VitalizeSystem::mainSystem::mpDeviceContext->GSSetShader(NULL, NULL, 0);
    // コンピュートシェーダーを無効にする。
    VitalizeSystem::mainSystem::mpDeviceContext->CSSetShader(NULL, NULL, 0);
    // ビュークリア
    VitalizeSystem::mainSystem::mpDeviceContext->ClearRenderTargetView(
        VitalizeSystem::mainSystem::mpRenderTargetView, ScreenClearColor);

    //フレームワーク更新開始準備
    MAIN_FRAMEWORK_INSTANCE.MainLoopSetup();

    return TRUE;
}

VOID ReleaseVitalizeDrive()
{
    //システム全解放
    VitalizeSystem::mainSystem::Release();
    Vitalize2DObjDrive::Release();
    VitalizeAudioSystem::Release();

    //フレームワーク解放
    MAIN_FRAMEWORK_INSTANCE.Release();

#if DEVELOP_DEBUG
    if (DebugConsolePointer != nullptr)
    {
        fclose(DebugConsolePointer);
        FreeConsole();
        DebugConsolePointer = nullptr;
    }
#endif

}

#pragma endregion





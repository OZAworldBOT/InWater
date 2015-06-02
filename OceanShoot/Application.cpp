/*
*	アプリケーションの初期化を行うクラス
*
*	2015/05/17		Y.Ozawa
*/

#include "Lib.h"
#include <crtdbg.h>
#include <cassert>

LPDIRECT3DDEVICE9		d3dDevice = 0;
LPDIRECTINPUTDEVICE8	dinputDevice = 0;

DWORD FloatToDWORD(float val){ return *((DWORD *)&val); }

//デフォルトコンストラクタ
Application::Application()
{
	DebugLog("アプリケーションを生成しました\n");

	//メモリリークを検出する
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//アプリケーションの初期化
	Initialize(win_title, Rect(0, 0, 640, 480), false, nullptr, 0);
}

//コンストラクタ
Application::Application(char* win_title, RECT win_rect, bool win_fullscreen, HINSTANCE hInst, int cmdShow)
{
	DebugLog("アプリケーションを生成しました\n");

	//メモリリークを検出する
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//アプリケーションの初期化
	Initialize(win_title, win_rect, win_fullscreen, hInst, cmdShow);
}

//デストラクタ
Application::~Application()
{
	DebugLog("アプリケーションを解放しました\n");

	//アプリケーションの終了処理
	Release();
}

void Application::Initialize(char* win_title, RECT win_rect, bool win_fullscreen, HINSTANCE hInst, int cmdShow)
{
	//	引数をメンバー変数に保存する
	Application::win_title = win_title;
	Application::win_rect = win_rect;
	Application::win_fullscreen = win_fullscreen;

	//ウィンドウの初期化
	if (!InitWindow(hInst, cmdShow))
		DebugAlert("ウィンドウを\nしょきかできませんでした");

	//Direct3Dの初期化
	if (!InitDirect3d())
		DebugAlert("Direct3Dを\n初期化できませんでした。");

	//プレゼンテーションパラメータの初期化
	if (!InitPresentParam())
		DebugAlert("プレゼンテーションパラメータを\n初期化できませんでした。");

	//Direct3Dデバイスの初期化
	if (!InitD3dDevice())
		DebugAlert("Direct3Dデバイスを\n初期化できませんでした。");

	//DirectInputの初期化
	if (!InitDirectInput(hInst))
		DebugAlert("DirectInputを\n初期化できませんでした。");

	//DirectInputデバイスの初期化
	if (!InitDinputDevice())
		DebugAlert("DirectInputデバイスを\n初期化できませんでした。");

	DebugLog("アプリケーションを初期化しました。\n");
}

//アプリケーションの解放
void Application::Release()
{
	//デバッグビルド時にビープ音を鳴らす
#if defined (DEBUG) | defined(_DEBUG)
	MessageBeep(MB_OK);
#endif

	//コンソールを閉じる
	CloseConsole();

	//モデルのテンプレートを破棄する
	SafeRelease(d3dDevice);
	SafeRelease(direct3d);
	SafeRelease(directInput);
}

//ループ処理
bool Application::Loop()
{
	MSG msg;

	//前のフレームの描画を終了する
	d3dDevice->EndScene();

	//前のフレームの描画を反映する
	d3dDevice->Present(0, 0, 0, 0);

	//Windowsメッセージを処理する
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		//ウィンドウを閉じる
		if (WM_QUIT == msg.message)
			return false;
	}

	//画面のクリア
	if (FAILED(d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 102, 204), 1.0f, 0)))
		return false;

	//今のフレームの描画を開始する
	d3dDevice->BeginScene();

	return true;
}

//ウィンドウプロシージャ
LRESULT CALLBACK Application::winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

//ウィンドウの初期化
bool Application::InitWindow(HINSTANCE hInst, int cmdShow)
{
	//ウィンドウクラスの初期化
	WNDCLASSEX winClass;
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = 0;
	winClass.lpfnWndProc = winProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = hInst;
	winClass.hIcon = nullptr;
	winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winClass.hbrBackground = nullptr;
	winClass.lpszMenuName = nullptr;
	winClass.lpszClassName = "GameWindow";
	winClass.hIconSm = nullptr;

	//ウィンドウクラスの登録
	if (!RegisterClassEx(&winClass))
		return false;

	//ウィンドウを作成
	if (win_fullscreen)
	{
		hWnd = CreateWindow(winClass.lpszClassName, win_title, WS_OVERLAPPEDWINDOW,
			0, 0, win_rect.right, win_rect.bottom,
			nullptr, nullptr, hInst, nullptr);
	}
	else
	{
		AdjustWindowRect(&win_rect, WS_OVERLAPPEDWINDOW, false);
		hWnd = CreateWindow(winClass.lpszClassName, win_title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, win_rect.right - win_rect.left, win_rect.bottom - win_rect.top,
			nullptr, nullptr, hInst, nullptr);
	}

	if (!hWnd)
		return false;

	if (FAILED(ShowWindow(hWnd, cmdShow)))
		return false;

	ValidateRect(hWnd, 0);

	return true;
}

//Direct3Dの初期化
bool Application::InitDirect3d()
{
	//Direct3D9オブジェクトの作成
	if ((direct3d = Direct3DCreate9(D3D_SDK_VERSION)) == 0)
		return false;

	return true;
}

//プレゼンテーションパラメータの初期化
bool Application::InitPresentParam()
{
	D3DDISPLAYMODE displayMode;

	//現在のディスプレイモードを取得
	if (FAILED(direct3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return false;

	//デバイスのプレゼンテーションパラメータを初期化
	ZeroMemory(&presentParam, sizeof(D3DPRESENT_PARAMETERS));
	presentParam.BackBufferCount = 1;
	//フルスクリーンの場合
	if (win_fullscreen)
	{
		presentParam.Windowed = false;						//フルスクリーン表示の指定
		presentParam.BackBufferWidth = win_rect.right;		//フルスクリーン時の横幅
		presentParam.BackBufferHeight = win_rect.bottom;	//フルスクリーン時の縦幅
	}
	else
	{
		presentParam.Windowed = true;						// ウインドウ内表示の指定
	}
	presentParam.BackBufferFormat = displayMode.Format;	//カラーモードの指定
	presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParam.EnableAutoDepthStencil = true;			//エラー対策
	presentParam.AutoDepthStencilFormat = D3DFMT_D16;		//エラー対策

	return true;
}


//Direct3Dデバイスの初期化
bool Application::InitD3dDevice()
{
	//ディスプレイアダプタを表すためのデバイスを作成
	//描画と頂点処理をハードウェアで行なう
	if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
		//上記の設定が失敗したら
		//描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
			//上記の設定が失敗したら
			//描画と頂点処理をCPUで行なう
			if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
				return false;

	//レンダリングステータスの設定
	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//両面描画モードの設定
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);				//Z比較を行う
	d3dDevice->SetRenderState(D3DRS_DITHERENABLE, true);		//ディザリングを行う

	// ポイントスプライトの設定
	d3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// ポイントスケールの係数
	d3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	d3dDevice->SetRenderState(D3DRS_POINTSIZE, FloatToDWORD(1.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));

	return true;
}

//DirectInputの初期化
bool Application::InitDirectInput(HINSTANCE hInstance)
{
	//DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr)))
		return false;

	return true;
}

//DirectInputデバイスの初期化
bool Application::InitDinputDevice()
{
	//DirectInputデバイスを作成
	if (FAILED(directInput->CreateDevice(GUID_SysMouse, &dinputDevice, nullptr)))
		return false;

	//データフォーマットを設定
	if (FAILED(dinputDevice->SetDataFormat(&c_dfDIMouse2)))
		return false;

	//協調モードを設定
	if (FAILED(dinputDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return false;

	//	軸モードを設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(dinputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		MessageBox(NULL, "設定に失敗", "Direct Input Error", MB_OK);
		return false;
	}

	// 入力制御開始
	dinputDevice->Acquire();

	return true;
}




























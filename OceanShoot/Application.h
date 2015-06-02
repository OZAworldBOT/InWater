/*
*	アプリケーションの初期化を行うクラス
*
*	2015/05/17		Y.Ozawa
*/

#pragma once

//	アプリケーション実行環境を制御するクラス
class Application
{
private:
	HWND	hWnd;
	char*	win_title;
	RECT	win_rect;
	bool	win_fullscreen;

	LPDIRECT3D9				direct3d;
	D3DPRESENT_PARAMETERS	presentParam;
	LPDIRECTINPUT8			directInput;

public:
	//デフォルトコンストラクタ
	Application();

	//コンストラクタ
	Application(char* win_title, RECT win_rect, bool win_fullscreen, HINSTANCE hInst, int cmdShow);


	//デストラクタ
	~Application();

	//初期化
	void Initialize(char* win_title, RECT win_rect, bool win_fullscreen, HINSTANCE hInst, int cmdShow);

	//解放
	void Release();

	//ループ処理
	bool Loop();

private:
	//ウィンドウプロシージャ
	static LRESULT CALLBACK winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//ウィンドウの初期化
	bool InitWindow(HINSTANCE hInst, int cmdShow);

	//Direct3Dの初期化
	bool InitDirect3d();

	//プレゼンテーションの初期化
	bool InitPresentParam();

	//Direct3Dデバイスの初期化
	bool InitD3dDevice();

	//DirectInputの初期化
	bool InitDirectInput(HINSTANCE hInst);

	//DirectInputデバイスの初期化
	bool InitDinputDevice();
};


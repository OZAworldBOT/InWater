/*
*	アプリケーションのエントリーポイントとメインループ
*
*	2015/05/18		Y.Ozawa
*/

#include "Lib.h"
#include <memory>
#include "Main.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "Title.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int cmdShow)
{
	//	コンソールを開く
#if defined (DEBUG) | defined(_DEBUG)
	OpenConsole();
#endif


	//	アプリケーション実行環境を構築
	unique_ptr<Application> app(new Application("オーシャン・シューッ！！", Rect(0, 0, 800, 600), false, hInst, cmdShow));
	unique_ptr<Light> light(new Light());
	unique_ptr<Player> player(new Player());
	unique_ptr<Enemy> enemy(new Enemy());
	unique_ptr<Stage> stage(new Stage());
	unique_ptr<Title> title(new Title());
	gameState = GAME_STATE_TITLE;

	RECT recDisplay;
	HWND hDeskWnd;
	hDeskWnd = GetDesktopWindow();

	GetWindowRect(hDeskWnd, &recDisplay);



	while (app->Loop())
	{
		switch (gameState)
		{
		case GAME_STATE_TITLE:
			
			if (!GetAsyncKeyState(VK_RETURN))
			{
				title->View();
				title->Move();
				if (GetAsyncKeyState(VK_ESCAPE))
				{
					title->Release();
				}
			}
			else
			{ 
				title->Release();
				gameState = GAME_STATE_MAIN;
			}
			break;
			
		case GAME_STATE_MAIN:
			//-----------------------
			//	プレイヤー
			//-----------------------
			player->Move();
			player->View();
			player->Draw();

			//-----------------------
			//	敵
			//-----------------------
			enemy->Draw();
			enemy->Move();
			enemy->Hit();

			//-----------------------
			//	ステージ
			//-----------------------
			stage->View();

			player->Shot();



			break;
		case GAME_STATE_CLEAR:
			break;
		case GAME_STATE_FAILED:
			break;
		}

		//	エスケープキーを押したらゲーム終了
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			break;
		}

		ShowCursor(FALSE);
		SetCursorPos(recDisplay.right / 2, recDisplay.bottom / 2);
	}

	return 0;
}



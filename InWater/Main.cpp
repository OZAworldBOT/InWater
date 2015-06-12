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
#include "underEnemy.h"
#include "Boss.h"
#include "Boss.h"
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
	unique_ptr<Application> app(new Application("水の中にいる", Rect(0, 0, 800, 600), false, hInst, cmdShow));
	unique_ptr<Light> light(new Light());
	unique_ptr<Title> title(new Title());
	unique_ptr<Stage> stage(new Stage());
	unique_ptr<Player> player(new Player());
	unique_ptr<Enemy> enemy(new Enemy());
	unique_ptr<underEnemy> Uenemy(new underEnemy());
	unique_ptr<Boss> boss(new Boss());
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
			if (GetAsyncKeyState(VK_RETURN))
			{
				gameState = GAME_STATE_MAIN;
			}
			else
			{ 
				title->View();
				title->Move();
			}
			break;
			
		case GAME_STATE_MAIN:

			//-----------------------
			//	プレイヤー
			//-----------------------
			player->Move();
			player->View();
			player->Draw();
			player->Destroy();

			//-----------------------
			//	敵
			//-----------------------
			enemy->Draw();
			Uenemy->Draw();
			boss->Draw();
			
			enemy->Move();
			Uenemy->Move();
			boss->Move();

			enemy->Hit();
			Uenemy->Hit();
			boss->Hit();
			//-----------------------
			//	ステージ
			//-----------------------
			stage->View();

			player->Shot();
			enemy->Shot();
			Uenemy->Shot();

			extern bool playerDeath;
			if (playerDeath == true)
			{
				gameState = GAME_STATE_FAILED;
			}
			
			break;
		case GAME_STATE_CLEAR:
			break;
		case GAME_STATE_FAILED:
			if (GetAsyncKeyState(VK_RETURN))
			{
				player->ResetPlayer();
				enemy->InitEnemy();
				Uenemy->InitUEnemy();
				boss->InitBoss();
				stage->initStage();

				gameState = GAME_STATE_MAIN;
			}
			break;
		}

		//	エスケープキーを押したらゲーム終了
		if (GetAsyncKeyState(VK_ESCAPE) && gameState == GAME_STATE_FAILED)
		{
			break;
		}

		ShowCursor(FALSE);
		SetCursorPos(recDisplay.right / 2, recDisplay.bottom / 2);
	}

	return 0;
}
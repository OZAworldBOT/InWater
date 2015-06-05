/*
*	画面遷移用のヘッダーファイル
*
*	2015/06/03		Y.Ozawa
*/
#pragma once

enum _GameState
{
	GAME_STATE_TITLE,
	GAME_STATE_MAIN,
	GAME_STATE_CLEAR,
	GAME_STATE_FAILED,
};
_GameState gameState;

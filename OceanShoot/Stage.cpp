/*
*	ステージのクラス　ステージ内にはポイントスプライトで泡を表示
*
*	2015/05/27		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Stage.h"

Stage::Stage()
{

	bubble = new b_point[BUBBLE_MAX];
	for (int i = 0; i < BUBBLE_MAX; i++)
	{
		bubble[i].deathCount = 0;
		bubble[i].pos = D3DXVECTOR3(0, 0, 0);
		bubble[i].exist = TRUE;
	}
	water = new Texture("Texture/effect0.png");
	stage = new Graphic();

	initStage();
}

Stage::~Stage()
{
	delete water;
	delete stage;

	delete[] bubble;
}


void Stage::Move()
{
}

void Stage::initStage()
{
	MinRange = D3DXVECTOR3(750, 0, 750);
	MaxRange = D3DXVECTOR3(1250, 10, 1250);

	D3DXVECTOR3 range = MaxRange - MinRange;

	for (int i = 0; i < BUBBLE_MAX; i++)
	{
		bubble[i].pos.x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
		bubble[i].pos.z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
	}
}

void Stage::View()
{
	//	プレイヤーの座標を取得
	extern D3DXVECTOR3 inputState;

	MinRange = D3DXVECTOR3(-250, 0, -250);
	MaxRange = D3DXVECTOR3(250, 10, 250);

	Vertex3 *vertex = new Vertex3[BUBBLE_MAX];
	D3DXVECTOR3 range = MaxRange - MinRange;
	srand((unsigned int)time(NULL));

	for (int i = 0; i < BUBBLE_MAX; i++)
	{

		if (bubble[i].exist == TRUE)
		{
			bubble[i].pos.y += rand() % 100 * 0.008f;
			D3DXVec3Normalize(&bubble[i].accel, &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
			bubble[i].pos.x += (bubble[i].accel.x * 0.1f);
			bubble[i].pos.z += (bubble[i].accel.z * 0.1f);
		}
		vertex[i].pos = bubble[i].pos;
		vertex[i].size = 1.0f;
		vertex[i].color = 0xffffffff;

		if (bubble[i].pos.y > 50)
		{
			bubble[i].pos.x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x + inputState.x;
			bubble[i].pos.z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z + inputState.z;
			bubble[i].pos.y = 0;
		}

	}

	stage->DrawPointSprite(vertex, BUBBLE_MAX, *water);
	delete[] vertex;


}
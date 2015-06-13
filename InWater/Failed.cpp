/*
*	タイトル画面のクラス
*
*	2015/06/03		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Failed.h"


Failed::Failed()
{

	point = new f_point[POINT_MAX];
	for (int i = 0; i < POINT_MAX; i++)
	{
		point[i].deathCount = 0;
		point[i].pos = D3DXVECTOR3(0, 0, 0);
		point[i].exist = TRUE;
	}
	texture = new Texture("Texture/effect2.png");
	graphic = new Graphic();
	camera = new Camera();

	Init();
}

Failed::~Failed()
{
	Release();
}

void Failed::Init()
{
	MinRange = D3DXVECTOR3(-70, 0, -10);
	MaxRange = D3DXVECTOR3(70, 10, 300);
	Pos = D3DXVECTOR3(0, 0.8, 2);
	Rot = D3DXVECTOR3(-0.1, D3DXToRadian(180), 0);
	Scale = D3DXVECTOR3(40, 40, 40);
	camera_Pos = D3DXVECTOR3(0, 0, 0);
	camera_Rot = D3DXVECTOR3(D3DXToRadian(-15), 0, 0);

	D3DXVECTOR3 range = MaxRange - MinRange;

	for (int i = 0; i < POINT_MAX; i++)
	{
		point[i].pos.x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
		point[i].pos.z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
		point[i].pos.y = rand() % 100;
	}
}

void Failed::Release()
{
	delete graphic;
	delete texture;
	delete[] point;
	delete camera;

	DebugLog("タイトル画面を破棄しました。\n");
}

void Failed::View()
{
	MinRange = D3DXVECTOR3(-70, 0, -10);
	MaxRange = D3DXVECTOR3(70, 10, 300);

	Vertex3 *vertex = new Vertex3[POINT_MAX];
	D3DXVECTOR3 range = MaxRange - MinRange;
	srand((unsigned int)time(NULL));

	//-----------------------------------------------------------------
	//	泡
	//-----------------------------------------------------------------
	for (int i = 0; i < POINT_MAX; i++)
	{
		if (point[i].exist == TRUE)
		{
			point[i].pos.y += (rand() % 100 + 1) * 0.008f;
			D3DXVec3Normalize(&point[i].accel, &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
			point[i].pos.x += (point[i].accel.x * 0.2f);
			point[i].pos.z += (point[i].accel.z * 0.2f);
		}

		if (point[i].pos.y > 100)
		{
			point[i].pos.x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
			point[i].pos.z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
			point[i].pos.y = 0;
		}
		vertex[i].pos = point[i].pos;
		vertex[i].size = 1.0f;
		vertex[i].color = 0xffffffff;
	}
	graphic->DrawString({ 0, 0, 800, 500 }, "ENTER->Retry");
	graphic->DrawString({ 0, 0, 800, 700 }, "ESCAPE->End");
	graphic->DrawPointSprite_A(vertex, POINT_MAX, *texture, true);
	
	camera->View(camera_Pos, camera_Rot);
	
	delete[] vertex;
}

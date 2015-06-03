/*
*	タイトル画面のクラス
*
*	2015/06/03		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Title.h"


Title::Title()
{

	point = new t_point[POINT_MAX];
	for (int i = 0; i < POINT_MAX; i++)
	{
		point[i].deathCount = 0;
		point[i].pos = D3DXVECTOR3(0, 0, 0);
		point[i].exist = TRUE;
	}
	model = new Model("Model/3dFont.x");
	texture = new Texture[2];
	texture[0].LoadTexture("Texture/effect2.png");
	texture[1].LoadTexture("Texture/gold_block.png");
	graphic = new Graphic();
	camera = new Camera();

	Init();
}

Title::~Title()
{
}



void Title::Init()
{
	MinRange = D3DXVECTOR3(-70, 0, -10);
	MaxRange = D3DXVECTOR3(70, 10, 300);
	Pos = D3DXVECTOR3(0, 0.8, 2);
	Rot = D3DXVECTOR3(-0.1, D3DXToRadian(180), 0);
	Scale = D3DXVECTOR3(30, 30, 30);
	camera_Pos = D3DXVECTOR3(0, 0, 0);
	camera_Rot = D3DXVECTOR3(D3DXToRadian(-15), 0, 0);
	Speed1 = 1;
	Speed2 = 1;

	D3DXVECTOR3 range = MaxRange - MinRange;

	for (int i = 0; i < POINT_MAX; i++)
	{
		point[i].pos.x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
		point[i].pos.z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
	}
}

void Title::Release()
{
	delete graphic;
	delete[] texture;
	delete model;
	delete[] point;
	delete camera;
}

void Title::View()
{
	MinRange = D3DXVECTOR3(-70, 0, -10);
	MaxRange = D3DXVECTOR3(70, 10, 300);

	Vertex3 *vertex = new Vertex3[POINT_MAX];
	D3DXVECTOR3 range = MaxRange - MinRange;
	srand((unsigned int)time(NULL));

	for (int i = 0; i < POINT_MAX; i++)
	{

		if (point[i].exist == TRUE)
		{
			point[i].pos.y += rand() % 100 * 0.008f;
			D3DXVec3Normalize(&point[i].accel, &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
			point[i].pos.x += (point[i].accel.x * 0.1f);
			point[i].pos.z += (point[i].accel.z * 0.1f);
		}
		vertex[i].pos = point[i].pos;
		vertex[i].size = 1.0f;
		vertex[i].color = 0xffffffff;

		if (point[i].pos.y > 100)
		{
			point[i].pos.x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
			point[i].pos.z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
			point[i].pos.y = 0;
		}

	}
	graphic->DrawString({ 0, 0, 800, 900 }, "PUSH ENTER");
	graphic->DrawPointSprite_A(vertex, POINT_MAX, texture[0], false);

	delete[] vertex;
}

void Title::Move()
{
	Scale.x += 0.02 * Speed1;
	Scale.y -= 0.05 * Speed2;
	if (Scale.x > 32)
	{
		Speed1 = -1;
	}

	if (Scale.x < 30)
	{
		Speed1 = 1;
	}

	if (Scale.y > 35)
	{
		Speed2 = 1;
	}
	if (Scale.y < 30)
	{
		Speed2 = -1;
	}

	graphic->DrawModelTexture(Pos, Rot, Scale, *model, texture[1]);
	camera->View(camera_Pos, camera_Rot);
}

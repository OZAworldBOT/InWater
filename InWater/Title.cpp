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
	model = new Model[2];
	model[0].LoadMesh("Model/3dFont.x");
	model[1].LoadMesh("Model/tako.x");
	texture = new Texture[3];
	texture[0].LoadTexture("Texture/effect2.png");
	texture[1].LoadTexture("Texture/font_skin.png");
	texture[2].LoadTexture("Texture/red_skin.png");
	graphic = new Graphic();
	camera = new Camera();

	Init();
}

Title::~Title()
{
	Release();
}



void Title::Init()
{
	MinRange = D3DXVECTOR3(-70, 0, -10);
	MaxRange = D3DXVECTOR3(70, 10, 300);
	Pos = D3DXVECTOR3(0, 0.8, 2);
	Rot = D3DXVECTOR3(-0.1, D3DXToRadian(180), 0);
	Scale = D3DXVECTOR3(40, 40, 40);
	camera_Pos = D3DXVECTOR3(0, 0, 0);
	camera_Rot = D3DXVECTOR3(D3DXToRadian(-15), 0, 0);
	Speed1 = 1;
	Speed2 = 1;
	Rand = 0;

	D3DXVECTOR3 range = MaxRange - MinRange;

	for (int i = 0; i < POINT_MAX; i++)
	{
		point[i].pos.x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
		point[i].pos.z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
		point[i].pos.y = rand() % 100;
	}
}

void Title::Release()
{
	delete graphic;
	delete[] texture;
	delete[] model;
	delete[] point;
	delete camera;

	DebugLog("タイトル画面を破棄しました。\n");
}

void Title::View()
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
	graphic->DrawString({ 0, 0, 800, 900 }, "PUSH ENTER");
	graphic->DrawPointSprite_A(vertex, POINT_MAX, texture[0], true);
	delete[] vertex;

	//-----------------------------------------------------------------
	//	たこ
	//-----------------------------------------------------------------
	if (tako_Flag == false)
	{
		Rand = rand() % 100;
		if (Rand == 2 || Rand == 51 || Rand == 89)
		{
			tako_Scale = D3DXVECTOR3(2, 2, 2);
			tako_Pos.x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
			tako_Pos.z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
			tako_Pos.y = 0;
			tako_Flag = true;
		}
	}
	if (tako_Flag == true)
	{
		D3DXVec3Normalize(&tako_Accel, &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
		tako_Pos.x += (tako_Accel.x * 0.1f);
		tako_Pos.z += (tako_Accel.z * 0.1f);
		tako_Pos.y += rand() % 100 * 0.008f;
		graphic->DrawModelTexture(tako_Pos, tako_Rot, tako_Scale, model[1], texture[2], true);
	}
	if (tako_Pos.y > 200)
	{
		tako_Flag = false;
	}

}

void Title::Move()
{
	Scale.x += 0.02 * Speed1;
	Scale.y += 0.05 * Speed2;
	if (Scale.x > 42)
	{
		Speed1 = -1;
	}

	if (Scale.x < 40)
	{
		Speed1 = 1;
	}

	if (Scale.y > 45)
	{
		Speed2 = -1;
	}
	if (Scale.y < 40)
	{
		Speed2 = 1;
	}

	graphic->DrawModelTexture(Pos, Rot, Scale, model[0], texture[1], true);
	camera->View(camera_Pos, camera_Rot);
}

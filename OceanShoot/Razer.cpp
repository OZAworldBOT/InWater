/*
*	レーザーのクラス
*
*	2015/05/27		Y.Ozawa
*/

#include "Lib.h"

Razer::Razer()
{
	razerPoint = new razer_point[RAZER_MAX];
	texture = new Texture("Texture/ball.bmp");
	razer = new Graphic();
}

Razer::~Razer()
{
	delete texture;
	delete razer;
	delete[] razerPoint;
}

void Razer::Draw(D3DXVECTOR3 Pos[])
{
	Vertex3 *vertex = new Vertex3[RAZER_MAX];

	for (int i = 0; i < RAZER_MAX; i++)
	{
		razerPoint[i].pos = Pos[i];

		vertex[i].pos = razerPoint[i].pos;
		vertex[i].size = 0.7f;
		vertex[i].color = 0xffffffff;

	}

	razer->DrawPointSprite(vertex, RAZER_MAX, *texture);
	delete[] vertex;
}
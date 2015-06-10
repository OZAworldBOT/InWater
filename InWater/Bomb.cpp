/*
*	”š’e‚ÌƒNƒ‰ƒX
*
*	2015/05/30		Y.Ozawa
*/

#include "Lib.h"

Bomb::Bomb()
{
	bombPoint = new bomb_point[BOMB_MAX];
	texture = new Texture("Texture/ball.bmp");
	bomb = new Graphic();
}

Bomb::~Bomb()
{
	delete texture;
	delete bomb;
	delete[] bombPoint;
}

void Bomb::Draw(D3DXVECTOR3 Pos[], float Size, bool Alpha)
{
	Vertex3 *vertex = new Vertex3[BOMB_MAX];

	for (int i = 0; i < BOMB_MAX; i++)
	{
		bombPoint[i].pos = Pos[i];

		vertex[i].pos = bombPoint[i].pos;
		vertex[i].size = Size;
		vertex[i].color = 0xffffffff;

	}
	bomb->DrawPointSprite_A(vertex, BOMB_MAX, *texture, Alpha);

	delete[] vertex;
}
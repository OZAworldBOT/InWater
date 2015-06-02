/*
*	’eŠÛ‚ÌƒNƒ‰ƒX
*
*	2015/05/27		Y.Ozawa
*/

#include "Lib.h"

Bullet::Bullet()
{
	bulletPoint = new bullet_point[BULLET_MAX];
	texture = new Texture("Texture/ball.bmp");
	bullet = new Graphic();

}

Bullet::~Bullet()
{
	delete texture;
	delete bullet;
	delete[] bulletPoint;
}

void Bullet::Draw(D3DXVECTOR3 Pos[])
{
	Vertex3 *vertex = new Vertex3[BULLET_MAX];

	for (int i = 0; i < BULLET_MAX; i++)
	{
		bulletPoint[i].pos = Pos[i];

		vertex[i].pos = bulletPoint[i].pos;
		vertex[i].size = 0.7f;
		vertex[i].color = 0xffffffff;

	}

	bullet->DrawPointSprite(vertex, BULLET_MAX, *texture);
	delete[] vertex;
}
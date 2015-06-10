/*
*	’eŠÛ‚ÌƒNƒ‰ƒX
*
*	2015/05/27		Y.Ozawa
*/

#include "Lib.h"

Bullet::Bullet(const TCHAR *textureFileName, int Num)
{
	bulletPoint = new bullet_point[Num];
	texture = new Texture(textureFileName);
	bullet = new Graphic();
}

Bullet::~Bullet()
{
	delete texture;
	delete bullet;
	delete[] bulletPoint;
}

void Bullet::Draw(D3DXVECTOR3 Pos[], float Size, int Num)
{
	Vertex3 *vertex = new Vertex3[Num];

	for (int i = 0; i < Num; i++)
	{
		bulletPoint[i].pos = Pos[i];

		vertex[i].pos = bulletPoint[i].pos;
		vertex[i].size = Size;
		vertex[i].color = 0xffffffff;

	}
	bullet->DrawPointSprite(vertex, Num, *texture);

	delete[] vertex;
}
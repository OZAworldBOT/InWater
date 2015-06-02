/*
*	”š”­‚ÌƒNƒ‰ƒX
*
*	2015/06/02		Y.Ozawa
*/

#include "Lib.h"

Explosion::Explosion()
{
	expPoint = new exp_point[EXPLOSION_MAX];
	texture = new Texture("Texture/effect1.png");
	explosion = new Graphic();
}

Explosion::~Explosion()
{
	delete texture;
	delete explosion;
	delete[] expPoint;
}

void Explosion::Draw(D3DXVECTOR3 Pos[])
{
	Vertex3 *vertex = new Vertex3[EXPLOSION_MAX];

	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		expPoint[i].pos = Pos[i];
		vertex[i].pos = expPoint[i].pos;
		vertex[i].size = 1.5f;
		vertex[i].color = 0xffffffff;
	}

	explosion->DrawPointSprite(vertex, EXPLOSION_MAX, *texture);
	delete[] vertex;
}


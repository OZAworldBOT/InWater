/*
*	�e�ۂ̃N���X
*
*	2015/05/27		Y.Ozawa
*/

#pragma once

class Texture;
class Graphic;

//���q�̃f�[�^�[
struct bullet_point
{
	D3DXVECTOR3 pos;			//	�e�ۂ̍��W
};

class Bullet
{	
private:

	Texture *texture;
	Graphic *bullet;
	bullet_point *bulletPoint;

public:

	//	�f�t�H���g�R���X�g���N�^
	Bullet();

	//	�f�X�g���N�^
	~Bullet();

	//	�`�悷��
	void Draw(D3DXVECTOR3 Pos[]);
};
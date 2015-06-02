/*
*	�����̃N���X
*
*	2015/06/02		Y.Ozawa
*/

#pragma once

class Texture;
class Graphic;

//���q�̃f�[�^�[
struct exp_point
{
	D3DXVECTOR3 pos;			//	�����̍��W
};


class Explosion
{
private:
	Texture *texture;
	Graphic *explosion;
	exp_point *expPoint;

public:
	//	�R���X�g���N�^
	Explosion();

	//	�f�X�g���N�^
	~Explosion();

	//	�`�悷��
	void Draw(D3DXVECTOR3 Pos[]);
};








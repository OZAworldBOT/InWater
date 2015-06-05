/*
*	���[�U�[�̃N���X
*
*	2015/05/27		Y.Ozawa
*/

#pragma once

class Texture;
class Graphic;

//���q�̃f�[�^�[
struct razer_point
{
	D3DXVECTOR3 pos;			//	���[�U�[�̍��W
};

class Razer
{
private:

	Texture *texture;
	Graphic *razer;
	razer_point *razerPoint;

public:

	//	�f�t�H���g�R���X�g���N�^
	Razer();

	//	�f�X�g���N�^
	~Razer();

	//	�`�悷��
	void Draw(D3DXVECTOR3 Pos[]);
};

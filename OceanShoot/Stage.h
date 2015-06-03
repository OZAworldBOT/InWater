/*
*	�X�e�[�W�̃N���X�@�X�e�[�W�Ȃ��ɂ̓|�C���g�X�v���C�g�ŖA��\��
*
*	2015/05/27		Y.Ozawa
*/
#pragma once


class Graphic;
class Texture;

//���q�̃f�[�^�[
struct b_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};


class Stage
{
	D3DXVECTOR3 MinRange;
	D3DXVECTOR3 MaxRange;

private:
	Texture *water;
		    
	Graphic *stage;
		    
	b_point *bubble;
public:

	//�f�t�H���g�R���X�g���N�^
	Stage();

	//�f�X�g���N�^
	~Stage();

	void Move();

	//�B�e����
	void View();

	//	������
	void initStage();

};
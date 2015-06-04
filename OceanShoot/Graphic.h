/*
*	�I�u�W�F�N�g
*
*	2015/05/19		Y.Ozawa
*/

#pragma once

class Model;
class Texture;

// 3D���_
struct Vertex3
{
	D3DXVECTOR3 pos;	// �ʒu
	float size;			// �T�C�Y
	DWORD color;		// �F
};

// �`��n�����N���X
class Graphic
{
private:

	// �|�C���g�X�v���C�g�̒��_�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 pointBuffer;

	// �t�H���g
	LPD3DXFONT font;

	//	���[���h�s��
	D3DXMATRIX m_world;
public:

	// �f�t�H���g�R���X�g���N�^
	Graphic();

	// �f�X�g���N�^
	~Graphic();

	// 3D���f����`�悷��
	void DrawModel(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale, Model &model);
	void DrawModelTexture(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale, Model &model, Texture &texture, bool alpha);

	// �|�C���g�X�v���C�g��`�悷��
	void DrawPointSprite(Vertex3 point[], int numPoint, Texture &texture);
	void DrawPointSprite_A(Vertex3 point[], int numPoint, Texture &texture, bool alpha);

	// ������̕`��
	void DrawString(RECT rect, const TCHAR *str);

private:

	// ���W�ϊ�
	void TransForm(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale);
};
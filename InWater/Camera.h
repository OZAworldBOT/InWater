/*
*	�J�����N���X
*
*	2015/05/19		Y.Ozawa
*/

#pragma once

// 3D���B�e���邽�߂̃N���X
class Camera
{
private:

	D3DVIEWPORT9 viewport;		// �r���[�|�[�g
	float aspect;				// �A�X�y�N�g��

	D3DXMATRIX m_view;			// �r���[�s��
	D3DXMATRIX m_projection;	// �ˉe�s��

public:

	//�f�t�H���g�R���X�g���N�^
	Camera();

	//�f�X�g���N�^
	~Camera();

	//�J�����̏�����
	void Initialize();

	//�J�����̉��
	void Release();

	//�J��������B�e����
	void View(D3DXVECTOR3 pos, D3DXVECTOR3 rota);

private:

	//���W�ϊ�
	void TransForm(D3DXVECTOR3 pos, D3DXVECTOR3 rota);

};

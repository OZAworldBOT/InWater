/*
*	�J�����N���X
*
*	2015/05/19		Y.Ozawa
*/

#include "Lib.h"

extern LPDIRECT3DDEVICE9 d3dDevice;

// �f�t�H���g�R���X�g���N�^
Camera::Camera()
{
	DebugLog("�J�����𐶐����܂����B\n");

	Initialize();
}

// �f�X�g���N�^
Camera::~Camera()
{
	DebugLog("�J������j�����܂����B\n");

	Release();
}

// �J�����̏�����
void Camera::Initialize()
{
	d3dDevice->GetViewport(&viewport);
	d3dDevice->SetViewport(&viewport);

	// �A�X�y�N�g��̌v�Z
	aspect = (float)viewport.Width / (float)viewport.Height;

	DebugLog("�J���������������܂����B\n");
}

// �J�����̔j��
void Camera::Release()
{

}

// �J��������B�e����
void Camera::View(D3DXVECTOR3 pos, D3DXVECTOR3 rota)
{
	// ���W�v�Z
	TransForm(pos, rota);

	// �J�����̃r���[�s����v�Z
	D3DXMATRIX *m_temp = new D3DXMATRIX;
	D3DXMatrixIdentity(m_temp);
	D3DXMatrixInverse(m_temp, nullptr, &m_view);	// �J�����̋t�s������߂�

	// �r���[�s���ݒ�
	d3dDevice->SetTransform(D3DTS_VIEW, m_temp);

	// �ˉe�s��̏�����
	D3DXMatrixIdentity(&m_projection);

	// �ˉe�s����쐬
	D3DXMatrixPerspectiveFovLH(&m_projection, D3DXToRadian(45.0f), aspect, 1.0f, 100000.0f);

	// �ˉe�s����p�C�v���C���ɐݒ�
	d3dDevice->SetTransform(D3DTS_PROJECTION, &m_projection);

	delete m_temp;
}

// ���W�ϊ�
void Camera::TransForm(D3DXVECTOR3 pos, D3DXVECTOR3 rota)
{
	D3DXMATRIX *m_temp = new D3DXMATRIX;
	D3DXMatrixIdentity(&m_view);
	D3DXMatrixIdentity(m_temp);

	// �s�����]����
	D3DXMatrixRotationYawPitchRoll(m_temp, rota.y, rota.x, rota.z);
	m_view *= *m_temp;

	// �s��𕽍s�ړ�������
	D3DXMatrixTranslation(m_temp, pos.x, pos.y, pos.z);
	m_view *= *m_temp;

	delete m_temp;
}

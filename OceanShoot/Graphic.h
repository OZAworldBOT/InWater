/*
*	オブジェクト
*
*	2015/05/19		Y.Ozawa
*/

#pragma once

class Model;
class Texture;

// 3D頂点
struct Vertex3
{
	D3DXVECTOR3 pos;	// 位置
	float size;			// サイズ
	DWORD color;		// 色
};

// 描画系処理クラス
class Graphic
{
private:

	// ポイントスプライトの頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 pointBuffer;

	// フォント
	LPD3DXFONT font;

	//	ワールド行列
	D3DXMATRIX m_world;
public:

	// デフォルトコンストラクタ
	Graphic();

	// デストラクタ
	~Graphic();

	// 3Dモデルを描画する
	void DrawModel(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale, Model &model);
	void DrawModelTexture(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale, Model &model, Texture &texture);

	// ポイントスプライトを描画する
	void DrawPointSprite(Vertex3 point[], int numPoint, Texture &texture);
	void DrawPointSprite_A(Vertex3 point[], int numPoint, Texture &texture, bool alpha);

	// 文字列の描画
	void DrawString(RECT rect, const TCHAR *str);

private:

	// 座標変換
	void TransForm(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale);
};
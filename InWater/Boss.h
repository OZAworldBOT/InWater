/*
*	ボス
*
*	2016/06/10		Y.Ozawa
*/

#pragma once

class Texture;
class Model;
class Graphic;

class Boss
{
	//-----------------------------------------------------
	//	敵関連
	//-----------------------------------------------------
	Graphic *boss;						//	Xファイル
	struct BossState
	{
		D3DXVECTOR3 Position;			//	敵の座標
		D3DXVECTOR3 Rotation;			//	敵の傾き
		D3DXVECTOR3 Accel;				//	敵の初速
		D3DXVECTOR3 Scale;				//	敵の拡大率
		D3DXVECTOR3 oldEnemyPos;		//	前フレームの敵の座標
		D3DXVECTOR3 oldEnemyRot;		//	前フレームの敵の傾き
		D3DXVECTOR3 Collider;			//	敵の中心座標
		D3DXVECTOR3 MaxRange;			//	敵出現の最大範囲
		D3DXVECTOR3 MinRange;			//	敵出現の最小範囲
		int			Vitality;			//	敵の体力
		int			DeathCount;	//	敵に攻撃が当たった回数
		float		Radius;				//	敵の半径
		bool		DeathFlag;			//	敵が生きてるかどうか
		bool		HitFlag;			//	敵に当たった
		bool		BulletFlag;			//	敵に当たった
		bool		CountFlag;			//	敵が生きている数を調べる
		int			Speed;				//	速度の制御
		int			x_Speed;			//	拡大率の制御	x
		int			z_Speed;			//	拡大率の制御	z
		int			y_Speed;			//	拡大率の制御	y
		int			Count;				//	敵の数
	};

	//-----------------------------------------------------
	//	当たり判定関連
	//-----------------------------------------------------
	float		bullet_Radius[BULLET_MAX];		//	弾丸の半径
	float		razer_Radius[RAZER_MAX];		//	レーザーの半径


private:

	float cameraAngle;		//カメラの角度

private:

	Model *model;
	Texture *texture;
	BossState *State;

public:

	//	デフォルトコンストラクタ
	Boss();

	//	デストラクタ
	~Boss();

	//	敵の動作
	void Move();

	//	描画する
	void Draw();

	//	初期化
	void InitBoss();

	//	当たり判定
	void Hit();

	//	討伐の処理
	void DestroyEnemy();

	//	解放
	void Release();
};

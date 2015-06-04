/*
*	敵のクラス
*
*	2015/05/28		Y.Ozawa
*/

#pragma once

class Texture;
class Model;
class Graphic;
class Bullet;
class Explosion;

//粒子のデーター
struct e_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};


class Enemy
{
	//-----------------------------------------------------
	//	敵関連
	//-----------------------------------------------------
	Graphic *enemy;							//	Xファイル
	struct EnemyState
	{
		D3DXVECTOR3 Position[ENEMY_MAX];		//	敵の座標
		D3DXVECTOR3 Rotation[ENEMY_MAX];		//	敵の傾き
		D3DXVECTOR3 Accel[ENEMY_MAX];			//	敵の初速
		D3DXVECTOR3 Scale[ENEMY_MAX];			//	敵の拡大率
		D3DXVECTOR3 oldEnemyPos[ENEMY_MAX];		//	前フレームの敵の座標
		D3DXVECTOR3 oldEnemyRot[ENEMY_MAX];		//	前フレームの敵の傾き
		D3DXVECTOR3 Collider[ENEMY_MAX];		//	敵の中心座標
		D3DXVECTOR3 MaxRange;					//	敵出現の最大範囲
		D3DXVECTOR3 MinRange;					//	敵出現の最小範囲
		int			Vitality[ENEMY_MAX];		//	敵の体力
		float		Radius[ENEMY_MAX];			//	敵の半径
		bool		enemyDeathFlag[ENEMY_MAX];	//	敵が生きてるかどうか
		int			Speed[ENEMY_MAX];			//	速度の制御
		int			x_Speed[ENEMY_MAX];			//	拡大率の制御	x
		int			z_Speed[ENEMY_MAX];			//	拡大率の制御	z
		int			y_Speed[ENEMY_MAX];			//	拡大率の制御	y
	};
	//-----------------------------------------------------
	//	攻撃関連
	//-----------------------------------------------------
	float		bullet_Radius[BULLET_MAX];		//	弾丸の半径
	float		razer_Radius[RAZER_MAX];		//	レーザーの半径
	float		bomb_Radius[BOMB_MAX];			//	爆弾の半径
	//-----------------------------------------------------
	//	爆発関連
	//-----------------------------------------------------
	struct Eexplosion
	{
		D3DXVECTOR3 Pos[EXPLOSION_MAX];					//	爆発の座標
		D3DXVECTOR3 Rot[EXPLOSION_MAX];					//	爆発の傾き
		D3DXVECTOR3 Accel[EXPLOSION_MAX];				//	爆発の初速
		D3DXVECTOR3 oldPos[EXPLOSION_MAX];				//	前フレームの爆発の座標
		D3DXVECTOR3 MinRange[EXPLOSION_MAX];			//	爆発の最小範囲
		D3DXVECTOR3 MaxRange[EXPLOSION_MAX];			//	爆発の最大範囲
		int			Count[EXPLOSION_MAX];				//	爆発の生存時間
		bool		Exist[EXPLOSION_MAX];				//	爆発が生きてるかどうか
		bool		death[EXPLOSION_MAX];				
		bool		Flag;								//	制御フラグ
	};


private:

	float cameraAngle;		//カメラの角度

private:

	Model *model;
	Texture *texture;
	Bullet *bullet;
	EnemyState *State;
	Explosion *explosion;
	Eexplosion *eExp;

public:

	//	デフォルトコンストラクタ
	Enemy();

	//	デストラクタ
	~Enemy();

	//	敵の動作
	void Move();

	//	描画する
	void Draw();

	//	初期化
	void InitEnemy();
	void InitBullet();
	void InitExplosion();

	//	攻撃
	void Shot();

	//	当たり判定
	void Hit();

	//	爆発の処理
	void DestroyEnemy();

	//	解放
	void Release();
};









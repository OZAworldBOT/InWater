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
		int			enemyDeathCount;			//	敵に攻撃が当たった回数
		float		Radius[ENEMY_MAX];			//	敵の半径
		bool		enemyDeathFlag[ENEMY_MAX];	//	敵が生きてるかどうか
		bool		enemyHitFlag[ENEMY_MAX];	//	敵に当たった
		bool		enemyBulletFlag;			//	敵に当たった
		bool		enemyCountFlag[ENEMY_MAX];	//	敵が生きている数を調べる
		int			Speed[ENEMY_MAX];			//	速度の制御
		int			x_Speed[ENEMY_MAX];			//	拡大率の制御	x
		int			z_Speed[ENEMY_MAX];			//	拡大率の制御	z
		int			y_Speed[ENEMY_MAX];			//	拡大率の制御	y
		int			Count;						//	敵の数
	};
	//-----------------------------------------------------
	//	攻撃関連
	//-----------------------------------------------------
	struct EnemyAttack
	{
		D3DXVECTOR3 Pos;			//	弾の座標
		D3DXVECTOR3 Rot;			//	弾の傾き
		D3DXVECTOR3 Accel;			//	弾の初速
		D3DXVECTOR3 oldPos;			//	前フレームの弾の座標
		int			Frame;			//	経過フレーム
		int			Count;			//	弾の生存時間
		bool		Exist;			//	弾が生きてるかどうか
		bool		flag;			//	弾の制御フラグ
		bool		death;			//	弾が敵に当たった
		float		Radiu;			//	弾の半径
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
	Bullet *bullet;
	EnemyState *State;
	EnemyAttack *Attack;

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

	//	攻撃
	void Shot();

	//	当たり判定
	void Hit();

	//	爆発の処理
	void DestroyEnemy();

	//	解放
	void Release();
};
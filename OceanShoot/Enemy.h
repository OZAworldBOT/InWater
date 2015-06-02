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

	//-----------------------------------------------------
	//	攻撃関連
	//-----------------------------------------------------
	float		bullet_Radius[BULLET_MAX];		//	弾丸の半径
	float		razer_Radius[RAZER_MAX];		//	レーザーの半径
	float		bomb_Radius[BOMB_MAX];			//	爆弾の半径
	
private:

	float cameraAngle;		//カメラの角度

private:

	Model *model;
	Texture *texture;
	Bullet *bullet;

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

	//	解放
	void Release();
};









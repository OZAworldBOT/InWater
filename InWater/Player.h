/*
*	プレイヤーのクラス
*
*	2015/05/27		Y.Ozawa
*/

#pragma once

class Camera;
class Model;
class Texture;
class Graphic;
class BUllet;
class Bomb;
class Razer;

//粒子のデーター
struct p_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};

class Player
{
	float		timeCount;		//	経過時間

	//-----------------------------------------------------
	//	プレイヤー関連
	//-----------------------------------------------------
	//	Xファイル
	Graphic		*player;
	Graphic		*HpBar;
	D3DXVECTOR3 Position;											//	プレイヤーの座標
	D3DXVECTOR3 Rotation;											//	プレイヤーの傾き
	D3DXVECTOR3 Accel;												//	プレイヤーの初速
	D3DXVECTOR3 Scale;												//	プレイヤーの拡大率
	D3DXVECTOR3 oldPlayerPos;										//	前フレームのプレイヤーの座標
	D3DXVECTOR3 oldPlayerRot;										//	前フレームのプレイヤーの傾き
	int			Count;												//	制御カウンター
	float		Vitality;											//	プレイヤーの体力
	float		player_Radius;										//	プレイヤーの半径
	float		enemy_Radius[ENEMY_MAX];							//	敵の半径
	float		enemyBullet_Radius[ENEMY_MAX];						//	敵の弾の半径
	float		UenemyBullet_Radius[ENEMY_MAX];						//	下の敵の弾の半径
	float		Boss_Radius;										//	ボスの半径
	float		x_Speed;
	float		y_Speed;
	float		z_Speed;
	bool		playerHitFlag;										//	敵の弾が当たったかどうか

	struct HitPoint
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scale;
		D3DXVECTOR3 Accel;
	};
	//-----------------------------------------------------
	//	カメラ関連
	//-----------------------------------------------------
	D3DXVECTOR3 camera_Pos;						//	カメラの座標
	D3DXVECTOR3 camera_Rot;						//	カメラの傾き
	D3DXVECTOR3 oldCameraRot;					//	前フレームのカメラの傾き

	//-----------------------------------------------------
	//	攻撃関連
	//-----------------------------------------------------
	//	通常弾
	struct Pbullet
	{
		D3DXVECTOR3 Pos[BULLET_MAX];			//	弾の座標
		D3DXVECTOR3 Rot[BULLET_MAX];			//	弾の傾き
		D3DXVECTOR3 Accel[BULLET_MAX];			//	弾の初速
		D3DXVECTOR3 oldPos[BULLET_MAX];			//	前フレームの弾の座標
		int			Count[BULLET_MAX];			//	弾の生存時間
		bool		Exist[BULLET_MAX];			//	弾が生きてるかどうか
		bool		flag;						//	弾の制御フラグ
		bool		death[BULLET_MAX];			//	弾が敵に当たった
		float		Radius[BULLET_MAX];			//	弾の半径
	};
	//	爆弾
	struct Pbomb
	{
		D3DXVECTOR3 Pos;							//	爆弾の座標
		D3DXVECTOR3 Rot;							//	爆弾の傾き
		D3DXVECTOR3 Accel;							//	爆弾の初速
		D3DXVECTOR3 oldPos;							//	前フレームの爆弾の座標
		bool		Exist;							//	爆弾が生きてるかどうか
		bool		flag;							//	爆弾の制御フラグ
		bool		death;							//	爆弾が敵に当たった
		bool		Alpha;							//	Zバッファーを行うかどうか
		float		Radius;							//	爆弾の半径
		float		Size;							//	爆弾の大きさ
		int			Count;							//	爆弾の生存時間
		D3DXVECTOR3 Explosion_Rot[BOMB_MAX];		//	爆風の傾き
		D3DXVECTOR3 Explosion_Pos[BOMB_MAX];		//	爆風の座標
		D3DXVECTOR3 Explosion_Accel[BOMB_MAX];		//	爆風の初速
		D3DXVECTOR3 oldExplosionPos[BOMB_MAX];		//	前フレームの爆風の座標
		D3DXVECTOR3 MinRange;						//	爆風の最小範囲
		D3DXVECTOR3 MaxRange;						//	爆風の最大範囲
		bool		Explosion_Exist[BOMB_MAX];		//	爆風が生きてるかどうか
		bool		Explosion_Death[BOMB_MAX];		//	爆風が敵に当たった
		bool		Explosion_Alpha;				//	Zバッファーを行うかどうか
		float		Explosion_Radius[BOMB_MAX];		//	爆風の半径
		float		Explosion_Size;					//	爆風の大きさ
		int			Explosion_Count[BOMB_MAX];		//	爆風の生存時間
	};
	//	レーザー
	struct Prazer
	{
		D3DXVECTOR3 Pos[RAZER_MAX];				//	レーザーの座標
		D3DXVECTOR3 Rot[RAZER_MAX];				//	レーザーの傾き
		D3DXVECTOR3 Accel[RAZER_MAX];			//	レーザーの初速
		D3DXVECTOR3 oldPos[RAZER_MAX];			//	前フレームのレーザーの座標
		int			Count[RAZER_MAX];			//	レーザーの生存時間
		bool		Exist[RAZER_MAX];			//	レーザーが生きてるかどうか
		bool		flag;						//	レーザーの制御フラグ
		bool		death[RAZER_MAX];			//	レーザーが敵に当たった
		float		Radius[RAZER_MAX];			//	レーザーの半径
	};


private:

	float cameraAngle;		//カメラの角度

private:

	Camera		*camera;
	Model		*model;
	Texture		*texture;
	Bullet		*bullet;
	Bomb		*bomb;
	Razer		*razer;
	Explosion	*explosion;
	Pbullet		*pBullet;
	Pbomb		*pBomb;
	Prazer		*pRazer;
	HitPoint	*hp;

public:

	//	デフォルトコンストラクタ
	Player();

	//	デストラクタ
	~Player();

	//	プレイヤーの動作
	void Move();

	//	描画する
	void Draw();
	void View();

	//	初期化
	void InitPlayer();
	void InitBullet();
	void InitBomb();
	void InitRazer();
	void InitHpBar();
	void ResetPlayer();

	//	ショット
	void Shot();

	//	弾丸のショット
	void BulletShot();

	//	爆弾のショット
	void BombShot();

	//	レーザーのショット
	void RazerShot();

	//	当たり判定
	void Destroy();

	//	解放
	void Release();
};







#pragma once
#include ".\Library\GameObject.h"
#include "global.h"

//enum class ENEMY_STATE
//{
//	PATROL,
//	CHASE,
//	ATTACK,
//	SEARCH
//};

class EnemyStateBase;

class Enemy :
    public GameObject
{
	int hImage_;//画像ID
	Point pos_;//位置
	DIR dir_;//移動方向
	EnemyStateBase* currentState_ = nullptr;
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;

	// 各Stateクラスから使うためのアクセサ
	Point GetPos() const { return pos_; }
	void SetPos(Point pos) { pos_ = pos; }
	DIR GetDir() const { return dir_; }
	void SetDir(DIR dir) { dir_ = dir; }

	void ChangeState(EnemyStateBase* newState);
	bool TryMove(DIR dir); // マス目移動を試みる。壁などで失敗したらfalse
	float GetDistSqToPlayer() const;
	Point GetVectorToPlayer() const; // プレイヤーへの相対ベクトル(dx, dy)
private:
	//ENEMY_STATE state_ = ENEMY_STATE::PATROL;
	//float searchTimer_ = 0.0f;
	//float coolTimer_ = 0.0f;
	//const float coolTimeMax_ = 1.0f;//攻撃間隔（秒）
	//void UpdatePatrol();
	//void UpdateChase();
	//void UpdateAttack();
	//void UpdateSearch();
};
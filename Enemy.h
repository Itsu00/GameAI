#pragma once
#include ".\Library\GameObject.h"
#include "global.h"
#include "EnemyStateBase.h"

class Enemy :
    public GameObject
{
	int hImage_;//‰æ‘œID
	Point pos_;//ˆÊ’u
	DIR dir_;//ˆÚ“®•ûŒü

public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;

	void ChangeState(EnemyStateBase* nextState);
	void ApplyStateChange();

	bool CheckCanSeePlayer();
	bool CheckAttackRange();
	bool CheckSearchTimerOver();

	void Patrol();
	void Chase();
	void Attack();
	void Search();

private:
	EnemyStateBase* state_ = nullptr;
	EnemyStateBase* nextState_ = nullptr;
};
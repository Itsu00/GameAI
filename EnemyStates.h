#pragma once
#include "EnemyStateBase.h"

class PatrolState : public EnemyStateBase
{
public:
	void Update(Enemy& enemy) override;
private:
	float dirTimer_ = 3.0f;
	float progTimer_ = 0.5f;
};

class ChaseState : public EnemyStateBase
{
public:
	void Update(Enemy& enemy) override;
private:
	float moveTimer_ = 0.2f;
};

class AttackState : public EnemyStateBase
{
public:
	void Update(Enemy& enemy) override;
private:
	float coolTimer_ = 0.0f;
	const float coolTimeMax_ = 1.0f;
};

class SearchState : public EnemyStateBase
{
public:
	void Update(Enemy& enemy) override;
private:
	float searchTimer_ = 0.0f;
	float dirTimer_ = 1.0f;
	float progTimer_ = 0.5f;
};
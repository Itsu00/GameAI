#pragma once
class Enemy;

class EnemyStateBase
{
public:
	virtual ~EnemyStateBase() = default;
	virtual void Update(Enemy& enemy) = 0;
};
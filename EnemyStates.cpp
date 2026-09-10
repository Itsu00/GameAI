#include "EnemyStates.h"
#include "Enemy.h"
#include "EnemyConstants.h"
#include "time.h"
#include "global.h"
#include "Library/ObjectManager.h"
#include "Player.h"
#include <cmath>

using namespace EnemyConst;

void PatrolState::Update(Enemy& enemy)
{
	float dt = Time::DeltaTime();
	dirTimer_ -= dt;
	progTimer_ -= dt;

	if (dirTimer_ < 0.0f)
	{
		enemy.SetDir((DIR)(GetRand(3)));
		dirTimer_ = 3.0f + dirTimer_;
	}
	if (progTimer_ < 0.0f)
	{
		enemy.TryMove(enemy.GetDir());
		progTimer_ = 0.5f + progTimer_;
	}

	if (enemy.GetDistSqToPlayer() <= SEARCH_RANGE * SEARCH_RANGE)
	{
		enemy.ChangeState(new ChaseState());
		return; // ChangeState内でthis(このPatrolState)がdeleteされるので、これ以上メンバに触らない
	}
}

void ChaseState::Update(Enemy& enemy)
{
	float distSq = enemy.GetDistSqToPlayer();

	if (distSq <= ATTACK_RANGE * ATTACK_RANGE)
	{
		enemy.ChangeState(new AttackState());
		return;
	}
	if (distSq > SEARCH_RANGE_END * SEARCH_RANGE_END)
	{
		enemy.ChangeState(new SearchState());
		return;
	}

	moveTimer_ -= Time::DeltaTime();
	if (moveTimer_ < 0.0f)
	{
		Point vec = enemy.GetVectorToPlayer();
		DIR dir = (fabsf(vec.x) > fabsf(vec.y))
			? (vec.x > 0 ? RIGHT : LEFT)
			: (vec.y > 0 ? DOWN : UP);
		enemy.SetDir(dir);
		enemy.TryMove(dir);
		moveTimer_ = 0.5f + moveTimer_;
	}
}

void AttackState::Update(Enemy& enemy)
{
	float distSq = enemy.GetDistSqToPlayer();

	if (distSq > ATTACK_RANGE * ATTACK_RANGE)
	{
		enemy.ChangeState(new SearchState());
		return;
	}

	coolTimer_ -= Time::DeltaTime();
	if (coolTimer_ <= 0.0f)
	{
		FindGameObject<Player>()->DestroyMe();
		SceneManager::ChangeScene("TITLE");
		coolTimer_ = coolTimeMax_;
	}
}

void SearchState::Update(Enemy& enemy)
{
	searchTimer_ += Time::DeltaTime();
	float distSq = enemy.GetDistSqToPlayer();

	if (distSq <= ATTACK_RANGE * ATTACK_RANGE)
	{
		enemy.ChangeState(new AttackState());
		return;
	}
	if (distSq <= SEARCH_RANGE * SEARCH_RANGE)
	{
		enemy.ChangeState(new ChaseState());
		return;
	}
	if (searchTimer_ >= SEARCH_TIME_LIMIT)
	{
		enemy.ChangeState(new PatrolState());
		return;
	}

	float dt = Time::DeltaTime();
	dirTimer_ -= dt;
	progTimer_ -= dt;

	if (dirTimer_ < 0.0f)
	{
		enemy.SetDir((DIR)(GetRand(3)));
		dirTimer_ = 1.0f + dirTimer_;
	}
	if (progTimer_ < 0.0f)
	{
		enemy.TryMove(enemy.GetDir());
		progTimer_ = 0.5f + progTimer_;
	}
}
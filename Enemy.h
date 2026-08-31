#pragma once
#include ".\Library\GameObject.h"
#include "global.h"

enum class ENEMY_STATE
{
	PATROL,
	CHASE,
	ATTACK,
	SEARCH
};

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
private:
	ENEMY_STATE state_ = ENEMY_STATE::PATROL;
	float searchTimer_ = 0.0f;

	void UpdatePatrol();
	void UpdateChase();
	void UpdateAttack();
	void UpdateSearch();
};
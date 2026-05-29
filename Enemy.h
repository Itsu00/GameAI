#pragma once
#include ".\Library\GameObject.h"
#include "global.h"

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

	void ChasePlayer(Point playerPos);
	void WallFollow();
	void TryMove(DIR dirs[4]);
	void ReturnToWall();
private:
	bool isChasing_ = false;//’Ç‚¢‚©‚¯’†‚©
	DIR wallDir_;//„‰ñ—p‚Ì•ûŒü‚ğ•Ê‚É•Û
};
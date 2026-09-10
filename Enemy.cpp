#include "Enemy.h"
#include "EnemyStates.h"
#include "EnemyConstants.h"
#include "time.h"
#include "Library/ObjectManager.h"
#include "Stage.h"
#include "Player.h"
//#include <cmath>

using namespace EnemyConst;

namespace
{
	const int ENEMY_SIZE = 48; //敵のサイズ 32*32
	const Point ENEMY_START_POS = { 24.68 * ENEMY_SIZE, 12.68 * ENEMY_SIZE }; //敵の初期位置
	const DIR INIT_ENEMY_DIR = { LEFT };
	//const int ENEMY_DRAW_SIZE = 32; //敵の描画サイズ
	const int animFrame[4]{ 0, 1, 2, 1 };
	const float ANIM_INTERVAL = 0.2f;

	/*const float CHASE_RANGE = 5.0f * CHA_SIZE;//追いかけ距離
	const float CHASE_RANGE_END = 7.0f * CHA_SIZE;  // 追跡終了距離（少し広め）
	const float SEARCH_RANGE = 5.0f * ENEMY_DRAW_SIZE;//発見
	const float SEARCH_RANGE_END = 7.0f * ENEMY_DRAW_SIZE;//見失う
	const float ATTACK_RANGE = 0.5f * ENEMY_DRAW_SIZE;//攻撃できる範囲
	const float SEARCH_TIME_LIMIT = 3.0f;//うろうろする探索時間*/
}

Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
	currentState_ = new PatrolState();
}

Enemy::~Enemy()
{
	delete currentState_;
}

void Enemy::Update()
{
	if (!FindGameObject<Player>()) return;
	currentState_->Update(*this);
	/*switch (state_)
	{
	case ENEMY_STATE::PATROL:
		UpdatePatrol();
		break;
	case ENEMY_STATE::CHASE:
		UpdateChase();
		break;
	case ENEMY_STATE::ATTACK:
		UpdateAttack();
		break;
	case ENEMY_STATE::SEARCH:
		UpdateSearch();
		break;
	}*/

	/*//GetRand(数値)
	//3秒に1回向きをランダムに変える
	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	dir_timer = dir_timer - dt;
	prog_timer = prog_timer - dt;
	if (dir_timer < 0.0f)
	{
		dir_ = (DIR)(GetRand(3));
		dir_timer = 3.0f + dir_timer;
	}
	Point newPos = pos_;
	if (prog_timer < 0.0f)
	{
		switch (dir_)
		{
		case UP:
			newPos.y -= ENEMY_DRAW_SIZE;
			break;
		case DOWN:
			newPos.y += ENEMY_DRAW_SIZE;
			break;
		case LEFT:
			newPos.x -= ENEMY_DRAW_SIZE;
			break;
		case RIGHT:
			newPos.x += ENEMY_DRAW_SIZE;
			break;
		default:
			break;
		}
		//移動先がステージの外に出ないようにする
		if (!(newPos.x < 1 || newPos.x >(STAGE_WIDTH - 2) * ENEMY_DRAW_SIZE
			|| newPos.y < 1 || newPos.y >(STAGE_HEIGHT - 2) * ENEMY_DRAW_SIZE))
		{
			pos_ = newPos;
		}
		prog_timer = 0.5f + prog_timer;
	}*/
}

void Enemy::Draw()
{
	// --- 範囲の可視化(マス目単位) ---
	int centerX = pos_.x + ENEMY_DRAW_SIZE / 2;
	int centerY = pos_.y + ENEMY_DRAW_SIZE / 2;
	// 敵のいるタイル座標
	int enemyTileX = pos_.x / ENEMY_DRAW_SIZE;
	int enemyTileY = pos_.y / ENEMY_DRAW_SIZE;

	// 一番外側の範囲(SEARCH_RANGE_END)がカバーするタイル数を求めてループ範囲にする
	int radiusTiles = (int)(SEARCH_RANGE_END / ENEMY_DRAW_SIZE) + 1;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

	for (int ty = enemyTileY - radiusTiles; ty <= enemyTileY + radiusTiles; ty++)
	{
		for (int tx = enemyTileX - radiusTiles; tx <= enemyTileX + radiusTiles; tx++)
		{
			// ステージ範囲外は無視
			if (tx < 0 || tx >= STAGE_WIDTH || ty < 0 || ty >= STAGE_HEIGHT) continue;

			// タイル中心座標との距離で判定
			int tileCenterX = tx * ENEMY_DRAW_SIZE + ENEMY_DRAW_SIZE / 2;
			int tileCenterY = ty * ENEMY_DRAW_SIZE + ENEMY_DRAW_SIZE / 2;
			float dx = (float)(tileCenterX - centerX);
			float dy = (float)(tileCenterY - centerY);
			float distSq = dx * dx + dy * dy;

			int color = -1; // -1なら描画しない
			if (distSq <= ATTACK_RANGE * ATTACK_RANGE)
				color = GetColor(255, 0, 0);        // 攻撃範囲:赤
			else if (distSq <= SEARCH_RANGE * SEARCH_RANGE)
				color = GetColor(255, 150, 0);      // 発見範囲:オレンジ
			else if (distSq <= SEARCH_RANGE_END * SEARCH_RANGE_END)
				color = GetColor(255, 255, 0);      // 見失う範囲:黄色

			if (color != -1)
			{
				DrawBox(tx * ENEMY_DRAW_SIZE, ty * ENEMY_DRAW_SIZE,
					(tx + 1) * ENEMY_DRAW_SIZE, (ty + 1) * ENEMY_DRAW_SIZE,
					color, TRUE);
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	static float animTimer = ANIM_INTERVAL;
	static int frame = 0;
	int nowFrame = animFrame[frame];

	Rect iRect[4] = {
		{  nowFrame * ENEMY_SIZE, 3 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 0 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 1 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 2 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE}
	};
	DrawBox(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		GetColor(255, 255, 0), FALSE, 2);
	DrawRectExtendGraph(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		iRect[dir_].x, iRect[dir_].y, iRect[dir_].w, iRect[dir_].h, hImage_, TRUE);
	if (animTimer < 0) {
		frame = (++frame) % 4;
		animTimer = ANIM_INTERVAL + animTimer;
	}
	animTimer = animTimer - Time::DeltaTime();
}

void Enemy::ChangeState(EnemyStateBase* newState)
{
	delete currentState_;
	currentState_ = newState;
}

bool Enemy::TryMove(DIR dir)
{
	Point newPos = pos_;
	switch (dir)
	{
	case UP:	newPos.y -= ENEMY_DRAW_SIZE; break;
	case DOWN:  newPos.y += ENEMY_DRAW_SIZE; break;
	case LEFT:  newPos.x -= ENEMY_DRAW_SIZE; break;
	case RIGHT: newPos.x += ENEMY_DRAW_SIZE; break;
	}

	if (newPos.x < 1 || newPos.x >(STAGE_WIDTH - 2) * ENEMY_DRAW_SIZE ||
		newPos.y < 1 || newPos.y >(STAGE_HEIGHT - 2) * ENEMY_DRAW_SIZE)
	{
		return false;
	}
	pos_ = newPos;
	return true;
}

float Enemy::GetDistSqToPlayer() const
{
	Point v = GetVectorToPlayer();
	return v.x * v.x + v.y * v.y;
}

Point Enemy::GetVectorToPlayer() const
{
	Point playerPos = FindGameObject<Player>()->GetPlayerPos();
	return { playerPos.x - pos_.x, playerPos.y - pos_.y };
}

/*void Enemy::UpdatePatrol()
{
	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	dir_timer -= dt;
	prog_timer -= dt;

	if (dir_timer < 0.0f)
	{
		dir_ = (DIR)(GetRand(3));
		dir_timer = 3.0f + dir_timer;
	}

	Point newPos = pos_;
	if (prog_timer < 0.0f)
	{
		switch (dir_)
		{
		case UP:	newPos.y -= ENEMY_DRAW_SIZE; break;
		case DOWN:  newPos.y += ENEMY_DRAW_SIZE; break;
		case LEFT:  newPos.x -= ENEMY_DRAW_SIZE; break;
		case RIGHT: newPos.x += ENEMY_DRAW_SIZE; break;
		}

		if (!(newPos.x < 1 || newPos.x >(STAGE_WIDTH - 2) * ENEMY_DRAW_SIZE ||
			  newPos.y < 1 || newPos.y >(STAGE_HEIGHT - 2) * ENEMY_DRAW_SIZE))
		{
			pos_ = newPos;
		}
		prog_timer = 0.5f + prog_timer;
	}

	//視界に入ったらChaseへ
	float distSq = Math2D::LengthSq(pos_, Player::GetInstance()->GetPos());
	if (distSq <= SEARCH_RANGE * SEARCH_RANGE)
	{
		state_ = ENEMY_STATE::CHASE;
	}
	Point playerPos = FindGameObject<Player>()->GetPlayerPos();
	float dx = playerPos.x - pos_.x;
	float dy = playerPos.y - pos_.y;
	float distSq = dx * dx + dy * dy;

	if (distSq <= SEARCH_RANGE * SEARCH_RANGE)
	{
		state_ = ENEMY_STATE::CHASE;
	}
}*/

/*void Enemy::UpdateChase()
{
	//Point playerPos = Player::GetInstance()->GetPos();
	//float distSq = Math2D::LengthSq(pos_, playerPos);
	Point playerPos = FindGameObject<Player>()->GetPlayerPos();
	float dx = playerPos.x - pos_.x;
	float dy = playerPos.y - pos_.y;
	float distSq = dx * dx + dy * dy;

	// 攻撃距離に入ったらAttackへ
	if (distSq <= ATTACK_RANGE * ATTACK_RANGE)
	{
		state_ = ENEMY_STATE::ATTACK;
		return;
	}

	// プレイヤー方向へ距離を詰める
	//Point dir = {playerPos.x - pos_.x, playerPos.y - pos_.y};
	//float len = sqrtf(distSq);
	//if (len > 0.0f)
	//{
	//	float speed = ENEMY_DRAW_SIZE * Time::DeltaTime(); // 1フレームの移動量
	//	pos_.x += dir.x / len * speed;
	//	pos_.y += dir.y / len * speed;
	//}

	// 一定以上離れたら見失ってSearchへ
	if (distSq > SEARCH_RANGE_END * SEARCH_RANGE_END)
	{
		state_ = ENEMY_STATE::SEARCH;
		searchTimer_ = 0.0f;
		return;
	}

	// プレイヤー方向へマス目移動（Patrolと同じ形）
	static float move_timer = 0.2f;
	move_timer -= Time::DeltaTime();
	if (move_timer < 0.0f)
	{
		// dx, dyの絶対値が大きい方の軸で向きを決める
		dir_ = (fabsf(dx) > fabsf(dy))
			? (dx > 0 ? RIGHT : LEFT)
			: (dy > 0 ? DOWN : UP);

		Point newPos = pos_;
		switch (dir_)
		{
		case UP:	newPos.y -= ENEMY_DRAW_SIZE; break;
		case DOWN:  newPos.y += ENEMY_DRAW_SIZE; break;
		case LEFT:  newPos.x -= ENEMY_DRAW_SIZE; break;
		case RIGHT: newPos.x += ENEMY_DRAW_SIZE; break;
		}
		if (!(newPos.x < 1 || newPos.x >(STAGE_WIDTH - 2) * ENEMY_DRAW_SIZE ||
			newPos.y < 1 || newPos.y >(STAGE_HEIGHT - 2) * ENEMY_DRAW_SIZE))
		{
			pos_ = newPos;
		}
		move_timer = 0.5f + move_timer;
	}
}*/

/*void Enemy::UpdateAttack()
{
	//Point playerPos = Player::GetInstance()->GetPos();
	//float distSq = Math2D::LengthSq(pos_, playerPos);
	Point playerPos = FindGameObject<Player>()->GetPlayerPos();
	float dx = playerPos.x - pos_.x;
	float dy = playerPos.y - pos_.y;
	float distSq = dx * dx + dy * dy;

	// 攻撃距離から逃げられたらSearchへ
	if (distSq > ATTACK_RANGE * ATTACK_RANGE)
	{
		state_ = ENEMY_STATE::SEARCH;
		searchTimer_ = 0.0f;
		return;
	}

	// ここに攻撃処理(クールタイム管理など)を書く
	// クールタイムが切れたら攻撃
	coolTimer_ -= Time::DeltaTime();
	if (coolTimer_ <= 0.0f)
	{
		FindGameObject<Player>()->DestroyMe();
		SceneManager::ChangeScene("TITLE");
		coolTimer_ = coolTimeMax_;
	}
}*/

/*void Enemy::UpdateSearch()
{
	searchTimer_ += Time::DeltaTime();

	//Point playerPos = Player::GetInstance()->GetPos();
	//float distSq = Math2D::LengthSq(pos_, playerPos);
	Point playerPos = FindGameObject<Player>()->GetPlayerPos();
	float dx = playerPos.x - pos_.x;
	float dy = playerPos.y - pos_.y;
	float distSq = dx * dx + dy * dy;

	// 攻撃距離まで詰まっていたら直接Attackへ
	if (distSq <= ATTACK_RANGE * ATTACK_RANGE)
	{
		state_ = ENEMY_STATE::ATTACK;
		return;
	}

	// 再び視界に入ったらAttack...ではなくChaseの方が自然かもしれません
	if (distSq <= SEARCH_RANGE * SEARCH_RANGE)
	{
		state_ = ENEMY_STATE::CHASE;
		return;
	}

	// うろうろして見つからなかったらPatrolへ
	if (searchTimer_ >= SEARCH_TIME_LIMIT)
	{
		state_ = ENEMY_STATE::PATROL;
		return;
	}

	// ここにうろうろ移動処理(Patrolのランダム移動を流用してもOK)
	static float dir_timer = 1.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	dir_timer -= dt;
	prog_timer -= dt;

	if (dir_timer < 0.0f)
	{
		dir_ = (DIR)(GetRand(3));
		dir_timer = 1.0f + dir_timer;
	}

	if (prog_timer < 0.0f)
	{
		Point newPos = pos_;
		switch (dir_)
		{
		case UP:	newPos.y -= ENEMY_DRAW_SIZE; break;
		case DOWN:  newPos.y += ENEMY_DRAW_SIZE; break;
		case LEFT:  newPos.x -= ENEMY_DRAW_SIZE; break;
		case RIGHT: newPos.x += ENEMY_DRAW_SIZE; break;
		}

		if (!(newPos.x < 1 || newPos.x >(STAGE_WIDTH - 2) * ENEMY_DRAW_SIZE ||
			newPos.y < 1 || newPos.y >(STAGE_HEIGHT - 2) * ENEMY_DRAW_SIZE))
		{
			pos_ = newPos;
		}
		prog_timer = 0.5f + prog_timer;
	}
}*/
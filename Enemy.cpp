#include "Enemy.h"
#include "time.h"
#include "Library/ObjectManager.h"
#include "Stage.h"
#include "Player.h"

namespace
{
	const int ENEMY_SIZE = 48; //敵のサイズ 32*32
	const Point ENEMY_START_POS = { 24.68 * ENEMY_SIZE, 12.68 * ENEMY_SIZE }; //敵の初期位置
	const DIR INIT_ENEMY_DIR = { LEFT };
	const int ENEMY_DRAW_SIZE = 32; //敵の描画サイズ
	const int animFrame[4]{ 0, 1, 2, 1 };
	const float ANIM_INTERVAL = 0.2f;
	const float CHASE_RANGE = 5.0f * CHA_SIZE;//追いかけ距離
}

Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	// prog_timerの外、一番上に追加
	printfDx("pos_.x:%d  pos_.y:%d\n", pos_.x, pos_.y);

	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	dir_timer = dir_timer - dt;
	prog_timer = prog_timer - dt;

	/*if (dir_timer < 0.0f) {
		dir_ = (DIR)(GetRand(3));
		dir_timer = 3.0f + dir_timer;
	}*/

	if (prog_timer < 0.0f)
	{
		GameObject* playerObj = FindGameObject<Player>();

		if (playerObj != nullptr) {
			Player* player = dynamic_cast<Player*>(playerObj);
			Point playerPos = player->GetPlayerPos();

			float dx = playerPos.x - pos_.x;
			float dy = playerPos.y - pos_.y;
			float dist = sqrtf(dx * dx + dy * dy);

			if (dist < CHASE_RANGE) {
				if (!isChasing_) {
					wallDir_ = dir_;//巡回中の方向を保存
					isChasing_ = true;
				}
				ChasePlayer(playerPos);//追いかけ
			}
			else {
				if (isChasing_) {//巡回に戻るときに方向を復元
					dir_ = wallDir_;//保存しておいた方向を戻す
					isChasing_ = false;
					ReturnToWall();
				}
				WallFollow();//通常
			}
		}
		prog_timer = 0.5f + prog_timer;
	}
}
//プレイヤーも壁から外に出ないようにする
//パンダを壁沿いにぐるぐる回るようにする
//元の移動処理はコメントにしておく

void Enemy::Draw()
{
	static float animTimer = ANIM_INTERVAL;
	static int frame = 0;
	int nowFrame = animFrame[frame];

	Rect iRect[4] = {
		{  nowFrame * ENEMY_SIZE, 3 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 2 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 0 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 1 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE}
	};
	DrawBox(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		GetColor(255, 255, 0), FALSE,2);
	DrawRectExtendGraph(pos_.x, pos_.y,pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		               iRect[dir_].x, iRect[dir_].y, iRect[dir_].w, iRect[dir_].h, hImage_, TRUE);
	if (animTimer < 0) {
		frame = (++frame) % 4;
		animTimer = ANIM_INTERVAL + animTimer;
	}
	animTimer = animTimer - Time::DeltaTime();
}

void Enemy::ChasePlayer(Point playerPos)
{
	DIR dirs[4];
	float dx = playerPos.x - pos_.x;
	float dy = playerPos.y - pos_.y;

	if (abs(dx) > abs(dy)) {
		dirs[0] = (dx > 0) ? RIGHT : LEFT;
		dirs[1] = (dy > 0) ? DOWN : UP;
		dirs[2] = (dy > 0) ? UP : DOWN;
		dirs[3] = (dx > 0) ? LEFT : RIGHT;
	}
	else {
		dirs[0] = (dy > 0) ? DOWN : UP;
		dirs[1] = (dx > 0) ? RIGHT : LEFT;
		dirs[2] = (dx > 0) ? LEFT : RIGHT;
		dirs[3] = (dy > 0) ? UP : DOWN;
	}
	TryMove(dirs);
}

void Enemy::WallFollow()
{
	DIR dirs[4];
	dirs[0] = (DIR)((dir_ + 3) % 4);
	dirs[1] = dir_;
	dirs[2] = (DIR)((dir_ + 1) % 4);
	dirs[3] = (DIR)((dir_ + 2) % 4);

	TryMove(dirs);
}

void Enemy::TryMove(DIR dirs[4])
{
	for (int i = 0; i < 4; i++) {
		Point newPos = pos_;
		switch (dirs[i])
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

		int mapValue = FindGameObject<Stage>()->GetMap(newPos.x / CHA_SIZE, newPos.y / CHA_SIZE);
		//Stage* stage = FindGameObject<Stage>();
		//移動先がステージの外に出ないようにする
		if (mapValue != 1)
		{
			dir_ = dirs[i];
			pos_ = newPos;
			break;
		}
	}
}

void Enemy::ReturnToWall()
{
	DIR dirs[4] = { UP,DOWN,LEFT,RIGHT };
	for (int i = 0;i < 4;i++) {
		Point checkPos = pos_;
		switch (dirs[i]) {
		case UP: checkPos.y -= ENEMY_DRAW_SIZE; break;
		case DOWN: checkPos.y += ENEMY_DRAW_SIZE; break;
		case LEFT: checkPos.x -= ENEMY_DRAW_SIZE; break;
		case RIGHT: checkPos.x += ENEMY_DRAW_SIZE; break;
		}

		int mapValue = FindGameObject<Stage>()->GetMap(checkPos.x / CHA_SIZE, checkPos.y / CHA_SIZE);

		printfDx("dir:%d checkPos:%d,%d mapValue:%d\n", dirs[i], checkPos.x / CHA_SIZE, checkPos.y / CHA_SIZE, mapValue);

		if (mapValue == 1)
		{
			dir_ = (DIR)((dirs[i] + 2) % 4);
			printfDx("壁発見！新しいdir_:%d\n", dir_);
			return;
		}
	}
	dir_ = wallDir_;//周囲に壁がない場合はwallDirに戻す
	printfDx("壁なし！wallDir_に戻す:%d\n", wallDir_);
}
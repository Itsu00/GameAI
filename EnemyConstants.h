#pragma once

namespace EnemyConst
{
	const int ENEMY_DRAW_SIZE = 32;

	const float SEARCH_RANGE = 5.0f * ENEMY_DRAW_SIZE;//発見距離
	const float SEARCH_RANGE_END = 7.0f * ENEMY_DRAW_SIZE;//見失う距離
	const float ATTACK_RANGE = 0.5f * ENEMY_DRAW_SIZE;//攻撃距離
	const float SEARCH_TIME_LIMIT = 3.0f;//うろうろする時間
}
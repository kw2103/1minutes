#ifndef ENEMY_H
#define ENEMY_H

#include "Player.h"
#include "../GameLib/game_lib.h"

constexpr float ENEMY_TEX_W = 160.0f;
constexpr float ENEMY_TEX_H = 246.0f;
constexpr int ENEMY_MAX = 3;

//class Enemy
//{
//	GameLib::Sprite* spr;
//	VECTOR2 pos;
//	float gravity;
//	int back_num;
//	int back_count;
//	bool exsit;
//	bool type;
//	bool move_through;
//	bool moveX_through;
//	bool moveY_through;
//public:
//	VECTOR2 velocity;
//	bool onGround;
//	bool onGround_through;
//	Enemy(int backNum, int next_backNum, int backCount, int stageNum, int enemyNum, VECTOR2 backPos, VECTOR2 stagePos, GameLib::Sprite* sprite);
//	void set_enemyPositionY(float posY);
//	VECTOR2 get_enemyPosition();
//	bool get_enemyExsit();
//	void enemy_through_reset();
//	bool enemy_erase(int backNum, int backCount);
//	void enemy_render();
//	void enemy_move(VECTOR2 velocity, float gravity);
//};
class Enemy
{
	GameLib::Sprite* spr;
	VECTOR2 pos;
	float gravity;
	int back_num;
	int back_count;
	bool exsit;
	bool type;
	bool move_through;
	bool moveX_through;
	bool moveY_through;
public:
	VECTOR2 velocity;
	bool onGround;
	bool onGround_through;
	Enemy(int backNum, int next_backNum, int backCount, int stageNum, int enemyNum, VECTOR2 backPos, VECTOR2 stagePos, GameLib::Sprite* sprite);
	void set_enemyPositionY(float posY);
	VECTOR2 get_enemyPosition();
	bool get_enemyExsit();
	void enemy_through_reset();
	bool enemy_erase(int backNum, int backCount);
	void enemy_render();
	void enemy_move(VECTOR2 velocity, float gravity);
};


#endif // !ENEMY_H

#ifndef STAGE_H
#define STAGE_H

#include "Player.h"
#include "Enemy.h"
#include "checkpoint.h"
#include "../GameLib/game_lib.h"

class OBJ2D;
class Enemy;

constexpr float STAGE_TEX_W = 400.0f;
constexpr float STAGE_TEX_H = 100.0f;
constexpr float JUMP_TEX_W = 100.0f;
constexpr float JUMP_TEX_H = 100.0f;
constexpr int QUANTITY = 8;

//class Enemy;
//class CheckPoint;

class Stage
{
	GameLib::Sprite* spr;
	VECTOR2 pos;
	int back_num;
	int back_count;
	int stage_num;
	bool exsit;
public:
	Stage(int backNum, int next_backNum, int backCount, int num, VECTOR2 backPos, GameLib::Sprite* spr);
	VECTOR2 stage_position();
	bool stage_exsit();
	void stage_move(VECTOR2 velocity, float gravity);
	bool stage_erase(int backNum, int backCount);
	void stage_render();
	bool stage_judge(OBJ2D* player);
	void enemy_onStage(Enemy* enemy);
	void checkPoint_through(CheckPoint* check, VECTOR2 back_pos, float player_posX);
	bool goal(VECTOR2 position);
};

class Stage_spr
{
public:
	GameLib::Sprite* spr[8];
	Stage_spr();
	void stage_load();
	void stage_data_delete();
};

class Jump_board
{
	GameLib::Sprite* spr;
	VECTOR2 pos;
public:
	bool exsit;
	Jump_board(int backNum, int next_backNum, int backCount, VECTOR2 backPos, GameLib::Sprite* sprite);
	bool player_onJumpBoard(OBJ2D* player);
	void jumpBoard_render();
	void jumpBoard_move(VECTOR2 velocity, float gravity);
};

#endif // !STAGE_H
#ifndef STAGE_H
#define STAGE_H


#include "enemy.h"
//#include "checkpoint.h"
#include "../GameLib/game_lib.h"

constexpr float STAGE_TEX_W = 400.0f;
constexpr float STAGE_TEX_H = 100.0f;



class Stage
{
	GameLib::Sprite* spr;
	VECTOR2 pos;
	int back_num;
	int back_count;
	int stage_num;
	bool exsit;
public:
	Stage(int backNum ,int next_backNum,int backCount,int num, VECTOR2 backPos,GameLib::Sprite* spr);
	VECTOR2 stage_position();
	bool stage_exsit();
	void stage_move(VECTOR2 backPos, int backNum, VECTOR2 velocity);
	bool stage_erase(int backNum, int backCount);
	void stage_render(int backNum);
	//void stage_judge(OBJ2D* player);
	//void enemy_onStage(Enemy* enemy);
	//void checkPoint_through(CheckPoint* check, VECTOR2 back_pos, float player_posX);
};

class Stage_spr
{
public:
	GameLib::Sprite* spr[8];
	Stage_spr();
	void stage_load();
	void stage_data_delete();
};

#endif // !STAGE_H
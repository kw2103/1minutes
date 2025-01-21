#include "Enemy.h"

using namespace GameLib;

float enemy_data0[20][QUANTITY][ENEMY_MAX] =
{
	{ {},{}, {}  },
	{ {},{} },
	{ {}, },
	{ {}, },
	{ {}, },
	{ {}, }
};

float enemy_data1[20][QUANTITY][ENEMY_MAX] =
{
	{ {100},{100}, {} },
	{ {},{},{300} },
	{ {}, },
	{ {}, },
	{ {}, },
	{ {}, }
};

float enemy_data2[20][QUANTITY][ENEMY_MAX] =
{
	{ {0},{}, {} },
	{ {},{} },
	{ {}, },
	{ {}, },
	{ {}, },
	{ {}, }
};

float enemy_data3[20][QUANTITY][ENEMY_MAX] =
{
	{ {},{}, {} },
	{ {},{} },
	{ {}, },
	{ {}, },
	{ {}, },
	{ {}, }
};

float enemy_data4[20][QUANTITY][ENEMY_MAX] =
{
	{ {},{},{},{300} },
	{ {},{} },
	{ {}, },
	{ {}, },
	{ {}, },
	{ {}, }
};

float enemy_data5[20][QUANTITY][ENEMY_MAX] =
{
	{ {},{}, {} },
	{ {},{} },
	{ {}, },
	{ {}, },
	{ {}, },
	{ {}, }
};

Enemy::Enemy(int backNum, int next_backNum, int backCount, int stageNum, int enemyNum, VECTOR2 backPos, VECTOR2 stagePos, Sprite* sprite) :back_num(next_backNum), back_count(backCount), exsit(true), onGround(true), gravity(0.5f)
{
	float width = 0, length = 0;

	spr = sprite;
	//stage_dataÇ©ÇÁéQè∆
	switch (next_backNum)
	{
	case 0:
		pos.x = enemy_data0[back_count][stageNum][enemyNum];
		//type= enemyType_data0[back_count][stageNum][enemyNum];
		break;
	case 1:
		pos.x = enemy_data1[back_count][stageNum][enemyNum];
		break;
	case 2:
		pos.x = enemy_data2[back_count][stageNum][enemyNum];
		break;
	case 3:
		pos.x = enemy_data3[back_count][stageNum][enemyNum];
		break;
	case 4:
		pos.x = enemy_data4[back_count][stageNum][enemyNum];
		break;
	case 5:
		pos.x = enemy_data5[back_count][stageNum][enemyNum];
		break;
	}

	type = rand() % 2;

	if (type == true)
	{
		velocity.x = 4.0f;
	}
	else
	{
		velocity.x = 2.0f;
	}

	if (pos.x == 0.0f && pos.y == 0.0f)
	{
		exsit = false;
	}

	pos += backPos;
	pos.x = stagePos.x;
	pos.y = stagePos.y;

	//length = SCREEN_H;

	//if (((backNum + 3) % 6 == next_backNum || (backNum + 3) % 6 - 1 == next_backNum || (backNum + 3) % 6 + 1 == next_backNum))	width += SCREEN_W;

	//if (backNum == 0 || backNum == 3)
	//{
	//	if ((backNum + 3) % 6 == back_num || (backNum + 3) % 6 + 1 == back_num || (backNum + 3) % 6 + 2 == back_num)
	//	{
	//		width += SCREEN_W;
	//	}
	//	;
	//}

	//if (backNum == 2 || backNum == 5)
	//{
	//	if ((backNum + 3) % 6 == back_num || (backNum + 3) % 6 - 1 == (backNum + 3) % 6 || backNum - 2 == back_num)
	//	{
	//		width += SCREEN_W;
	//	}
	//}

	//pos.x += width;

	//if (width == 0)
	//{
	//	pos.y += length*(back_num-backNum);
	//}
	//else
	//{
	//	pos.y += length * ((back_num + 3) % 6 - backNum);
	//}

}

void Enemy::set_enemyPositionY(float posY)
{
	pos.y = posY - ENEMY_TEX_H * 0.5f;
}

VECTOR2 Enemy::get_enemyPosition()
{
	return pos;
}

bool Enemy::get_enemyExsit()
{
	return exsit;
}

void Enemy::enemy_move(VECTOR2 Velocity, float Gravity)
{
	debug::setString("pos.y%f", pos.y);
	if (!move_through)
	{
		pos.x += Velocity.x;
		pos.y += Velocity.y + Gravity;
		move_through = true;
	}

	if (pos.x < SCREEN_W && (pos.y >= 0 && pos.y <= SCREEN_H))
	{
		if (!moveX_through)
		{
			pos.x -= velocity.x;
			moveX_through = true;
		}
	}

	if (!moveY_through)
	{
		if (!onGround)
		{
			velocity.y += gravity;
			pos.y += velocity.y;
			moveY_through = true;
		}

	}

	if (pos.x + ENEMY_TEX_W < 0)
	{
		exsit = false;
	}



}


void Enemy::enemy_through_reset()
{
	move_through = false;
	moveX_through = false;
	moveY_through = false;
	onGround_through = false;
}


bool Enemy::enemy_erase(int backNum, int backCount)
{
	if (backNum == 0 || backNum == 3)
	{
		return((backNum == back_num || backNum + 1 == back_num || backNum + 2 == back_num) && backCount == back_count);
	}
	if (backNum == 2 || backNum == 5)
	{
		return((backNum == back_num || backNum - 1 == back_num || backNum - 2 == back_num) && backCount == back_count);
	}
	return((backNum == back_num || backNum - 1 == back_num || backNum + 1 == back_num) && backCount == back_count);
}

void Enemy::enemy_render()
{
	sprite_render(spr, pos.x, pos.y, 1, 1, 0, 0, ENEMY_TEX_W, ENEMY_TEX_H, ENEMY_TEX_W * 0.5f, ENEMY_TEX_H * 0.5f,0, 1, 1, 1, 1);
	//primitive::rect(pos.x - ENEMY_TEX_W * 0.5f, pos.y - ENEMY_TEX_H * 0.5f, ENEMY_TEX_W, ENEMY_TEX_H);
	//primitive::circle(pos.x,pos.y, 20,1,1,0,1,0,0);
}

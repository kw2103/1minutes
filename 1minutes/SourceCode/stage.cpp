#include "stage.h"
#include "player.h"


VECTOR2 stage_data[6][20][QUANTITY] =
{
	{
		{},
		{{STAGE_TEX_W + 80,(float)SCREEN_H},{STAGE_TEX_W * 2.0f + 80,(float)SCREEN_H},{STAGE_TEX_W * 3.0f + 80,(float)SCREEN_H},{STAGE_TEX_W * 4.0f + 80,(float)SCREEN_H}},
		{},
		{},
		{},
		{ {240,500},{STAGE_TEX_W + 240,500},{STAGE_TEX_W * 2.0f + 240,500},{STAGE_TEX_W * 3.0f + 240,500},{STAGE_TEX_W * 4.0f + 240,500} },
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{ {80,530} },

	},
	{
		{ {0.0f,SCREEN_H * 0.5f},{STAGE_TEX_W,SCREEN_H * 0.5f},{STAGE_TEX_W * 2.0f,SCREEN_H * 0.5f},{STAGE_TEX_W * 3.0f,SCREEN_H * 0.5f},{STAGE_TEX_W * 4.0f,SCREEN_H * 0.5f}	},
		{ {0.0f,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 2.0f,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 3.0f,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 4.0f,SCREEN_H - STAGE_TEX_H},},
		{ { 880,0 },{ STAGE_TEX_W + 880,0 },{ STAGE_TEX_W * 2.0f + 880,0 },{ 880,SCREEN_H - STAGE_TEX_H },{ STAGE_TEX_W + 880,SCREEN_H - STAGE_TEX_H },{ STAGE_TEX_W * 2.0f + 880,SCREEN_H - STAGE_TEX_H },},
		{ {160,481},{STAGE_TEX_W + 160,481},{STAGE_TEX_W * 2.0f + 160,481}},
		{ {158,467},{SCREEN_W - STAGE_TEX_W - 160,0},{SCREEN_W - 160.0f,0} },
		{},
		{ {320,0},{STAGE_TEX_W + 320,0},{1840,SCREEN_H - STAGE_TEX_H} },
		{ {0,SCREEN_H - STAGE_TEX_H},{1120,450},{STAGE_TEX_W + 1120,450}, },
		{ {80,450},{STAGE_TEX_W,450},{STAGE_TEX_W * 2.0f,450}},
		{ {160,SCREEN_H - STAGE_TEX_H} },
		{},
		{{1760,SCREEN_H - STAGE_TEX_H}},
		{ {720,440},{STAGE_TEX_W + 720,440},{STAGE_TEX_W * 2.0f + 720,440} },
		{ {0,1},{STAGE_TEX_W,1},{STAGE_TEX_W * 2.0f,1},{STAGE_TEX_W * 3.0f,1} },
		{},
		{ {82,410},{STAGE_TEX_W + 82,410},{STAGE_TEX_W * 2.0f + 82,410} },
		{ {242,0},{STAGE_TEX_W + 242,0},{1677,450} },
		{ {960,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 960,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 2.0f + 960,SCREEN_H - STAGE_TEX_H} },
		{ {240,SCREEN_H * 0.5f},{STAGE_TEX_W + 240,SCREEN_H * 0.5f},{STAGE_TEX_W * 2.0f + 240,SCREEN_H * 0.5f},{STAGE_TEX_W * 3.0 + 240,SCREEN_H * 0.5f} },
		{ {240,SCREEN_H * 0.5f},{STAGE_TEX_W + 240,SCREEN_H * 0.5f},{STAGE_TEX_W * 2.0f + 240,SCREEN_H * 0.5f},{STAGE_TEX_W * 3.0 + 240,SCREEN_H * 0.5f},{STAGE_TEX_W * 4.0 + 240,SCREEN_H * 0.5f} }
	},
	{
		{},{},{},
		{ { SCREEN_W - 240.0f,0 } },
		{ {160,400},{STAGE_TEX_W + 160.0f,400},{STAGE_TEX_W * 2.0f + 160.0f,400},{STAGE_TEX_W * 3.0f + 160.0f,400},{STAGE_TEX_W * 4.0f + 160.0f,400} },
		{ {280,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 280.0f,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 2.0f + 280.0f,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 3.0f + 280.0f,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 4.0f + 280.0f,SCREEN_H - STAGE_TEX_H}, },
		{ {320,400},{STAGE_TEX_W + 320,400}},
		{},{},
		{ {1280,400},{STAGE_TEX_W + 1280,400} },
		{ {162,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 162,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 2.0f + 162,SCREEN_H - STAGE_TEX_H} ,{STAGE_TEX_W * 3.0f + 162,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 4.0f + 162,SCREEN_H - STAGE_TEX_H}},
		{ {320,440},{STAGE_TEX_W + 320,440} },
		{},
		{},
		{ {740,135},{STAGE_TEX_W + 740,135},{STAGE_TEX_W * 2.0f + 740,135}},
		{ {82,135},{1282,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 1282,SCREEN_H - STAGE_TEX_H} },
		{ {242,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 242,SCREEN_H - STAGE_TEX_H},{1677,440} },
		{},
		{}

	},
	{
		{ },
		{ { STAGE_TEX_W * 2.0f,SCREEN_H * 0.5f },{ STAGE_TEX_W * 3.0f,SCREEN_H * 0.5f },{ STAGE_TEX_W * 4.0f,SCREEN_H * 0.5f },{ STAGE_TEX_W * 5.0f,SCREEN_H * 0.5f } },
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{ {400,530},{STAGE_TEX_W + 400,530},{STAGE_TEX_W * 2.0 + 400,530},{STAGE_TEX_W * 3.0f + 400,530} },

	},
	{
		{ {80.0f,SCREEN_H * 0.5f},{STAGE_TEX_W + 80.0f,SCREEN_H * 0.5f},{STAGE_TEX_W * 2.0f + 80.0f,SCREEN_H * 0.5f},{STAGE_TEX_W * 3.0f + 80.0,SCREEN_H * 0.5f},	},
		{},
		{ {160,0},{STAGE_TEX_W + 160,0},{160,SCREEN_H - STAGE_TEX_H},{SCREEN_W - STAGE_TEX_W - 240.0f,481},{SCREEN_W - 240.0f,481} },
		{ {878,467},{STAGE_TEX_W + 878.0f,467},{STAGE_TEX_W * 2.0f + 878.0f,467} },
		{ {240,0},{STAGE_TEX_W + 240.0f,0},{STAGE_TEX_W * 2.0f + 240.0f,0} },
		{ {1040,0},{STAGE_TEX_W + 1040,0},{STAGE_TEX_W * 2.0f + 1040,0} },
		{ {320,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 320,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 2.0f + 320,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 3.0f + 320,SCREEN_H - STAGE_TEX_H} },
		{ {0,450},{STAGE_TEX_W,450},{STAGE_TEX_W * 2.0f,450},{STAGE_TEX_W * 3.0f,450},{STAGE_TEX_W * 4.0f,450} },
		{ {80,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 80,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 2.0f + 80,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 3.0f + 80,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 4.0f + 80,SCREEN_H - STAGE_TEX_H} },
		{},
		{},
		{ {240,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 240,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 2.0f + 240,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 3.0f + 240,SCREEN_H - STAGE_TEX_H}},
		{ {0,440},{STAGE_TEX_W,440},{1520,1} },
		{},
		{ {402,410},{STAGE_TEX_W + 402,410},{STAGE_TEX_W * 2.0f + 402,410},{STAGE_TEX_W * 3.0f + 402,410} },
		{ {162,0},{STAGE_TEX_W + 162,0},{STAGE_TEX_W * 2.0f + 162,0},{STAGE_TEX_W * 3.0f + 162,0},{STAGE_TEX_W * 4.0f + 162,0} },
		{ {157,450},{STAGE_TEX_W + 157,450},{STAGE_TEX_W * 2.0f + 157,450},{STAGE_TEX_W * 3.0f + 157,450},{STAGE_TEX_W * 4.0f + 157,450} },
		{ {240,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 240,SCREEN_H - STAGE_TEX_H},{1760,SCREEN_H * 0.5f} },
		{ {960,SCREEN_H * 0.5f},{STAGE_TEX_W + 960,SCREEN_H * 0.5f},{STAGE_TEX_W * 2.0f + 960,SCREEN_H * 0.5f} },
		{ {318,SCREEN_H * 0.5f},{STAGE_TEX_W + 318,SCREEN_H * 0.5f},{STAGE_TEX_W * 2.0f + 318,SCREEN_H * 0.5f},{STAGE_TEX_W * 3.0f + 318,SCREEN_H * 0.5f},{STAGE_TEX_W * 4.0f + 318,SCREEN_H * 0.5f},{STAGE_TEX_W * 5.0f + 318,SCREEN_H * 0.5f},{STAGE_TEX_W * 6.0f + 318,SCREEN_H * 0.5f},{STAGE_TEX_W * 7.0f + 318,SCREEN_H * 0.5f} }

	},
	{
		{},
		{ {400,SCREEN_H * 0.45f},{STAGE_TEX_W + 400,SCREEN_H * 0.45f} ,{ STAGE_TEX_W * 2.0f + 400,SCREEN_H * 0.45f } ,{ STAGE_TEX_W * 3.0f + 400,SCREEN_H * 0.45f }},
		{},
		{ {160,0},{STAGE_TEX_W + 160.0f,0},{STAGE_TEX_W * 2.0f + 160.0f,0},{STAGE_TEX_W * 3.0f + 160.0f,0} },
		{ {240,400},{STAGE_TEX_W + 240,400},{STAGE_TEX_W * 2.0f + 240,400},{SCREEN_W - 120.0f,SCREEN_H - STAGE_TEX_H} },
		{ {1040,400},{STAGE_TEX_W + 1040,400},{STAGE_TEX_W * 2.0f + 1040, 400} },
		{},{},{},
		{ {160,400},{STAGE_TEX_W + 160,400},{STAGE_TEX_W * 2.0f + 160,400} },
		{ {1040,440},{STAGE_TEX_W + 1040,440},{STAGE_TEX_W * 2.0f + 1040,440} },
		{},//12
		{},//13
		{},//14
		{{20,135},{STAGE_TEX_W + 20,135},{STAGE_TEX_W * 2.0f + 20,135},{STAGE_TEX_W * 3.0f + 20,135},{STAGE_TEX_W * 4.0f + 20,135}},
		{ {162,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W + 162,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 2.0f + 162,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 3.0f + 162,SCREEN_H - STAGE_TEX_H},{STAGE_TEX_W * 4.0f + 162,SCREEN_H - STAGE_TEX_H} },
		{ {157,440},{STAGE_TEX_W + 157, 440},{STAGE_TEX_W * 2.0f + 157, 440},{STAGE_TEX_W * 3.0f + 157, 440},{STAGE_TEX_W * 4.0f + 157, 440} },
		{},//18
		{},//19
		{}//20
	}
};

VECTOR2 jump_board_data[6][20] =
{
	{},
	{ {},{},{},{1360,481},{558,467},{},{}, { 400,SCREEN_H - STAGE_TEX_H },{}, {},{},{},{},{1600,0},{},{1282,410},{},{},{1840,SCREEN_H * 0.5f} },
	{ {},{},{80,SCREEN_H * 0.45f},{},{},{},{1120,400},{},{},{},{},{1120,440},{},{},{},{},{1040,SCREEN_H - STAGE_TEX_H},{237,440}},
	{},
	{ {STAGE_TEX_W * 4.0f + 80.0,SCREEN_H * 0.5f},{160,0},{560,SCREEN_H - STAGE_TEX_H},{},{1440,0},{800,440},{},{},{},{},{},{1840,SCREEN_H - STAGE_TEX_H},{800,440},{},{},{},{},{1040,SCREEN_H - STAGE_TEX_H},{},{} },
	{ {},{},{},{},{},{360,SCREEN_H - STAGE_TEX_H},{},{},{},{},{242,SCREEN_H - STAGE_TEX_H},{},{}}
};

Stage::Stage(int backNum, int next_backNum, int backCount, int num, VECTOR2 backPos, GameLib::Sprite* sprite) :back_num(next_backNum), back_count(backCount), exsit(true), stage_num(num)
{
	float width = 0, length = 0;

	spr = sprite;

	//stage_dataÇ©ÇÁéQè∆
	pos = stage_data[back_num][back_count][stage_num];

	if (pos.x == 0.0f && pos.y == 0.0f)
	{
		exsit = false;
	}

	pos += backPos;

	length = SCREEN_H;

	if (((backNum + 3) % 6 == next_backNum || (backNum + 3) % 6 - 1 == next_backNum || (backNum + 3) % 6 + 1 == next_backNum))	width += SCREEN_W;

	if ((backNum == 0 || backNum == 3) && width == 0)
	{
		if ((backNum + 3) % 6 == back_num || (backNum + 3) % 6 + 1 == back_num || (backNum + 3) % 6 + 2 == back_num)
		{
			width += SCREEN_W;
		}
		;
	}

	if ((backNum == 2 || backNum == 5) && width == 0)
	{
		if ((backNum + 3) % 6 == back_num || (backNum + 3) % 6 - 1 == (backNum + 3) % 6 || backNum - 2 == back_num)
		{
			width += SCREEN_W;
		}
	}

	pos.x += width;

	if (width == 0)
	{
		pos.y += length * (back_num - backNum);
	}
	else
	{
		pos.y += length * ((back_num + 3) % 6 - backNum);
	}

}


VECTOR2 Stage::stage_position()
{
	return pos;
}

bool Stage::stage_exsit()
{
	return exsit;
}

void Stage::stage_move(VECTOR2 velocity, float gravity)
{
	pos.x += velocity.x;
	pos.y += velocity.y + gravity;
}

bool Stage::stage_erase(int backNum, int backCount)
{
	return (pos.x + STAGE_TEX_W < 0);
}

void Stage::stage_render()
{
	sprite_render(spr, pos.x, pos.y, 1, 1, 0, 0, STAGE_TEX_W, STAGE_TEX_H, 0, 0, 0, 1, 1, 1, 1);
	GameLib::primitive::rect(pos.x, pos.y, STAGE_TEX_W, STAGE_TEX_H, 0, 0, 0, 0, 0, 0, 0.5f);
	GameLib::primitive::rect(pos.x, pos.y, STAGE_TEX_W - 1, STAGE_TEX_H - 1, 0, 0, 0, 1, 1, 1, 0.5f);
}

bool Stage::stage_judge(OBJ2D* player)
{
	if (player->pos.x - player->texSize.x * 0.25 > pos.x + STAGE_TEX_W)	return false;
	if (player->pos.x + player->texSize.x * 0.25 < pos.x)	return false;
	if (player->pos.y - player->texSize.y > pos.y + STAGE_TEX_H)	return false;
	if (player->pos.y < pos.y)	return false;

	return true;
}

//void Stage::enemy_onStage(Enemy* enemy)
//{
//	VECTOR2 enemyPos = enemy->get_enemyPosition();
//	if (!enemy->onGround_through)
//	{
//		if (enemyPos.y - ENEMY_TEX_H * 0.5f <= pos.y + STAGE_TEX_H && enemyPos.y + ENEMY_TEX_H * 0.5f >= pos.y &&
//			enemyPos.x + ENEMY_TEX_W * 0.5f > pos.x && enemyPos.x - ENEMY_TEX_W * 0.5f < pos.x + STAGE_TEX_W) {
//			enemy->onGround = true;
//			enemy->set_enemyPositionY(pos.y);
//			enemy->onGround_through = true;
//		}
//		else
//		{
//			enemy->onGround = false;
//		}
//	}
//}

void Stage::checkPoint_through(CheckPoint* check, VECTOR2 back_pos, float player_posX)
{
	switch (back_count)
	{
	case 0:
		if (back_num == 1 && stage_num == 0 && player_posX > pos.x && !check->through[0])
		{
			check->backNum = back_num;
			check->backPos = back_pos;
			check->backCount = back_count;
			check->through[0] = true;
		}
		break;
	case 5:
		if (back_num == 2 && stage_num == 0 && player_posX > pos.x && !check->through[1])
		{
			check->backNum = back_num;
			check->backPos = back_pos;
			check->backCount = back_count;
			check->through[1] = true;
		}
		break;
	case 10:
		if (back_num == 2 && stage_num == 0 && player_posX > pos.x && !check->through[2])
		{
			check->backNum = back_num;
			check->backPos = back_pos;
			check->backCount = back_count;
			check->through[2] = true;
		}
		break;
	case 15:
		if (back_num == 1 && stage_num == 0 && player_posX > pos.x && !check->through[3])
		{
			check->backNum = back_num;
			check->backPos = back_pos;
			check->backCount = back_count;
			check->through[3] = true;
		}
		break;
	}
}

bool Stage::goal(VECTOR2 position)
{
	return(back_num == 4 && back_count == 19 && stage_num == 0 && position.x < pos.x);
}



Stage_spr::Stage_spr()
{
	for (auto& sprData : spr)
	{
		sprData = nullptr;
	}
}

void Stage_spr::stage_load()
{
	spr[0] = GameLib::sprite_load(L"./Data/Images/road0.png");
	spr[1] = GameLib::sprite_load(L"./Data/Images/road1.png");
	spr[2] = GameLib::sprite_load(L"./Data/Images/road2.png");
	spr[3] = GameLib::sprite_load(L"./Data/Images/road3.png");
	spr[4] = GameLib::sprite_load(L"./Data/Images/road4.png");
	spr[5] = GameLib::sprite_load(L"./Data/Images/road5.png");
	spr[6] = GameLib::sprite_load(L"./Data/Images/road6.png");
	spr[7] = GameLib::sprite_load(L"./Data/Images/road7.png");
}

void Stage_spr::stage_data_delete()
{
	for (auto& sprData : spr)
	{
		safe_delete(sprData);
	}
}

Jump_board::Jump_board(int backNum, int next_backNum, int backCount, VECTOR2 backPos, GameLib::Sprite* sprite) :exsit(true)
{
	float width = 0, length = 0;

	spr = sprite;

	//stage_dataÇ©ÇÁéQè∆
	pos = jump_board_data[next_backNum][backCount];

	if (pos.x == 0.0f && pos.y == 0.0f)
	{
		exsit = false;
	}

	pos += backPos;

	length = SCREEN_H;

	if (((backNum + 3) % 6 == next_backNum || (backNum + 3) % 6 - 1 == next_backNum || (backNum + 3) % 6 + 1 == next_backNum))	width += SCREEN_W;

	if ((backNum == 0 || backNum == 3) && width == 0)
	{
		if ((backNum + 3) % 6 == next_backNum || (backNum + 3) % 6 + 1 == next_backNum || (backNum + 3) % 6 + 2 == next_backNum)
		{
			width += SCREEN_W;
		}
		;
	}

	if ((backNum == 2 || backNum == 5) && width == 0)
	{
		if ((backNum + 3) % 6 == next_backNum || (backNum + 3) % 6 - 1 == (backNum + 3) % 6 || backNum - 2 == next_backNum)
		{
			width += SCREEN_W;
		}
	}

	pos.x += width;

	if (width == 0)
	{
		pos.y += length * (next_backNum - backNum);
	}
	else
	{
		pos.y += length * ((next_backNum + 3) % 6 - backNum);
	}


}

void Jump_board::jumpBoard_move(VECTOR2 velocity, float gravity)
{
	pos.x += velocity.x;
	pos.y += velocity.y + gravity;
}

void Jump_board::jumpBoard_render()
{
	GameLib::primitive::rect(pos.x, pos.y, JUMP_TEX_W, JUMP_TEX_H, 0, 0, 0, 1, 1, 1, 0.5f);
}

bool Jump_board::player_onJumpBoard(OBJ2D* player)
{
	if (player->pos.x - player->texSize.x * 0.25 > pos.x + JUMP_TEX_W)	return false;
	if (player->pos.x + player->texSize.x * 0.25 < pos.x)	return false;
	if (player->pos.y - player->texSize.y > pos.y + JUMP_TEX_H)	return false;
	if (player->pos.y < pos.y)	return false;

	return true;
}

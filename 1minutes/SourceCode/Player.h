#pragma once

#include "../GameLib/vector.h"


#define PLAYER_TEX_W        (256.0f)   // �v���C���[�̉摜1�̕�
#define PLAYER_TEX_H        (256.0f)   // �v���C���[�̉摜1�̍���
#define PLAYER_PIVOT_X      (PLAYER_TEX_W/2)    // �v���C���[�̉������̒��S
#define PLAYER_PIVOT_Y      (PLAYER_TEX_H/2)   // �v���C���[�̏c�����̉��[



class PLAYER {
public:
	float angle;
	VECTOR2 speed;
	VECTOR2 position;
	VECTOR2 scale;
	VECTOR2 texPos;
	VECTOR2 texSize;
	VECTOR2 pivot;
	VECTOR4 color;

	int state;
	float radius;
	VECTOR2 offset;


	
};



void player_init();
void player_deinit();
void player_update();
void player_render();
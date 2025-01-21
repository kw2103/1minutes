#include "All.h"


using namespace GameLib::input;
using namespace GameLib;
using namespace std;

int shougai;//��


int nitoro;
int nitoroCount;


float speedX;
float speedY;

//�e�e
int bullet;


//�O������
int grenade;
int grenadeCount;

float GX;
float GY;
float GSX;
float GSY;
float GTX;
float GTY;
float GTW;
float GTH;
float GCX;
float GCY;
float G_angle;

OBJ2D player;
OBJ2D bullets[100];

Sprite* sprPlayer;
Sprite* sprGrenade;
Sprite* sprPlayerGun;
Sprite* sprBullet;

void player_init()
{
	player.state = 0;
	player.speed = { speedX,speedY };

	nitoro = 2;
	nitoroCount = 2;
	

	grenade = 3;
	grenadeCount = 3;

}

void player_deinit()
{
	player.state = 0;
	safe_delete(sprPlayer);
}

void player_update()
{
	switch (player.state)
	{
	case 0:
		sprPlayer    = sprite_load(L"./Data/Images/player.png");
		sprGrenade   = sprite_load(L"./Data/Images/Grenade.png");
		sprPlayerGun = sprite_load(L"./Data/Images/Gun.png");
		sprBullet    = sprite_load(L"./Data/Images/Bullet.png");
		++player.state;

		for (int i = 0; i < 100; i++)
		{
			bullets[i].state = 0;
			bullets[i].pos = { 0, 0 };
			bullets[i].velocity = { 8, 0 };
			bullets[i].scale = { 0.5, 0.5 };
			bullets[i].texPos = { 232, 226 };
			bullets[i].texSize = { 57, 25 };
			bullets[i].pivot = { 57 / 2, 25 / 2 };
			bullets[i].angle = 0 * 0.01745;
			bullets[i].color = { 1.0, 1.0, 1.0, 1.0 };
		}
		//player
		
		player.scale = { 1.5f, 1.5f };
		player.texPos = { 0, 0 };
		player.texSize = { PLAYER_TEX_W, PLAYER_TEX_H };
		player.pivot = { PLAYER_PIVOT_X, PLAYER_PIVOT_Y };

	case 1:
		++player.state;
	case 2:
		//�ʏ펞
		
		//�v���C���[�̃X�s�[�h�����X�ɏオ��
		speedX++;

		//�v���C���[����Q���ɓ����������A��������
		speedX = 1.0f;

		//�e����
		for (int i = 0; i < 100; i++)
		{
			if (bullets[i].state == 0 && STATE(0) & PAD_TRG4)
			{
				bullets[i].pos.x = player.pos.x;
				bullets[i].pos.y = player.pos.y;
				bullets[i].state = 1;
				break;
			}
			bullets[i].pos += bullets[i].velocity;
		}
		
		//�e�e���G���Q���ɓ���������

		
		//�X�y�[�X�L�[�Ńj�g�����g��
		if (TRG(0) & PAD_TRG4)
		{
			speedX += 2.0f;
			nitoroCount -= 1;
		}
		//�E�N���b�N�ŃO���������g��
		if (TRG(0) & PAD_R1)
		{
			GX++;
			if (GX < 5.0f)
			{

				grenadeCount -= 1;
			}
		}
		
		++player.state;
		
	}


}

void player_render()
{
	sprite_render(
		sprPlayer,
		player.position.x, player.position.y,
		player.scale.x, player.scale.y,
		player.texPos.x, player.texPos.y,
		player.texSize.x, player.texSize.y,
		player.pivot.x, player.pivot.y,
		player.angle,
		player.color.x, player.color.y, player.color.z, player.color.w
	);

	//���N���b�N���������Ă�Ƃ��ɒe�ۂ�`�悳����

	for (int i = 0; i < 100; i++)
	{
		if (bullets[i].state != 0)
		{
			sprite_render(sprBullet, bullets[i].pos.x, bullets[i].pos.y, bullets[i].scale.x/*sx*/, bullets[i].scale.y/*sy*/, bullets[i].texPos.x/*tx*/, bullets[i].texPos.y/*ty*/, bullets[i].texSize.x/*tw*/, bullets[i].texSize.y/*th*/, bullets[i].pivot.x/*cx*/, bullets[i].pivot.y/*cy*/, bullets[i].angle/*angle*/, bullets[i].color.x, bullets[i].color.y, bullets[i].color.z, bullets[i].color.w);
		}
	}
	//�E�N���b�N���������Ƃ��ɃO��������`�ʂ�����

	if (TRG(0) & PAD_R1)
	{
		sprite_render(sprGrenade,
			GX,GY,
			GTX,GTY,
			GTW,GTH,
			GCX,GCY,
			1.0f,
			1.0f,1.0f,1.0f,1.0f);
	}
	//�@�e�̕`�ʁi�}�E�X�J�[�\����ǂ킹��j

}
float(* const ToRadian)(float) = DirectX::XMConvertToRadians;  // �p�x�����W�A����
float(* const ToDegree)(float) = DirectX::XMConvertToDegrees;  // ���W�A�����p�x��

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

float BX;
float BY;
float BSX;
float BSY;
float BTX;
float BTY;
float BTW;
float BTH;
float BCX;
float BCY;
float B_angle;

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

PLAYER player;

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
	case 1:
		//player
		player.angle = ToRadian(0);
		player.scale = { 1.5f, 1.5f };
		player.texPos = { 0, 0 };
		player.texSize = { PLAYER_TEX_W, PLAYER_TEX_H };
		player.pivot = { PLAYER_PIVOT_X, PLAYER_PIVOT_Y };
		++player.state;

		//
	case 2:
		//�ʏ펞
		//�v���C���[�̃X�s�[�h�����X�ɏオ��
		speedX++;

		//�v���C���[����Q���ɓ����������A��������
		speedX = 1.0f;

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

	if (STATE(0) & PAD_L1)
	{
		sprite_render(sprBullet, 
			BX, BY, 
			player.position.x, player.position.y, 
			BTX, BTY, 
			BTW, BTH, 
			BCX, BCY, 
			B_angle, 
			1.0f, 1.0f, 1.0f, 1.0f);
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

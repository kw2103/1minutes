#include "All.h"


using namespace GameLib::input;
using namespace GameLib;
using namespace std;

int shougai;//��

int bullet;

int nitoro;
int nitoroCount;
float nitoroSpeed;

int grenade;
int grenadeCount;

float speedX;
float speedY;

PLAYER player;

Sprite* sprPlayer;
Sprite* sprGrenade;
Sprite* sprGun;
Sprite* sprBullet;

void player_init()
{
	player.player_state = 0;
	player.speed = { speedX,speedY };

	nitoro = 2;
	nitoroCount = 2;
	nitoroSpeed = 0.0f;

	grenade = 3;
	grenadeCount = 3;

}

void player_deinit()
{
	safe_delete(sprPlayer);
}

void player_update()
{
	switch (player.player_state)
	{
	case 0:
		sprPlayer  = sprite_load(L"./Data/Images/player.png");
		sprGrenade = sprite_load(L"./Data/Images/Grenade.png");
		sprGun     = sprite_load(L"./Data/Images/Gun.png");
		sprBullet  = sprite_load(L"./Data/Images/Bullet.png");
		++player.player_state;
	case 1:
		player.scale = { 1.5f, 1.5f };
		player.texPos = { 0, 0 };
		player.texSize = { PLAYER_TEX_W, PLAYER_TEX_H };
		player.pivot = { PLAYER_PIVOT_X, PLAYER_PIVOT_Y };
		++player.player_state;
	case 2:
		//�ʏ펞
		//�v���C���[�̃X�s�[�h�����X�ɏオ��
		speedX++;
		//�v���C���[����Q���ɓ����������A��������
		
		//�X�y�[�X�L�[�Ńj�g�����g��
		if (TRG(0) & PAD_TRG4)
		{

		}
		//�E�N���b�N�ŃO���������g��
		if (TRG(0) & PAD_R1)
		{

		}
		//�v���C���[�ɑ��b���������̏���

		++player.player_state;
		
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

	//�E�N���b�N���������Ƃ��ɃO��������`�ʂ�����

	//�@�e�̕`�ʁi�}�E�X�J�[�\����ǂ킹��j

}

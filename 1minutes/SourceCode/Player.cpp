#include "All.h"


using namespace GameLib::input;
using namespace GameLib;
using namespace std;

int shougai;//仮

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
		//通常時
		//プレイヤーのスピードが徐々に上がる
		speedX++;
		//プレイヤーが障害物に当たった時、減速する
		
		//スペースキーでニトロを使う
		if (TRG(0) & PAD_TRG4)
		{

		}
		//右クリックでグレランを使う
		if (TRG(0) & PAD_R1)
		{

		}
		//プレイヤーに装甲がついた時の処理

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
	//左クリック長押ししてるときに弾丸を描画させる

	//右クリックを押したときにグレランを描写させる

	//機銃の描写（マウスカーソルを追わせる）

}

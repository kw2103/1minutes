#include "All.h"


using namespace GameLib;
using namespace std;

int bullet;

int nitoro;
int nitoroCount;
int nitoroSpeed;

int grenade;
int grenadeCount;

PLAYER player;

Sprite* sprPlayer;
Sprite* sprFinish;

void player_init()
{
	player.player_state = 0;
	player.speed = { 1.0f,0.0f };

	nitoro = 2;
	nitoroCount = 0.0f;
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
		sprPlayer = sprite_load(L"./Data/Images/player.png");
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

		//プレイヤーが障害物に当たった時、減速する

		//スペースキーでニトロを使う

		//右クリックでグレランを使う

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
}

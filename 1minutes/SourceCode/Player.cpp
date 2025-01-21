#include "All.h"


using namespace GameLib::input;
using namespace GameLib;
using namespace std;

int shougai;//仮


int nitoro;
int nitoroCount;


float speedX;
float speedY;

//銃弾
int bullet;


//グレラン
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
		//通常時
		
		//プレイヤーのスピードが徐々に上がる
		speedX++;

		//プレイヤーが障害物に当たった時、減速する
		speedX = 1.0f;

		//弾発射
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
		
		//銃弾が敵や障害物に当たった時

		
		//スペースキーでニトロを使う
		if (TRG(0) & PAD_TRG4)
		{
			speedX += 2.0f;
			nitoroCount -= 1;
		}
		//右クリックでグレランを使う
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

	//左クリック長押ししてるときに弾丸を描画させる

	for (int i = 0; i < 100; i++)
	{
		if (bullets[i].state != 0)
		{
			sprite_render(sprBullet, bullets[i].pos.x, bullets[i].pos.y, bullets[i].scale.x/*sx*/, bullets[i].scale.y/*sy*/, bullets[i].texPos.x/*tx*/, bullets[i].texPos.y/*ty*/, bullets[i].texSize.x/*tw*/, bullets[i].texSize.y/*th*/, bullets[i].pivot.x/*cx*/, bullets[i].pivot.y/*cy*/, bullets[i].angle/*angle*/, bullets[i].color.x, bullets[i].color.y, bullets[i].color.z, bullets[i].color.w);
		}
	}
	//右クリックを押したときにグレランを描写させる

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
	//機銃の描写（マウスカーソルを追わせる）

}
float(* const ToRadian)(float) = DirectX::XMConvertToRadians;  // 角度をラジアンに
float(* const ToDegree)(float) = DirectX::XMConvertToDegrees;  // ラジアンを角度に

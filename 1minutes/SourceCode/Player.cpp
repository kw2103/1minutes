#include "All.h"


using namespace GameLib::input;
using namespace GameLib;
using namespace std;

int shougai;//仮


int nitoro;
int nitoroCount;

int grenade;
int grenadeCount;

float speedX;
float speedY;

//銃弾
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

PLAYER player;

Sprite* sprPlayer;
Sprite* sprGrenade;
Sprite* sprGun;
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
		sprPlayer  = sprite_load(L"./Data/Images/player.png");
		sprGrenade = sprite_load(L"./Data/Images/Grenade.png");
		sprGun     = sprite_load(L"./Data/Images/Gun.png");
		sprBullet  = sprite_load(L"./Data/Images/Bullet.png");
		++player.state;
	case 1:
		player.angle = ToRadian(0);
		player.scale = { 1.5f, 1.5f };
		player.texPos = { 0, 0 };
		player.texSize = { PLAYER_TEX_W, PLAYER_TEX_H };
		player.pivot = { PLAYER_PIVOT_X, PLAYER_PIVOT_Y };
		++player.state;
	case 2:
		//通常時
		//プレイヤーのスピードが徐々に上がる
		speedX++;
		//プレイヤーが障害物に当たった時、減速する
		speedX = 1.0f;
		//スペースキーでニトロを使う
		if (TRG(0) & PAD_TRG4)
		{
			speedX += 2.0f;
			nitoroCount -= 1;
		}
		//右クリックでグレランを使う
		if (TRG(0) & PAD_R1)
		{

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
	if (TRG(0) & PAD_L1)
	{
		sprite_render(sprBullet, BX, BY, BSX, BSY, BTX, BTY, BTW, BTH, BCX, BCY, B_angle, 1.0f, 1.0f, 1.0f, 1.0f);;
	}
	//右クリックを押したときにグレランを描写させる
	if (TRG(0) & PAD_R1)
	{
		sprite_render(sprGrenade,);
	}
	//機銃の描写（マウスカーソルを追わせる）

}
float(* const ToRadian)(float) = DirectX::XMConvertToRadians;  // 角度をラジアンに
float(* const ToDegree)(float) = DirectX::XMConvertToDegrees;  // ラジアンを角度に

#include "All.h"

int title_state;
int title_timer;
extern int nextScene;

GameLib::Sprite* sprCar;

//--------------------------------------
//  初期設定
//--------------------------------------
void title_init()
{
    title_state = 0;
    title_timer = 0;
}

//--------------------------------------
//  終了処理
//--------------------------------------
void title_deinit()
{
    //music::stop(0);

    safe_delete(sprCar);
}

//--------------------------------------
//  タイトルの更新処理
//--------------------------------------
void title_update()
{
    switch (title_state)
    {
    case 0:
        //////// 初期設定 ////////

        sprCar = GameLib::sprite_load(L"./Data/Images/right.png");

        title_state++;
        /*fallthrough*/

    case 1:
        //////// パラメータの設定 ////////

        GameLib::setBlendMode(GameLib::Blender::BS_ALPHA);

       /* music::play(0);
        music::setVolume(0, 0.1f);*/

        title_state++;
        /*fallthrough*/

    case 2:
        //////// 通常時 ////////

        if (GameLib::input::TRG(0) & GameLib::input::PAD_START)
        {
            //sound::play(XWB_SYSTEM, XWB_SYSTEM_START);

            nextScene = SCENE_GAME;
            break;
        }

        break;
    }

    GameLib::debug::setString("");
    GameLib::debug::setString("title_state:%d", title_state);
    GameLib::debug::setString("title_timer:%d", title_timer);

    title_timer++;
}

//--------------------------------------
//  タイトルの描画処理
//--------------------------------------
void title_render()
{
    // 画面を青で塗りつぶす
    GameLib::clear(0.3f, 0.5f, 1.0f);

    // タイトルの文字
    GameLib::font::textOut(4, "ECC COMP", VECTOR2(100, 80), VECTOR2(2.4f, 2.4f), VECTOR4(1, 0.8f, 0, 1));
    GameLib::font::textOut(4, "Game Programming I", VECTOR2(80, 180), VECTOR2(2.0f, 2.0f), VECTOR4(0, 1, 0.6f, 1));

    // "Push Enter Key" 点滅
    if (title_timer >> 5 & 0x01)
    {
        GameLib::font::textOut(4, "Push Enter Key", VECTOR2(120, 560), VECTOR2(1.4f, 1.4f));
    }

    sprite_render(sprCar, 200, 200);
}

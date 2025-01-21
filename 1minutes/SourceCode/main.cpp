//******************************************************************************
//
//
//      main
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include "Main.h"

//------< 変数 >----------------------------------------------------------------
int curScene = SCENE_NONE;
int nextScene = SCENE_TITLE;

//--------------------------------------
//  WinMain（エントリポイント）
//--------------------------------------
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    GameLib::init(L"ゲームプログラミングⅠ", SCREEN_W, SCREEN_H);

    // オーディオの初期設定
    //audio_init();

    while (GameLib::gameLoop())
    {
        // シーン切り替え処理
        if (curScene != nextScene)
        {
            // 現在のシーンに応じた終了処理
            switch (curScene)
            {
            case SCENE_TITLE:
                title_deinit();
                break;

            case SCENE_GAME:
                game_deinit();
                break;
            }

            // 次のシーンに応じた初期設定処理
            switch (nextScene)
            {
            case SCENE_TITLE:
                title_init();
                break;

            case SCENE_GAME:
                game_init();
                break;
            }

            curScene = nextScene;
        }

        // 入力を更新する
        GameLib::input::update();

        // 音楽の更新処理
        GameLib::music::update();

        // 現在のシーンに応じた更新・描画処理
        switch (curScene)
        {
        case SCENE_TITLE:
            title_update();
            title_render();
            break;

        case SCENE_GAME:
            game_update();
            game_render();
            break;
        }

        // デバッグ文字列の描画
        GameLib::debug::display(1.0f, 1.0f, 1.0f, 1, 1);

        //debug::setString("GP1_09 SAMPLE");

        // バックバッファの内容を表示
        GameLib::present(1, 0);
    }

    // 現在のシーンに応じた終了処理
    switch (curScene)
    {
    case SCENE_TITLE:
        title_deinit();
        break;

    case SCENE_GAME:
        game_deinit();
        break;
    }

    // オーディオの終了処理
    //audio_deinit();

    // ゲームライブラリの終了処理
    GameLib::uninit();

    return 0;
}
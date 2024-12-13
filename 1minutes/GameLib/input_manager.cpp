//******************************************************************************
//
//
//      入力マネージャ
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------

#include "input_manager.h"
#include "./DirectXTK-master/Inc/Keyboard.h"
#include "./DirectXTK-master/Inc/GamePad.h"
#include "game_lib.h"

namespace GameLib
{
    using namespace input;
    using DirectX::Keyboard;

    //------< データ >-----------------------------------------------------------

    // キー割り当てデータ
    PadAssign keyAssign00[] = {
        { PAD_UP        , Keyboard::Up },           // 'VK_UP'
        { PAD_DOWN      , Keyboard::Down },         // 'VK_DOWN'
        { PAD_LEFT      , Keyboard::Left },         // 'VK_LEFT'
        { PAD_RIGHT     , Keyboard::Right },        // 'VK_RIGHT'

        { PAD_START     , Keyboard::Space },        // VK_SPACE
        { PAD_SELECT    , Keyboard::Back },         // VK_BACK
        { PAD_TRG1      , Keyboard::Z },            // 'Z'
        { PAD_TRG2      , Keyboard::X },            // 'X'
        { PAD_TRG3      , Keyboard::C },            // 'C'

        { PAD_UP        , Keyboard::W },            // 'W'
        { PAD_DOWN      , Keyboard::S },            // 'S'
        { PAD_LEFT      , Keyboard::A },            // 'A'
        { PAD_RIGHT     , Keyboard::D },            // 'D'
        { PAD_START     , Keyboard::Enter },        // VK_RETURN
        { PAD_TRG2      , Keyboard::N },            // 'N'
        { PAD_TRG3      , Keyboard::M },            // 'M'

        //{ PAD_START    , VK_LBUTTON },              // マウス左ボタン
        //{ PAD_TRG4     , VK_RBUTTON },              // マウス右ボタン
        //{ PAD_R3       , VK_MBUTTON },              // マウス中ボタン

        { 0x00, 0x00 }                              // 終了コード
    };

    // ジョイスティック割り当てデータ
    PadAssign joyAssign00[] = {
        { PAD_UP        , GamePad::UP },            // 上キー
        { PAD_DOWN      , GamePad::DOWN },          // 下キー
        { PAD_LEFT    	, GamePad::LEFT },          // 左キー
        { PAD_RIGHT     , GamePad::RIGHT },         // 右キー

        { PAD_START     , GamePad::START },         // スタートボタン
        { PAD_TRG1      , GamePad::A },             // Aボタン
        { PAD_TRG2      , GamePad::B },             // Bボタン
        { PAD_TRG3      , GamePad::X },             // Xボタン
        { PAD_TRG4      , GamePad::Y },             // Yボタン
        { PAD_L1        , GamePad::LSHOULDER },     // Lボタン
        { PAD_R1        , GamePad::RSHOULDER },     // Rボタン
        { PAD_L3        , GamePad::LSTICK },        // Lスティック
        { PAD_R3        , GamePad::RSTICK },        // Rスティック

        { PAD_SELECT    , GamePad::BACK },          // バック（セレクト）ボタン

        { 0x00, 0x00 }                              // 終了コード
    };

    // キー割り当てデータ
    PadAssign keyAssign01[] = {
        //{ PAD_UP       , Keyboard::Up },            // VK_UP
        //{ PAD_DOWN     , Keyboard::Down },          // VK_DOWN
        //{ PAD_LEFT     , Keyboard::Left },          // VK_LEFT
        //{ PAD_RIGHT    , Keyboard::Right },         // VK_RIGHT

        //{ PAD_UP        , Keyboard::W },            // VK_W
        //{ PAD_DOWN      , Keyboard::S },            // VK_S
        //{ PAD_LEFT      , Keyboard::A },            // VK_A
        //{ PAD_RIGHT     , Keyboard::D },            // VK_D

        { PAD_TRG1      , Keyboard::J },            // 'J'
        { PAD_TRG2      , Keyboard::K },            // 'K'
        { PAD_TRG3      , Keyboard::L },            // 'L'

        { PAD_SELECT    , Keyboard::F1 },           // VK_F1

//      { PAD_START     , VK_LBUTTON },             // マウス左ボタン
//      { PAD_SELECT    , VK_RBUTTON },             // マウス右ボタン
//      { PAD_TRG1      , VK_MBUTTON },             // マウス中ボタン

        { 0x00, 0x00 }                              // 終了コード
    };

    //--------------------------------------------------------------
    //  キー割り当ての設定
    //--------------------------------------------------------------
    void InputManager::setKeyAssign(int no, PadAssign *data)
    {
        pad[no].keyAssign = data;
    }

    //--------------------------------------------------------------
    //  ジョイスティック割り当ての設定
    //--------------------------------------------------------------
    void InputManager::setJoyAssign(int no, PadAssign *data)
    {
        pad[no].joyAssign = data;
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    InputManager::~InputManager()
    {
        safe_delete(gamePad);
        safe_delete(keyboard);
    }

    //--------------------------------------------------------------
    //  初期化
    //--------------------------------------------------------------
    void InputManager::init()
    {
        keyboard = new DirectX::Keyboard;
        gamePad = new DirectX::GamePad;

        // padをゼロクリアしておく
        SecureZeroMemory(pad, sizeof(PadState) * input::GAMEPAD_NUM);

        // コントローラ0の設定
        setKeyAssign(0, keyAssign00);
        setJoyAssign(0, joyAssign00);

        // コントローラ1の設定
        setKeyAssign(1, keyAssign01);
        setJoyAssign(1, joyAssign00);

        // コントローラ2の設定
        setJoyAssign(2, joyAssign00);

        // コントローラ3の設定
        setJoyAssign(3, joyAssign00);
    }

    //--------------------------------------------------------------
    //  入力情報の更新
    //--------------------------------------------------------------
    void InputManager::update(HWND hwnd)
    {
        // キーボードの状態取得
        DirectX::Keyboard::State kb = keyboard->GetState();
        DirectX::GamePad::State gpad[GAMEPAD_NUM] = {};

        // ゲームパッドの状態取得
        for (int i = 0; i < GAMEPAD_NUM; i++)
            gpad[i] = gamePad->GetState(i);

        // 
        for (int i = 0; i < GAMEPAD_NUM; i++)
        {
            PadState* p = &pad[i];

            int old_state = p->state;

            // pad_stateの更新
            p->state = 0;

            // キーボード・マウス
            PadAssign *assign = p->keyAssign;
            if (assign)
            {
                while (assign->bit)
                {
                    if ((assign->code == VK_LBUTTON) ||
                        (assign->code == VK_RBUTTON) ||
                        (assign->code == VK_MBUTTON))
                    {	// Mouse
                        if (GetAsyncKeyState(assign->code) < 0) p->state |= assign->bit;
                    }
                    else
                    {	// Keyboard
                        if (kb.IsKeyDown((Keyboard::Keys)assign->code)) p->state |= assign->bit;
                    }

                    assign++;
                }
            }

            // ジョイスティック
            assign = p->joyAssign;
            if (assign && gpad[i].IsConnected())
            {
                while (assign->bit)
                {
                    struct hoge_t { bool b[10]; } hoge;
                    if (assign->code >= GamePad::A)
                    {	// buttons
                        hoge = *(hoge_t*)&gpad[i].buttons;
                        if (hoge.b[assign->code - GamePad::A]) p->state |= assign->bit;
                    }
                    else
                    {	// dpad
                        hoge = *(hoge_t*)&gpad[i].dpad;
                        if (hoge.b[assign->code]) p->state |= assign->bit;
                    }

                    assign++;
                }

                // 左スティック
                p->leftX = gpad[i].thumbSticks.leftX;
                p->leftY = gpad[i].thumbSticks.leftY;

                // 右スティック
                p->rightX = gpad[i].thumbSticks.rightX;
                p->rightY = gpad[i].thumbSticks.rightY;

                // 左右トリガー
                p->left = gpad[i].triggers.left;
                p->right = gpad[i].triggers.right;

                // 遊びの設定
                // ※必要があれば設定する
            }

            // p->triggerの更新
            p->trigger = (~old_state) & p->state;

            // p->triggerUpの更新
            p->triggerUp = old_state & (~p->state);

            // p->repeatの計算
            p->repeat = p->state;
            for (int j = 8 - 1; j > 0; --j) {
                p->old[j] = p->old[j - 1];				// データをひとつずらす
                p->repeat &= p->old[j];					// repeatを計算する
            }
            p->old[0] = p->state & (~p->repeat);		// 最新のデータを保存
            p->repeat |= p->trigger;					// 本来のトリガー入力も設定する
        }

        // マウス座標の取得
        GetCursorPos(&mousePos);
        ScreenToClient(hwnd, &mousePos);
    }

    //--------------------------------------------------------------
    //  マウスカーソルのx座標を取得
    //--------------------------------------------------------------
    int InputManager::getCursorPosX()
    {
        return mousePos.x;
    }

    //--------------------------------------------------------------
    //  マウスカーソルのy座標を取得
    //--------------------------------------------------------------
    int InputManager::getCursorPosY()
    {
        return mousePos.y;
    }

}

//******************************************************************************

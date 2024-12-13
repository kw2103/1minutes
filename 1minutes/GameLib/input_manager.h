#ifndef INCLUDED_INPUT_MANAGER
#define	INCLUDED_INPUT_MANAGER

//******************************************************************************
//
//
//      入力マネージャ
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
//#define NOMINMAX	// 学習用にコメントアウト
#include <Windows.h>
#include "./DirectXTK-master/Inc/Keyboard.h"
#include "./DirectXTK-master/Inc/GamePad.h"

namespace GameLib
{

    namespace input
    {

        // キーラベル
        const int PAD_UP = (1 << 0);
        const int PAD_DOWN = (1 << 1);
        const int PAD_LEFT = (1 << 2);
        const int PAD_RIGHT = (1 << 3);
        const int PAD_START = (1 << 4);
        const int PAD_TRG1 = (1 << 5);
        const int PAD_TRG2 = (1 << 6);
        const int PAD_TRG3 = (1 << 7);
        const int PAD_SELECT = (1 << 8);
        const int PAD_TRG4 = (1 << 9);
        const int PAD_L1 = (1 << 10);
        const int PAD_R1 = (1 << 11);
        const int PAD_L2 = (1 << 12);
        const int PAD_R2 = (1 << 13);
        const int PAD_L3 = (1 << 14);
        const int PAD_R3 = (1 << 15);

        // パッドの最大数
        const int	GAMEPAD_NUM = 4;

        // ゲームパッド
        enum GamePad
        {
            UP = 0,// dpad
            DOWN,
            RIGHT,
            LEFT,
            A = 4, // buttons
            B,
            X,
            Y,
            LSTICK,
            RSTICK,
            LSHOULDER,
            RSHOULDER,
            BACK,
            START,
        };

    }

    // キー割り当てデータ用構造体
    struct PadAssign
    {
        int         bit;				// PAD_STATE用ビットデータ
        int         code;				// 対応するキー（ボタン）コード
    };

    //==============================================================================
    //
    //      入力情報クラス
    //
    //==============================================================================
    class PadState
    {
    public:

        int         state;              // キー入力情報
        int         trigger;            // トリガー入力
        int         triggerUp;          // 離した瞬間
        int         repeat;             // リピート入力
        int         old[8];
        float       leftX, leftY;       // 左スティック
        float       rightX, rightY;     // 右スティック
        float       left, right;        // 左右トリガー
        PadAssign*  keyAssign;          // キーボード割り当てデータ
        PadAssign*  joyAssign;          // ジョイスティック割り当てデータ
    };

    //==============================================================================
    //
    //      InputManagerクラス
    //
    //==============================================================================
    class InputManager
    {
    private:
        DirectX::Keyboard*  keyboard = nullptr;   // https://github.com/Microsoft/DirectXTK/wiki/Keyboard
        DirectX::GamePad*   gamePad = nullptr;    // https://github.com/Microsoft/DirectXTK/wiki/GamePad

        PadState     pad[input::GAMEPAD_NUM] = {};  // ゲームパッド
        POINT        mousePos = {}; 	            // マウス座標

        // キー・ボタン割り当て
        void setKeyAssign(int no, PadAssign* data); // キー割り当ての設定
        void setJoyAssign(int no, PadAssign* data); // ジョイスティック割り当ての設定

    public:
        InputManager() { init(); }
        ~InputManager();

        // 初期化
        void init();                                // 初期化

        // 更新
        void update(HWND);                          // 入力情報の更新

        // パッド
        PadState* getPadAddress() { return pad; }	// ゲームパッドの取得

        // マウス関連
        int getCursorPosX();                        // x座標取得
        int getCursorPosY();                        // y座標取得
    };

}


//******************************************************************************

#endif // !INCLUDED_INPUT_MANAGER

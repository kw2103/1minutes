#include "game_lib.h"

namespace GameLib {

    //b2Vec2 b2GetCursorPos(HWND hwnd)
    //{
    //    POINT curPos;
    //    GetCursorPos(&curPos);
    //    ScreenToClient(hwnd, &curPos);

    //    VECTOR2 wPos = view::screenToWorld({ static_cast<float>(curPos.x), static_cast<float>(curPos.y) });
    //    return view::worldToBox2d(wPos);
    //}

    void startHrTimer();
    void stopHrTimer();

    //--------------------------------------------------------------
    //  ウインドウプロシージャ
    //--------------------------------------------------------------
    LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        using DirectX::Keyboard;

        // マウスホイール用
        static int nWheelFraction = 0;	// 回転量の端数

        // マウス移動用
        static POINT prevPos;

        switch (msg)
        {

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_ACTIVATEAPP:
            Keyboard::ProcessMessage(msg, wParam, lParam);
            nWheelFraction = 0;
            break;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
            {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
            }
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            Keyboard::ProcessMessage(msg, wParam, lParam);
            break;

        case WM_ENTERSIZEMOVE:
            // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
            stopHrTimer();
            //m.hrTimer.stop();
            break;

        case WM_EXITSIZEMOVE:
            // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
            // Here we reset everything based on the new window dimensions.
            startHrTimer();
            //m.hrTimer.start();
            break;

        case WM_LBUTTONDOWN:
        {
            //b2Vec2 b2Pos = b2GetCursorPos(hwnd);

            //if (GetAsyncKeyState(VK_SHIFT) < 0)
            //    Game::instance()->ShiftMouseDown(b2Pos);
            //else
            //    Game::instance()->MouseDown(b2Pos);

            SetCapture(hwnd);   // カーソルが画面外に出てもつかんだまま
            break;
        }

        case WM_LBUTTONUP:
        {
            //Game::instance()->MouseUp(b2GetCursorPos(hwnd));

            ReleaseCapture();   // ウィンドウを放す
            break;
        }

        case WM_RBUTTONDOWN:
        {
            SetCapture(hwnd);   // カーソルが画面外に出てもつかんだまま
            break;
        }

        case WM_RBUTTONUP:
        {
            ReleaseCapture();   // ウィンドウを放す
            break;
        }

        case WM_MOUSEMOVE:
        {
//            Game::instance()->MouseMove(b2GetCursorPos(hwnd));

            break;
        }

        case WM_MOUSEWHEEL:
        {
            POINT curPos;
            GetCursorPos(&curPos);
            ScreenToClient(hwnd, &curPos);

            // 画面外なら動作しない
            if (curPos.x < 0 || curPos.x >= window::getWidth() ||
                curPos.y < 0 || curPos.y >= window::getHeight())
                break;

            int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	// 回転量

            // 前回の端数を追加
            zDelta += nWheelFraction;

            // ノッチ数を求める
            int nNotch = zDelta / WHEEL_DELTA;

            // 端数を保存する
            nWheelFraction = zDelta % WHEEL_DELTA;

            if (nNotch == 0)
                break;

            float rate = nNotch > 0 ? 1.04f : 0.96f;
            VECTOR2 sPos = { static_cast<float>(curPos.x), static_cast<float>(curPos.y) };
            VECTOR2 wPos = view::screenToWorld(sPos);
            view::setScale(view::getScale() * rate);
            view::setByPosition(sPos, wPos);

            break;
        }

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        return 0;
    }
}

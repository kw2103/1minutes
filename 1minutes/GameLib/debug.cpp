//******************************************************************************
//
//
//      デバッグ文字列表示用
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include "debug.h"
#include "game_lib.h"

namespace GameLib
{
    //--------------------------------------------------------------
    //	コンストラクタ
    //--------------------------------------------------------------
    Debug::Debug(ID3D11Device *device, 
        const wchar_t *fileName, size_t maxInstance, int scr_h)
        : SpriteBatch(device, fileName, maxInstance), yPos(0), screenHeight(scr_h)
    {
    }

    //--------------------------------------------------------------
    //	デストラクタ
    //--------------------------------------------------------------
    Debug::~Debug()
    {
    }

    //--------------------------------------------------------------
    //	デバッグ文字列の設定
    //--------------------------------------------------------------
    void Debug::setString(const char *str)
    {
        std::string s(str);
        debugQueue.push(s);
    }

    //--------------------------------------------------------------
    //	デバッグ文字列の表示
    //--------------------------------------------------------------
    void Debug::display(ID3D11DeviceContext* context, 
        float r, float g, float b, float sx, float sy)
    {
        yPos = 0;
        begin(context);
        while (!debugQueue.empty())
        {
            std::string t = debugQueue.front();
            yPos += textout(t, VECTOR2(0, yPos), VECTOR2(sx, sy), VECTOR4(r, g, b, 1), 
                GameLib::TEXT_ALIGN::UPPER_LEFT, 0.0f);
            debugQueue.pop();

            if (yPos > screenHeight)
            {
                while (!debugQueue.empty())	debugQueue.pop();
                break;
            }
        }
        end(context);
    }

}

//******************************************************************************
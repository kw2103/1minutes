//******************************************************************************
//
//
//      フォント表示用
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include "font.h"
#include "util.h"
#include "view_settings.h"

namespace GameLib
{

    //------< データ >-----------------------------------------------------------
    LoadFont fontTexture[] = {
        { FONTNO::FONT0, L"./Data/Fonts/font0.png" }, // フォント
        { FONTNO::FONT1, L"./Data/Fonts/font1.png" }, // フォント
        { FONTNO::FONT2, L"./Data/Fonts/font2.png" }, // フォント
        { FONTNO::FONT3, L"./Data/Fonts/font3.png" }, // フォント
        { FONTNO::FONT4, L"./Data/Fonts/font4.png" }, // フォント
        { FONTNO::FONT5, L"./Data/Fonts/font5.png" }, // フォント
        { FONTNO::FONT6, L"./Data/Fonts/font6.png" }, // フォント
        { -1,            nullptr                   }  // 終了フラグ
    };

    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    FontManager::FontManager(ID3D11Device* device) : font(nullptr)
    {
        init(device);
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    FontManager::~FontManager()
    {
        uninit();
    }

    //--------------------------------------------------------------
    //  初期化
    //--------------------------------------------------------------
    void FontManager::init(ID3D11Device* device)
    {
        font = new SpriteBatch*[FONT_NUM];

        LoadFont *data = fontTexture;
        while ((size_t)data->fontNo < FONT_NUM)
        {
            font[data->fontNo] = new SpriteBatch(
                device, data->fileName, FONT_MAX_INSTANCE);
            data++;
        }
    }

    //--------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------
    void FontManager::uninit()
    {
        releaseAll();
        safe_delete_array(font);
    }

    //--------------------------------------------------------------
    //  番号を指定して解放
    //--------------------------------------------------------------
    void FontManager::release(int i)
    {
        if ((size_t)i < FONT_NUM)
        {
            safe_delete(font[i]);
        }
    }

    //--------------------------------------------------------------
    //  全て解放
    //--------------------------------------------------------------
    void FontManager::releaseAll()
    {
        for (size_t i = 0; i < FONT_NUM; i++) release(i);
    }

    //--------------------------------------------------------------
    //  テキスト表示
    //--------------------------------------------------------------
    float FontManager::textOut(ID3D11DeviceContext* context,
        int fontNo, std::string str,
        const VECTOR2& position, const VECTOR2& scale,
        const VECTOR4& color, TEXT_ALIGN align, 
        float z, bool world)
    {
        if (fontNo >= FONT_NUM) return -1;
        font[fontNo]->begin(context);
        float h = font[fontNo]->textout(str, position, scale, color, align, z, world);
        font[fontNo]->end(context);
        return h;
    }

}
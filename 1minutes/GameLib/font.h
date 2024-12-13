#ifndef INCLUDED_FONT
#define INCLUDED_FONT

//******************************************************************************
//
//
//      フォント表示用
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include <Windows.h>
#include "sprite.h"

namespace GameLib
{

    const UINT FONT_NUM = 7;
    const UINT FONT_MAX_INSTANCE = 256;

    // フォントロードデータ
    struct LoadFont
    {
        int             fontNo;     // テクスチャ番号
        const wchar_t*  fileName;   // ファイル名
    };

    // ラベル定義
    enum FONTNO
    {
        // タイトル
        FONT0 = 0,
        FONT1,
        FONT2,
        FONT3,
        FONT4,
        FONT5,
        FONT6,
    };

    //==========================================================================
    //
    //      フォントマネージャ
    //
    //==========================================================================
    class FontManager
    {
    private:
        SpriteBatch** font;

    public:
        FontManager(ID3D11Device*);
        ~FontManager();

        void init(ID3D11Device*);
        void uninit();
        void release(int);
        void releaseAll();

        float textOut(ID3D11DeviceContext*, int,
            std::string, 
            const VECTOR2&, const VECTOR2&,
            const VECTOR4&, TEXT_ALIGN align = (TEXT_ALIGN::UPPER_LEFT ), 
            float z = (0.0f), bool world = ( false ));
    };

}

#endif // !INCLUDED_FONT
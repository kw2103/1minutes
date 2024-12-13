//******************************************************************************
//
//
//      テクスチャ管理クラス、テクスチャクラス
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include "texture.h"
#include "game_lib.h"

namespace GameLib
{

    //==========================================================================
    //
    //      テクスチャ管理クラス
    //
    //==========================================================================

    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    TextureManager::TextureManager() : maxNum( 0 ), texture( nullptr ), current( 0 )
    {
        init(system::TEXTURE_NUM);
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    TextureManager::~TextureManager()
    {
        uninit();
    }

    //--------------------------------------------------------------
    //  初期化
    //--------------------------------------------------------------
    void TextureManager::init(size_t max)
    {
        uninit();	// 念のため

        texture = new Texture[max];
        maxNum = max;
    }

    //--------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------
    void TextureManager::uninit()
    {
        if (texture)
        {
            releaseAll();
            safe_delete_array(texture);
            maxNum = 0;
        }
    }

    //--------------------------------------------------------------
    //  テクスチャロード（番号指定）
    //--------------------------------------------------------------
    void TextureManager::load(ID3D11Device *device, int i, const wchar_t* fileName, size_t maxInstance)
    {
        if ((size_t)i < maxNum) texture[i].load(device, fileName, maxInstance);
    }

    //--------------------------------------------------------------
    //  テクスチャのロード（空き番号検索）
    //--------------------------------------------------------------
    int TextureManager::load(ID3D11Device *device, const wchar_t* fileName, size_t maxInstance)
    {
        for (size_t i = 0; i < maxNum; i++)
        {
            if (texture[i].isEmpty())
            {
                load(device, i, fileName, maxInstance);
                return i;
            }
        }
        return -1;
    }

    //--------------------------------------------------------------
    //  テクスチャの一括ロード
    //--------------------------------------------------------------
    void TextureManager::load(ID3D11Device* device, const LoadTexture* data)
    {
        while ((size_t)data->texNum < maxNum)
        {
            release(data->texNum);
            load(device, data->texNum, data->fileName, data->maxInstance);
            data++;
        }
    }

    //--------------------------------------------------------------
    //  テクスチャ解放（１つ）
    //--------------------------------------------------------------
    void TextureManager::release(int i)
    {
        if ((size_t)i < maxNum)
        {
            texture[i].release();
        }
    }

    //--------------------------------------------------------------
    //  テクスチャの一括解放
    //--------------------------------------------------------------
    void TextureManager::release(const LoadTexture* data)
    {
        while ((size_t)data->texNum < maxNum)
        {
            release(data->texNum);
            data++;
        }
    }

    //--------------------------------------------------------------
    //  テクスチャ解放（すべて）
    //--------------------------------------------------------------
    void TextureManager::releaseAll()
    {
        for (size_t i = 0; i < maxNum; i++) release(i);
    }

    //--------------------------------------------------------------
    //  描画の前処理
    //--------------------------------------------------------------
    void TextureManager::begin(ID3D11DeviceContext* context, int texNo)
    {
        texture[texNo].begin(context);
    }

    //--------------------------------------------------------------
    //  描画の後処理
    //--------------------------------------------------------------
    void TextureManager::end(ID3D11DeviceContext* context, int texNo)
    {
        texture[texNo].end(context);
    }

    //--------------------------------------------------------------
    //  テクスチャ描画
    //--------------------------------------------------------------
    void TextureManager::draw(int i,
        const VECTOR2& position, const VECTOR2& scale,
        const VECTOR2& texPos, const VECTOR2& texSize,
        const VECTOR2& center, float angle, const VECTOR4& color, 
        float z, 
        bool world)
    {
        if ((size_t)i < maxNum)
        {
            texture[i].draw(
                position, scale,
                texPos, texSize,
                center, angle,
                color, z, 
                world);
        }
    }

    //--------------------------------------------------------------
    //  テクスチャ描画
    //--------------------------------------------------------------
    void TextureManager::draw(int i,
        float x, float y,
        float scaleX, float scaleY,
        float left, float top,
        float width, float height,
        float centerX, float centerY, float angle,
        float r, float g, float b, float a,
        float z, 
        bool world)
    {
        if ((size_t)i < maxNum)
        {
            draw(i,
                VECTOR2(x, y), VECTOR2(scaleX, scaleY),
                VECTOR2(left, top), VECTOR2(width, height),
                VECTOR2(centerX, centerY), angle,
                VECTOR4(r, g, b, a), z, 
                world);
        }
    }

    //==========================================================================
    //
    //      テクスチャクラス
    //
    //==========================================================================

    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    Texture::Texture() : sprBat( nullptr )
    {
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    Texture::~Texture()
    {
        release();
    }

    //--------------------------------------------------------------
    //  テクスチャのロード
    //--------------------------------------------------------------
    bool Texture::load(ID3D11Device *device, const wchar_t* fileName, size_t maxInstance)
    {
        sprBat = new SpriteBatch(device, fileName, maxInstance);
        return (sprBat != nullptr);
    }

    //--------------------------------------------------------------
    //  描画の前処理
    //--------------------------------------------------------------
    void Texture::begin(ID3D11DeviceContext* context)
    {
        sprBat->begin(context);
    }

    //--------------------------------------------------------------
    //  描画の後処理
    //--------------------------------------------------------------
    void Texture::end(ID3D11DeviceContext* context)
    {
        sprBat->end(context);
    }

    //--------------------------------------------------------------
    //  テクスチャの描画
    //--------------------------------------------------------------
    void Texture::draw(
        float x, float y, float scaleX, float scaleY,
        float left, float top, float width, float height,
        float centerX, float centerY, float angle,
        float r, float g, float b, float a, 
        float z, 
        bool world)
    {
        draw(VECTOR2(x, y), VECTOR2(scaleX, scaleY),
            VECTOR2(left, top), VECTOR2(width, height),
            VECTOR2(centerX, centerY), angle,
            VECTOR4(r, g, b, a), z, 
            world);
    }

    //--------------------------------------------------------------
    //  テクスチャの描画
    //--------------------------------------------------------------
    void Texture::draw(
        const VECTOR2& pos, const VECTOR2& scale,
        const VECTOR2& texPos, const VECTOR2& texSize,
        const VECTOR2& center, float angle,
        const VECTOR4& color, float z, 
        bool world)
    {
        if (!sprBat) return;

        if (world)
        {
            float scaleFactor = view::getScale();
            VECTOR2 s = scale * scaleFactor;
            VECTOR2 c = center;
            VECTOR2 v = view::worldToScreen(pos);
            sprBat->render(v, s, texPos, texSize, c, angle, color, z);
        }
        else
        {
            float y, cy;
#ifdef GAMELIB_PLUS_UP
            y = (window::getHeight() - pos.y);
            cy = texSize.y - center.y;
#else
            y = pos.y;
            cy = center.y;
#endif

            sprBat->render(
                VECTOR2(pos.x, y), scale, texPos, texSize,
                VECTOR2(center.x, cy), angle, color, z);
        }
    }

    //--------------------------------------------------------------
    //  テクスチャの解放
    //--------------------------------------------------------------
    void Texture::release()
    {
        safe_delete(sprBat);
    }

    //--------------------------------------------------------------
    //  テクスチャの有無
    //--------------------------------------------------------------
    bool Texture::isEmpty()
    {
        return (sprBat == nullptr);
    }

}

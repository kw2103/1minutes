#ifndef INCLUDED_SPRITE
#define INCLUDED_SPRITE
//******************************************************************************
//
//
//      スプライト描画用クラス
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include <string>

#include <d3d11.h>
#include <DirectXMath.h>

#include "vector.h"
#include "resource_manager.h"

namespace GameLib
{

    // 文字描画のアライン
    enum class TEXT_ALIGN
    {
        UPPER_LEFT = 0, // 左上
        UPPER_MIDDLE,   // 上中央
        UPPER_RIGHT,    // 右上
        MIDDLE_LEFT,    // 左中央
        MIDDLE,         // 中央
        MIDDLE_RIGHT,   // 右中央
        LOWER_LEFT,     // 左下
        LOWER_MIDDLE,   // 下中央
        LOWER_RIGHT,    // 右下
    };

    //==========================================================================
    //
    //      Spriteクラス
    //
    //==========================================================================
    class Sprite
    {
    private:
        ID3D11VertexShader*         vertexShader        = nullptr;
        ID3D11PixelShader*          pixelShader         = nullptr;
        ID3D11InputLayout*          inputLayout         = nullptr;
        ID3D11Buffer*               buffer              = nullptr;
        ID3D11RasterizerState*      rasterizerState     = nullptr;

        ID3D11ShaderResourceView*   shaderResourceView  = nullptr;
        D3D11_TEXTURE2D_DESC        tex2dDesc           = {};
        ID3D11SamplerState*         samplerState        = nullptr;
        //ID3D11DepthStencilState*    depthStencilState   = nullptr;

    public:

        //--------------------------------------------------------------
        //  コンストラクタ
        //--------------------------------------------------------------
        Sprite(ID3D11Device*, const wchar_t*, int nBufSize = ( 4 ));

        //--------------------------------------------------------------
        //  デストラクタ
        //--------------------------------------------------------------
        ~Sprite();

        //--------------------------------------------------------------
        //  スプライト描画
        //--------------------------------------------------------------
        void render(ID3D11DeviceContext*, 
            const VECTOR2&, const VECTOR2&, 
            const VECTOR2&, const VECTOR2&,
            const VECTOR2&, float,
            const VECTOR4&, 
            float z = (0.5f)
            ) const;

        //--------------------------------------------------------------
        //  スプライト描画
        //--------------------------------------------------------------
        void render(ID3D11DeviceContext*,
            float, float, float, float, 
            float, float, float, float,
            float, float, float,
            float, float, float, float, 
            float z = (0.5f)
            ) const;

        //--------------------------------------------------------------
        //  テキスト描画
        //--------------------------------------------------------------
        float textout(ID3D11DeviceContext*, 
            std::string, const VECTOR2&,
            const VECTOR2&, const VECTOR4&, 
            float z = ( 0.5f ), 
            bool world = ( false )) const;

        //--------------------------------------------------------------
        //  テキスト描画
        //--------------------------------------------------------------
        float textout(ID3D11DeviceContext*, 
            std::string, float, float,
            float, float,
            float, float, float, float, 
            float z = (0.5f),
            bool world = ( false )) const;

        //--------------------------------------------------------------
        //  四角ポリゴン描画（頂点指定）
        //--------------------------------------------------------------
        void quad(ID3D11DeviceContext*, 
            const VECTOR2(&v)[4],
            const VECTOR2&, const VECTOR2&,
            const VECTOR4&, 
            float z = (0.5f)
            ) const;

        //--------------------------------------------------------------
        //  四角ポリゴン描画（頂点指定）
        //--------------------------------------------------------------
        void quad(ID3D11DeviceContext*, 
            const VECTOR2(&v)[4],
            float, float, float, float,
            float, float, float, float, 
            float z = (0.5f)
        ) const;

        void draw_triangle_strip(ID3D11DeviceContext*,
            const VECTOR3* pVertex, const VECTOR4* pColor, const VECTOR2* pUvs,
            int nVert, int nPolygon, float z = (0.5f)
            ) const;

        void draw_triangles_indexed(ID3D11DeviceContext*,
            const VECTOR3* pVertex, const VECTOR4* pColor, const VECTOR2* pUvs,
            short* pIndices, int nVert, int nPolygon, float z = (0.5f)
            ) const;

        void draw_triangle_fan_indexed(ID3D11DeviceContext*,
            const VECTOR3* pVertex, const VECTOR4* pColor, const VECTOR2* pUvs,
            short* pIndices, int nPolygon, float z = (0.5f)
            ) const;

        //--------------------------------------------------------------
        //  スプライトのサイズ取得
        //--------------------------------------------------------------
        bool getSize(int& w, int& h);

        //--------------------------------------------------------------
        //  構造体定義
        //--------------------------------------------------------------
        struct vertex { VECTOR3 position; VECTOR4 color; VECTOR2 texcoord; };
    };

    //==========================================================================
    //
    //      SpriteBatchクラス
    //
    //==========================================================================
    class SpriteBatch
    {
    private:
        ID3D11VertexShader*         vertexShader        = nullptr;
        ID3D11PixelShader*          pixelShader         = nullptr;
        ID3D11InputLayout*          inputLayout         = nullptr;
        ID3D11Buffer*               buffer              = nullptr;
        ID3D11RasterizerState*      rasterizerState     = nullptr;

        ID3D11ShaderResourceView*   shaderResourceView  = nullptr;
        D3D11_TEXTURE2D_DESC        tex2dDesc           = {};
        ID3D11SamplerState*         samplerState        = nullptr;
//        ID3D11DepthStencilState*    depthStencilState   = nullptr;

        size_t MAX_INSTANCES;

        //--------------------------------------------------------------
        //  構造体定義
        //--------------------------------------------------------------
        struct instance
        {
            DirectX::XMFLOAT4X4 ndcTransform            = {};
            VECTOR4 texcoordTransform                   = {};
            VECTOR4 color                               = {};
        };
        ID3D11Buffer* instanceBuffer;

    public:

        //--------------------------------------------------------------
        //  構造体定義
        //--------------------------------------------------------------
        struct vertex
        {
            VECTOR3 position                            = {};
            VECTOR2 texcoord                            = {};
        };

        //--------------------------------------------------------------
        //  コンストラクタ
        //--------------------------------------------------------------
        SpriteBatch(ID3D11Device*, const wchar_t*, size_t);

        //--------------------------------------------------------------
        //  デストラクタ
        //--------------------------------------------------------------
        ~SpriteBatch();

        //--------------------------------------------------------------
        //  前処理
        //--------------------------------------------------------------
        void begin(ID3D11DeviceContext*);

        //--------------------------------------------------------------
        //  スプライトバッチ描画
        //--------------------------------------------------------------
        void render(
            const VECTOR2&, const VECTOR2&, 
            const VECTOR2&, const VECTOR2&,
            const VECTOR2&, float angle, 
            const VECTOR4&, float z = (0.5f)
            );

        //--------------------------------------------------------------
        //  テキスト描画
        //--------------------------------------------------------------
        float textout(
            std::string, const VECTOR2&,
            const VECTOR2&, const VECTOR4&, 
            TEXT_ALIGN align = (TEXT_ALIGN::UPPER_LEFT ), 
            float z = (0.5f), 
            bool world = ( false )
            );

        //--------------------------------------------------------------
        //  後処理
        //--------------------------------------------------------------
        void end(ID3D11DeviceContext*);

    private:

        D3D11_VIEWPORT viewport;

        UINT instanceCount = 0;
        instance* instances = nullptr;
    };

}

//******************************************************************************

#endif // !INCLUDED_SPRITE

//******************************************************************************
//
//
//      Sprite描画用
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include "sprite.h"
#include "util.h"
#include "misc.h"
#include "game_lib.h"

namespace GameLib
{

    //--------------------------------------------------------------
    //  テキスト描画の原点の調整用
    //--------------------------------------------------------------
    VECTOR2 adjustTextOrigin(TEXT_ALIGN align, const VECTOR2& v, float w, float h)
    {
        VECTOR2 pos = v;
        switch (align)
        {
        default:
        case TEXT_ALIGN::UPPER_LEFT:                                    break;
        case TEXT_ALIGN::UPPER_MIDDLE:  pos.x -= w / 2;                 break;
        case TEXT_ALIGN::UPPER_RIGHT:   pos.x -= w;                     break;
        case TEXT_ALIGN::MIDDLE_LEFT:   pos.y -= h / 2;                 break;
        case TEXT_ALIGN::MIDDLE:        pos.y -= h / 2; pos.x -= w / 2; break;
        case TEXT_ALIGN::MIDDLE_RIGHT:  pos.y -= h / 2; pos.x -= w;     break;
        case TEXT_ALIGN::LOWER_LEFT:    pos.y -= h;                     break;
        case TEXT_ALIGN::LOWER_MIDDLE:  pos.y -= h;     pos.x -= w / 2; break;
        case TEXT_ALIGN::LOWER_RIGHT:   pos.y -= h;     pos.x -= w;     break;
        }

        return pos;
    }

    static const float UV_ADJUST = 1.0f;// 0.99994f;

    //==========================================================================
    //
    //      Spriteクラス
    //
    //==========================================================================

    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    Sprite::Sprite(ID3D11Device* device, const wchar_t* fileName, int nBufSize)
    {
        //VertexBufferの作成
        //vertex vertices[4] = {};
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(vertex) * nBufSize;   // 頂点バッファのサイズ
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // 頂点バッファ
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU→読み取りのみ　CPU→書き込みのみ
        bd.MiscFlags = 0;
        bd.StructureByteStride = 0;
        //D3D11_SUBRESOURCE_DATA initData = {};
        //initData.pSysMem = vertices;                // 頂点のアドレス
        //initData.SysMemPitch = 0;
        //initData.SysMemSlicePitch = 0;
        if (FAILED(device->CreateBuffer(&bd, nullptr, &buffer)))
        {
            assert(!"頂点バッファの作成に失敗(Sprite)");
            return;
        }

        // 頂点宣言
        // 入力レイアウトの定義
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,     D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 4 * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        //	頂点シェーダーの読み込み
        if (!ResourceManager::loadVertexShader(device, "./Data/Shaders/sprite_vs.cso", layout, numElements, &vertexShader, &inputLayout))
        {
            assert(!"頂点シェーダーの読み込みに失敗(Sprite)");
            return;
        }
        //	ピクセルシェーダーの作成
        if (!ResourceManager::loadPixelShader(device, "./Data/Shaders/sprite_ps.cso", &pixelShader))
        {
            assert(!"ピクセルシェーダーの作成に失敗(Sprite)");
            return;
        }

        //	ラスタライザステートの設定
        D3D11_RASTERIZER_DESC rsDesc = {};
        rsDesc.FillMode = D3D11_FILL_SOLID;//
        rsDesc.CullMode = D3D11_CULL_NONE;//	カリング
        rsDesc.FrontCounterClockwise = false;
        rsDesc.DepthBias = 0;
        rsDesc.DepthBiasClamp = 0;
        rsDesc.SlopeScaledDepthBias = 0;
        rsDesc.DepthClipEnable = false;
        rsDesc.ScissorEnable = false;
        rsDesc.MultisampleEnable = false;
        rsDesc.AntialiasedLineEnable = false;
        if (FAILED(device->CreateRasterizerState(&rsDesc, &rasterizerState)))
        {
            assert(!"ラスタライザステートの作成に失敗(Sprite)");
            return;
        }

        //	テクスチャ画像読み込み
        if (!ResourceManager::loadShaderResourceView(device, fileName, &shaderResourceView, &tex2dDesc))
        {
            assert(!"テクスチャ画像読み込み失敗(Sprite)");
            return;
        }

        //	サンプラーステート設定
        D3D11_SAMPLER_DESC	samplerDesc;
        SecureZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.MipLODBias = 0;
        samplerDesc.MaxAnisotropy = 16;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        if (FAILED(device->CreateSamplerState(&samplerDesc, &samplerState)))
        {
            assert(!"サンプラーステートの生成に失敗(Sprite)");
            return;
        }

        //D3D11_DEPTH_STENCIL_DESC dsDesc;
        //dsDesc.DepthEnable = TRUE;
        //dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        //dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
        //dsDesc.StencilEnable = FALSE;
        //dsDesc.StencilReadMask = 0xFF;
        //dsDesc.StencilWriteMask = 0xFF;
        //dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        //dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        //dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        //dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        //dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        //dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        //dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        //dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        //if (FAILED(device->CreateDepthStencilState(&dsDesc, &depthStencilState)))
        //{
        //    assert(!"デプスステンシルステートの作成に失敗(Sprite)");
        //    return;
        //}
    }

    //--------------------------------------------------------------
    //	デストラクタ
    //--------------------------------------------------------------
    Sprite::~Sprite()
    {
        //safe_release(depthStencilState);
        safe_release(samplerState);
        ResourceManager::releaseShaderResourceView(shaderResourceView);
        safe_release(rasterizerState);
        safe_release(buffer);
        ResourceManager::releasePixelShader(pixelShader);
        ResourceManager::releaseVertexShader(vertexShader, inputLayout);
    }

    //--------------------------------------------------------------
    //  スプライト描画
    //--------------------------------------------------------------
    void Sprite::render(ID3D11DeviceContext* context, 
        const VECTOR2& position, const VECTOR2& scale,
        const VECTOR2& texPos, const VECTOR2& texSize, 
        const VECTOR2& center, float angle, 
        const VECTOR4& color, float z) const
    {
        if (scale.x == 0.0f || scale.y == 0.0f) return;

        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        float tw = texSize.x;
        float th = texSize.y;
        if (tw == FLT_MIN && th == FLT_MIN)
        {
            tw = (float)tex2dDesc.Width;
            th = (float)tex2dDesc.Height;
        }

        vertex vertices[4] = {};
#ifdef GAMELIB_PLUS_UP
        vertices[0] = { VECTOR3(0.0f, 1.0f, z), color, VECTOR2(0, 0) };
        vertices[1] = { VECTOR3(1.0f, 1.0f, z), color, VECTOR2(1, 0) };
        vertices[2] = { VECTOR3(0.0f, 0.0f, z), color, VECTOR2(0, 1) };
        vertices[3] = { VECTOR3(1.0f, 0.0f, z), color, VECTOR2(1, 1) };
#else
        vertices[0] = { VECTOR3(0.0f, 0.0f, z), color, VECTOR2(0, 0) };
        vertices[1] = { VECTOR3(1.0f, 0.0f, z), color, VECTOR2(1, 0) };
        vertices[2] = { VECTOR3(0.0f, 1.0f, z), color, VECTOR2(0, 1) };
        vertices[3] = { VECTOR3(1.0f, 1.0f, z), color, VECTOR2(1, 1) };
#endif

        float sinValue = sinf(angle);
        float cosValue = cosf(angle);
        float mx = (tw * scale.x) / tw * center.x;
        float my = (th * scale.y) / th * center.y;
        for (int i = 0; i < 4; i++)
        {
            vertices[i].position.x *= (tw * scale.x);
            vertices[i].position.y *= (th * scale.y);

            vertices[i].position.x -= mx;
            vertices[i].position.y -= my;

            float rx = vertices[i].position.x;
            float ry = vertices[i].position.y;
            vertices[i].position.x = rx * cosValue - ry * sinValue;
            vertices[i].position.y = rx * sinValue + ry * cosValue;

            vertices[i].position.x += mx;
            vertices[i].position.y += my;

            vertices[i].position.x += (position.x - scale.x * center.x);
            vertices[i].position.y += (position.y - scale.y * center.y);

            vertices[i].position.x = -1.0f + vertices[i].position.x * 2 / viewport.Width;
            vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;
#ifdef GAMELIB_PLUS_UP
            vertices[i].position.y = -vertices[i].position.y;
#endif

            // UV座標の調整
            vertices[i].texcoord.x = (std::min)(vertices[i].texcoord.x, UV_ADJUST);
            vertices[i].texcoord.y = (std::min)(vertices[i].texcoord.y, UV_ADJUST);

            vertices[i].texcoord.x = (texPos.x + vertices[i].texcoord.x * tw) / tex2dDesc.Width;
            vertices[i].texcoord.y = (texPos.y + vertices[i].texcoord.y * th) / tex2dDesc.Height;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, vertices, sizeof(vertices));
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);

        context->PSSetShaderResources(0, 1, &shaderResourceView);
        context->PSSetSamplers(0, 1, &samplerState);

        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(4, 0);
    }

    //--------------------------------------------------------------
    //  スプライト描画
    //--------------------------------------------------------------
    void Sprite::render(ID3D11DeviceContext* context, 
        float x, float y, float sx, float sy, 
        float tx, float ty, float tw, float th,
        float cx, float cy, float angle, 
        float r, float g, float b, float a, 
        float z
        ) const
    {
        return render(context, VECTOR2(x, y), VECTOR2(sx, sy), VECTOR2(tx, ty), VECTOR2(tw, th), VECTOR2(cx, cy), angle, VECTOR4(r, g, b, a), z);
    }

    //--------------------------------------------------------------
    //  テキスト描画
    //--------------------------------------------------------------
    float Sprite::textout(ID3D11DeviceContext* context, 
        std::string str, 
        const VECTOR2& pos, const VECTOR2& scale, 
        const VECTOR4& color, float z, 
        bool world) const
    {
        float tw = static_cast<float>(tex2dDesc.Width / 16.0);
        float th = static_cast<float>(tex2dDesc.Height / 16.0);
        float cursor = 0.0f;

        if (world)
        {
            VECTOR2 v, s;
#ifdef GAMELIB_PLUS_UP
            v = (pos - view::getLeftBottom()) * view::getScale();
            s = scale * view::getScale();
#else
            v = (pos - view::getLeftTop()) * view::getScale();
            s = scale * view::getScale();
#endif
            for (const auto& c : str)
            {
                render(context, VECTOR2(v.x + cursor, v.y), s, VECTOR2(tw*(c & 0x0F), th*(c >> 4)),
                    VECTOR2(tw, th), VECTOR2(0, 0), 0, color, z);
                cursor += tw * s.x;
            }
            return th * s.y;
        }
        else
        {
            for (const auto& c : str)
            {
                render(context, VECTOR2(pos.x + cursor, pos.y), scale, VECTOR2(tw*(c & 0x0F), th*(c >> 4)),
                    VECTOR2(tw, th), VECTOR2(0, 0), 0, color, z);
                cursor += tw * scale.x;
            }
            return th * scale.y;
        }
    }

    //--------------------------------------------------------------
    //  テキスト描画
    //--------------------------------------------------------------
    float Sprite::textout(ID3D11DeviceContext* context,
        std::string s,
        float x, float y, float sx, float sy,
        float r, float g, float b, float a, 
        float z, 
        bool world) const
    {
        return textout(context, s, VECTOR2(x, y), VECTOR2(sx, sy), VECTOR4(r, g, b, a), z, world);
    }

    //--------------------------------------------------------------
    //  四角ポリゴン描画
    //--------------------------------------------------------------
    void Sprite::quad(ID3D11DeviceContext* context, 
        const VECTOR2(&v)[4], 
        const VECTOR2& texPos, const VECTOR2& texSize, 
        const VECTOR4 &color, float z) const
    {
        float tw = texSize.x;
        float th = texSize.y;
        if (tw <= 0.0f || th <= 0.0f)
        {
            tw = (float)tex2dDesc.Width;
            th = (float)tex2dDesc.Height;
        }

        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[4];
#ifdef GAMELIB_PLUS_UP
        vertices[0] = { VECTOR3(v[3].x, v[3].y, z), color, VECTOR2(0, 0) };
        vertices[1] = { VECTOR3(v[2].x, v[2].y, z), color, VECTOR2(1, 0) };
        vertices[2] = { VECTOR3(v[0].x, v[0].y, z), color, VECTOR2(0, 1) };
        vertices[3] = { VECTOR3(v[1].x, v[1].y, z), color, VECTOR2(1, 1) };
#else
        vertices[0] = { VECTOR3(v[3].x, v[3].y, z), color, VECTOR2(0, 1) };
        vertices[1] = { VECTOR3(v[2].x, v[2].y, z), color, VECTOR2(1, 1) };
        vertices[2] = { VECTOR3(v[0].x, v[0].y, z), color, VECTOR2(0, 0) };
        vertices[3] = { VECTOR3(v[1].x, v[1].y, z), color, VECTOR2(1, 0) };
#endif

        for (int i = 0; i < 4; i++)
        {
            vertices[i].position.x = -1.0f + vertices[i].position.x * 2 / viewport.Width;
            vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;
#ifdef GAMELIB_PLUS_UP
            vertices[i].position.y = -vertices[i].position.y;
#endif
            vertices[i].texcoord.x = (texPos.x + vertices[i].texcoord.x * tw) / tex2dDesc.Width;
            vertices[i].texcoord.y = (texPos.y + vertices[i].texcoord.y * th) / tex2dDesc.Height;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, vertices, sizeof(vertices));
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, NULL, 0);
        context->PSSetShader(pixelShader, NULL, 0);
        context->PSSetShaderResources(0, 1, &shaderResourceView);
        context->PSSetSamplers(0, 1, &samplerState);
        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(4, 0);
    }

    //--------------------------------------------------------------
    //  四角ポリゴン描画
    //--------------------------------------------------------------
    void Sprite::quad(ID3D11DeviceContext* context, 
        const VECTOR2(&v)[4], 
        float tx, float ty, float tw, float th, 
        float r, float g, float b, float a, 
        float z) const
    {
        quad(context, v, VECTOR2(tx, ty), VECTOR2(tw, th), VECTOR4(r, g, b, a), z);
    }

    void Sprite::draw_triangle_strip(ID3D11DeviceContext* context,
        const VECTOR3* pVertex, const VECTOR4* pColor, const VECTOR2* pUvs,
        int nVert, int nPolygon, float z) const
    {
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[128] = {};
        for (int i = 0; i < nVert; i++)
        {
            vertices[i].position = pVertex[i];
            vertices[i].color = pColor[i];
            vertices[i].texcoord = pUvs[i];

            vertices[i].position.x = -1.0f + vertices[i].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[i].position.y = -1.0f + vertices[i].position.y * 2 / viewport.Height;
#else
            vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;
#endif
            vertices[i].position.z = z;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        //vertex* v = (vertex*)msr.pData;
        memcpy(msr.pData, vertices, sizeof(vertex) * nVert);
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);

        context->PSSetShaderResources(0, 1, &shaderResourceView);
        context->PSSetSamplers(0, 1, &samplerState);

        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(nPolygon + 2, 0);
    }

    void Sprite::draw_triangles_indexed(ID3D11DeviceContext* context,
        const VECTOR3* pVertex, const VECTOR4* pColor, const VECTOR2* pUvs,
        short* pIndices, int nVert, int nPolygon, float z) const 
    {
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[PRIMITIVE_VERTEX_NUM] = {};
        for (int i = 0; i < nVert; i++)
        {
            vertices[i].position = pVertex[pIndices[i]];
            vertices[i].color = pColor[pIndices[i]];
            vertices[i].texcoord = pUvs[pIndices[i]];

            vertices[i].position.x = -1.0f + vertices[i].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[i].position.y = -1.0f + vertices[i].position.y * 2 / viewport.Height;
#else
            vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;
#endif
            vertices[i].position.z = z;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        //vertex* v = (vertex*)msr.pData;
        memcpy(msr.pData, vertices, sizeof(vertex) * nVert);
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);

        context->PSSetShaderResources(0, 1, &shaderResourceView);
        context->PSSetSamplers(0, 1, &samplerState);

        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(nPolygon * 3, 0);
    }

    void Sprite::draw_triangle_fan_indexed(ID3D11DeviceContext* context,
        const VECTOR3* pVertex, const VECTOR4* pColor, const VECTOR2* pUvs,
        short* pIndices, int nPolygon, float z) const
    {
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[128] = {};
        int index = 0;
        for (int i = 0; i < nPolygon; i++)
        {
            vertices[index].position = pVertex[pIndices[0]];//最初の頂点を中心としている
            vertices[index].position.x = -1.0f + vertices[index].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].position.y = -1.0f + vertices[index].position.y * 2 / viewport.Height;
#else
            vertices[index].position.y = 1.0f - vertices[index].position.y * 2 / viewport.Height;
#endif
            vertices[index].position.z = z;
            vertices[index].color = pColor[pIndices[0]];
            vertices[index].texcoord = pUvs[pIndices[0]];
            index++;

            vertices[index].position = pVertex[pIndices[i + 1]];
            vertices[index].position.x = -1.0f + vertices[index].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].position.y = -1.0f + vertices[index].position.y * 2 / viewport.Height;
#else
            vertices[index].position.y = 1.0f - vertices[index].position.y * 2 / viewport.Height;
#endif
            vertices[index].position.z = z;
            vertices[index].color = pColor[pIndices[i + 1]];
            vertices[index].texcoord = pUvs[pIndices[i + 1]];
            index++;

            vertices[index].position = pVertex[pIndices[i + 2]];
            vertices[index].position.x = -1.0f + vertices[index].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].position.y = -1.0f + vertices[index].position.y * 2 / viewport.Height;
#else
            vertices[index].position.y = 1.0f - vertices[index].position.y * 2 / viewport.Height;
#endif
            vertices[index].position.z = z;
            vertices[index].color = pColor[pIndices[i + 2]];
            vertices[index].texcoord = pUvs[pIndices[i + 2]];
            index++;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        //vertex* v = (vertex*)msr.pData;
        memcpy(msr.pData, vertices, sizeof(vertex) * index);
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);

        context->PSSetShaderResources(0, 1, &shaderResourceView);
        context->PSSetSamplers(0, 1, &samplerState);

        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(nPolygon * 3, 0);
    }

    //--------------------------------------------------------------
    //  スプライトのサイズ取得
    //--------------------------------------------------------------
    bool Sprite::getSize(int& w, int& h)
    {
        w = tex2dDesc.Width;
        h = tex2dDesc.Height;

        return true;
    }

    //==========================================================================
    //
    //      SpriteBatchクラス
    //
    //==========================================================================

    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    SpriteBatch::SpriteBatch(ID3D11Device* device, const wchar_t* fileName, size_t maxInstance)
    {
        MAX_INSTANCES = maxInstance;

        //VertexBufferの作成
        vertex vertices[4] = {
            { VECTOR3(0, 0, 0), VECTOR2(0, 0) },
            { VECTOR3(1, 0, 0), VECTOR2(1, 0) },
            { VECTOR3(0, 1, 0), VECTOR2(0, 1) },
            { VECTOR3(1, 1, 0), VECTOR2(1, 1) },
        };

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = sizeof(vertices);
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem = vertices;
        subresourceData.SysMemPitch = 0; //Not use for vertex buffers.
        subresourceData.SysMemSlicePitch = 0; //Not use for vertex buffers.
        if (FAILED(device->CreateBuffer(&bufferDesc, &subresourceData, &buffer)))
        {
            assert(!"頂点バッファの作成に失敗(SpriteBatch)");
            return;
        }

        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION",           0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
            { "TEXCOORD",           0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
            { "NDC_TRANSFORM",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM",      1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM",      2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM",      3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "TEXCOORD_TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "COLOR",              0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        };
        UINT numElements = ARRAYSIZE(layout);
        if (!ResourceManager::loadVertexShader(device, "./Data/Shaders/sprite_batch_vs.cso", layout, numElements, &vertexShader, &inputLayout))
        {
            assert(!"頂点シェーダーの読み込みに失敗(SpriteBatch)");
            return;
        }
        if (!ResourceManager::loadPixelShader(device, "./Data/Shaders/sprite_batch_ps.cso", &pixelShader))
        {
            assert(!"ピクセルシェーダーの作成に失敗(SpriteBatch)");
            return;
        }

        instance* inst = new instance[MAX_INSTANCES];
        {
            D3D11_BUFFER_DESC bd = {};
            D3D11_SUBRESOURCE_DATA sd = {};

            bd.ByteWidth = sizeof(instance) * MAX_INSTANCES;
            bd.Usage = D3D11_USAGE_DYNAMIC;
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bd.MiscFlags = 0;
            bd.StructureByteStride = 0;
            sd.pSysMem = inst;
            sd.SysMemPitch = 0; //Not use for vertex buffers.mm 
            sd.SysMemSlicePitch = 0; //Not use for vertex buffers.
            if (FAILED(device->CreateBuffer(&bd, &sd, &instanceBuffer)))
            {
                assert(!"バッファの生成に失敗(SpriteBatch)");
                return;
            }
        }
        delete[] inst;

        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
        rasterizerDesc.CullMode = D3D11_CULL_NONE; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK   
        rasterizerDesc.FrontCounterClockwise = FALSE;
        rasterizerDesc.DepthBias = 0;
        rasterizerDesc.DepthBiasClamp = 0;
        rasterizerDesc.SlopeScaledDepthBias = 0;
        rasterizerDesc.DepthClipEnable = FALSE;
        rasterizerDesc.ScissorEnable = FALSE;
        rasterizerDesc.MultisampleEnable = FALSE;
        rasterizerDesc.AntialiasedLineEnable = FALSE;
        if (FAILED(device->CreateRasterizerState(&rasterizerDesc, &rasterizerState)))
        {
            assert(!"ラスタライザステートの作成に失敗(SpriteBatch)");
            return;
        }

        if (!ResourceManager::loadShaderResourceView(device, fileName, &shaderResourceView, &tex2dDesc))
        {
            assert(!"テクスチャ画像読み込み失敗(SpriteBatch)");
            return;
        }

        D3D11_SAMPLER_DESC samplerDesc;
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.MipLODBias = 0;
        samplerDesc.MaxAnisotropy = 16;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        for (int i = 0; i < 4; i++) samplerDesc.BorderColor[i] = 0;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        if (FAILED(device->CreateSamplerState(&samplerDesc, &samplerState)))
        {
            assert(!"サンプラーステートの生成に失敗(SpriteBatch)");
            return;
        }

        //D3D11_DEPTH_STENCIL_DESC dsDesc;
        //dsDesc.DepthEnable = false;
        //dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        //dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        //dsDesc.StencilEnable = false;
        //dsDesc.StencilReadMask = 0xFF;
        //dsDesc.StencilWriteMask = 0xFF;
        //dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        //dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        //dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        //dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        //dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        //dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        //dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        //dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        //if (FAILED(device->CreateDepthStencilState(&dsDesc, &depthStencilState)))
        //{
        //    assert(!"デプスステンシルステートの作成に失敗(SpriteBatch)");
        //    return;
        //}
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    SpriteBatch::~SpriteBatch()
    {
        safe_release(instanceBuffer);
//        safe_release(depthStencilState);
        safe_release(samplerState);
        ResourceManager::releaseShaderResourceView(shaderResourceView);
        safe_release(rasterizerState);
        safe_release(buffer);
        ResourceManager::releasePixelShader(pixelShader);
        ResourceManager::releaseVertexShader(vertexShader, inputLayout);
    }

    //--------------------------------------------------------------
    //  前処理（描画前に1度呼ぶ）
    //--------------------------------------------------------------
    void SpriteBatch::begin(ID3D11DeviceContext* context)
    {
        HRESULT hr = S_OK;

        UINT strides[2] = { sizeof(vertex), sizeof(instance) };
        UINT offsets[2] = { 0, 0 };
        ID3D11Buffer *vbs[2] = { buffer, instanceBuffer };
        context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout);
        //context->OMSetDepthStencilState(depthStencilState, 1);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);
        context->PSSetShaderResources(0, 1, &shaderResourceView);
        context->PSSetSamplers(0, 1, &samplerState);

        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
        D3D11_MAPPED_SUBRESOURCE mappedBuffer;
        hr = context->Map(instanceBuffer, 0, map, 0, &mappedBuffer);
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        instances = static_cast<instance *>(mappedBuffer.pData);

        instanceCount = 0;
    }

    //--------------------------------------------------------------
    //  描画準備
    //--------------------------------------------------------------
    void SpriteBatch::render(
        const VECTOR2& position, const VECTOR2& scale, 
        const VECTOR2& texPos, const VECTOR2& texSize,
        const VECTOR2& center, float angle/*radian*/, 
        const VECTOR4& color, float z)
    {
        if (instanceCount >= MAX_INSTANCES)
        {
            assert(!"Number of instances must be less than MAX_INSTANCES.(SpriteBatch)");
            return;
        }

        if (scale.x == 0.0f || scale.y == 0.0f) return;
        float tw = texSize.x;
        float th = texSize.y;
        if (texSize.x == FLT_MIN && texSize.y == FLT_MIN)
        {
            tw = (float)tex2dDesc.Width;
            th = (float)tex2dDesc.Height;
        }

        //	float cx = dw*0.5f, cy = dh*0.5f; /*Center of Rotation*/
        float cx = center.x;
        float cy = center.y;
        float sx = scale.x;
        float sy = scale.y;

#ifdef GAMELIB_PLUS_UP
        sy = -sy; // Y軸上が正ならスケール反転
        cy = texSize.y - cy;
#endif

        cx *= sx;
        cy *= sy;

#if 0
        DirectX::XMVECTOR scaling = DirectX::XMVectorSet(dw, dh, 1.0f, 0.0f);
        DirectX::XMVECTOR origin = DirectX::XMVectorSet(cx, cy, 0.0f, 0.0f);
        DirectX::XMVECTOR translation = DirectX::XMVectorSet(dx, dy, 0.0f, 0.0f);
        DirectX::XMMATRIX M = DirectX::XMMatrixAffineTransformation2D(scaling, origin, angle*0.01745f, translation);
        DirectX::XMMATRIX N(
            2.0f / viewport.Width, 0.0f, 0.0f, 0.0f,
            0.0f, -2.0f / viewport.Height, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f);
        XMStoreFloat4x4(&instances[count_instance].ndc_transform, DirectX::XMMatrixTranspose(M*N)); //column_major
#else
        FLOAT c = cosf(angle);
        FLOAT s = sinf(angle);
        FLOAT w = 2.0f / viewport.Width;
        FLOAT h = -2.0f / viewport.Height;

        instances[instanceCount].ndcTransform._11 = w * sx * tw * c;
        instances[instanceCount].ndcTransform._21 = h * sx * tw * s;
        instances[instanceCount].ndcTransform._31 = 0.0f;
        instances[instanceCount].ndcTransform._41 = 0.0f;
        instances[instanceCount].ndcTransform._12 = w * sy * th * -s;
        instances[instanceCount].ndcTransform._22 = h * sy * th * c;
        instances[instanceCount].ndcTransform._32 = 0.0f;
        instances[instanceCount].ndcTransform._42 = 0.0f;
        instances[instanceCount].ndcTransform._13 = 0.0f;
        instances[instanceCount].ndcTransform._23 = 0.0f;
        instances[instanceCount].ndcTransform._33 = 1.0f;
        instances[instanceCount].ndcTransform._43 = 0.0f;
        instances[instanceCount].ndcTransform._14 = w * (-cx * c + -cy * -s + cx * 0 + position.x) - 1.0f;
        instances[instanceCount].ndcTransform._24 = h * (-cx * s + -cy * c + cy * 0 + position.y) + 1.0f;
        instances[instanceCount].ndcTransform._34 = z;
        instances[instanceCount].ndcTransform._44 = 1.0f;
#ifdef GAMELIB_PLUS_UP
            instances[instanceCount].ndcTransform._21 *= -1;
            instances[instanceCount].ndcTransform._22 *= -1;
            instances[instanceCount].ndcTransform._24 *= -1;
#endif
#endif
        float tex_width = static_cast<float>(tex2dDesc.Width);
        float tex_height = static_cast<float>(tex2dDesc.Height);

        // UV座標の調整
        float u = tw * UV_ADJUST / tex_width;
        float v = th * UV_ADJUST / tex_height;

        instances[instanceCount].texcoordTransform = VECTOR4(texPos.x / tex_width, texPos.y / tex_height, u, v);
        instances[instanceCount].color = color;

        instanceCount++;
    }

    //--------------------------------------------------------------
    //  テキスト描画準備
    //--------------------------------------------------------------
    float SpriteBatch::textout(std::string str, 
        const VECTOR2& pos, const VECTOR2& scale, 
        const VECTOR4& color, TEXT_ALIGN align, 
        float z,
        bool world)
    {
        float tw = static_cast<float>(tex2dDesc.Width / 16.0);
        float th = static_cast<float>(tex2dDesc.Height / 16.0);
        float cursor = 0.0f;

        // alignを元に左上の座標を計算
        float textWidth = str.length() * tw * scale.x;
        float textHeight = th * scale.y;
        VECTOR2 position = adjustTextOrigin(align, pos, textWidth, textHeight);

        if (world)
        {
            VECTOR2 v = view::worldToScreen(position);
            VECTOR2 s = scale * view::getScale();

            for (const auto& c : str)
            {
                render(VECTOR2(v.x + cursor, v.y), s, VECTOR2(tw*(c & 0x0F), th*(c >> 4)),
                    VECTOR2(tw, th), VECTOR2(0, 0), 0, color, z);
                cursor += tw * s.x;
            }
            return th * s.y;
        }
        else
        {
            float y;
#ifdef GAMELIB_PLUS_UP
            y = (window::getHeight() - position.y) - th * scale.y;
#else
            y = position.y;
#endif

            for (const auto& c : str)
            {
                render(VECTOR2(position.x + cursor, y), scale, VECTOR2(tw*(c & 0x0F), th*(c >> 4)),
                    VECTOR2(tw, th), VECTOR2(0, 0), 0, color, z);
                cursor += tw * scale.x;
            }
            return th * scale.y;
        }
    }

    //--------------------------------------------------------------
    //  後処理（描画準備後に1度呼ぶ）
    //--------------------------------------------------------------
    void SpriteBatch::end(ID3D11DeviceContext* context)
    {
        context->Unmap(instanceBuffer, 0);
        context->DrawInstanced(4, instanceCount, 0, 0);
    }

}

//******************************************************************************

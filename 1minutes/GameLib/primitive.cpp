//******************************************************************************
//
//
//      プリミティブ描画用
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include "primitive.h"
#include "resource_manager.h"
#include "misc.h"
#include "util.h"
#include "game_lib.h"

namespace GameLib
{

    //**************************************************************************
    //
    //      Primitiveクラス
    //
    //**************************************************************************

    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    Primitive::Primitive(ID3D11Device* device)
    {
        //VertexBufferの作成
        vertex vertices[PRIMITIVE_VERTEX_NUM] = { VECTOR3(0, 0, 0), VECTOR4(0, 0, 0, 0) };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(vertices);			// 頂点バッファのサイズ
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファ
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// GPU→読み取りのみ　CPU→書き込みのみ
        bd.MiscFlags = 0;
        bd.StructureByteStride = 0;
        if (FAILED(device->CreateBuffer(&bd, nullptr, &buffer)))
        {
            assert(!"頂点バッファの作成に失敗(Primitive)");
            return;
        }

        // 頂点宣言
        // 入力レイアウトの定義
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,     D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        //	頂点シェーダーの読み込み
        if (!ResourceManager::loadVertexShader(device, "./Data/Shaders/primitive_vs.cso", layout, numElements, &vertexShader, &inputLayout))
        {
            assert(!"頂点シェーダーの読み込みに失敗(Primitive)");
            return;
        }
        //	ピクセルシェーダーの作成
        if (!ResourceManager::loadPixelShader(device, "./Data/Shaders/primitive_ps.cso", &pixelShader))
        {
            assert(!"ピクセルシェーダーの作成に失敗(Primitive)");
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
            assert(!"ラスタライザステートの作成に失敗(Primitive)");
            return;
        }

        //D3D11_DEPTH_STENCIL_DESC dsDesc;
        //dsDesc.DepthEnable = FALSE;
        //dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        //dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
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
        //    assert(!"デプスステンシルステートの作成に失敗(Primitive)");
        //    return;
        //}
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    Primitive::~Primitive()
    {
        //safe_release(depthStencilState);
        safe_release(rasterizerState);
        safe_release(buffer);
        ResourceManager::releasePixelShader(pixelShader);
        ResourceManager::releaseVertexShader(vertexShader, inputLayout);
    }

    //--------------------------------------------------------------
    //  四角形の描画
    //--------------------------------------------------------------
    void Primitive::rect(ID3D11DeviceContext* context, 
        const VECTOR2& pos, const VECTOR2& size,
        const VECTOR2& center, float angle, 
        const VECTOR4& color, float z
        ) const
    {
        if (size.x == 0.0f || size.y == 0.0f) return;

        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[4]{};
#ifdef GAMELIB_PLUS_UP
        vertices[0] = { VECTOR3(-0.0f, +1.0f, z), color };
        vertices[1] = { VECTOR3(+1.0f, +1.0f, z), color };
        vertices[2] = { VECTOR3(-0.0f, -0.0f, z), color };
        vertices[3] = { VECTOR3(+1.0f, -0.0f, z), color };
#else
        vertices[0] = { VECTOR3(-0.0f, -0.0f, z), color };
        vertices[1] = { VECTOR3(+1.0f, -0.0f, z), color };
        vertices[2] = { VECTOR3(-0.0f, +1.0f, z), color };
        vertices[3] = { VECTOR3(+1.0f, +1.0f, z), color };
#endif

        float sinValue = sinf(angle);
        float cosValue = cosf(angle);
        for (int i = 0; i < 4; i++) {
            vertices[i].position.x *= size.x;
            vertices[i].position.y *= size.y;
            vertices[i].position.x -= center.x;
            vertices[i].position.y -= center.y;

            float rx = vertices[i].position.x;
            float ry = vertices[i].position.y;
            vertices[i].position.x = rx * cosValue - ry * sinValue;
            vertices[i].position.y = rx * sinValue + ry * cosValue;
            vertices[i].position.x += pos.x;
            vertices[i].position.y += pos.y;

            vertices[i].position.x = -1.0f + vertices[i].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[i].position.y = -1.0f + vertices[i].position.y * 2 / viewport.Height;
#else
            vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;
#endif
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
        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(4, 0);
    }

    //--------------------------------------------------------------
    //  線の描画
    //--------------------------------------------------------------
    void Primitive::line(ID3D11DeviceContext* context,
        const VECTOR2& from, const VECTOR2& to,
        const VECTOR4& color, float width, float z
        ) const
    {
        if (width <= 0.0f) return;

        VECTOR2 v1(from.x, from.y);
        VECTOR2 v2(to.x, to.y);
        float w = vec2Length(v2 - v1);
        float h = width;
        float cx = w * 0.5f;
        float cy = h * 0.5f;
        float x = (from.x + to.x) * 0.5f;
        float y = (from.y + to.y) * 0.5f;
        float angle = atan2(to.y - from.y, to.x - from.x);

        rect(context, VECTOR2(x, y), VECTOR2(w, h), VECTOR2(cx, cy), angle, color, z);
    }

    //--------------------------------------------------------------
    //  円の描画
    //--------------------------------------------------------------
    void Primitive::circle(ID3D11DeviceContext* context,
        const VECTOR2& center, float radius, 
        const VECTOR2& scale, float angle, 
        const VECTOR4& color, int n, float z
        ) const
    {
        if (n < 3 || radius <= 0.0f) return;
        if (n > 64) n = 64;//最大64角形

        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[130] = { VECTOR3(0,0,0) };
        float arc = DirectX::XM_PI * 2 / n;
        vertex *p = &vertices[0];

        float sinValue = sinf(angle);
        float cosValue = cosf(angle);
        for (int i = 0; i <= n; i++)
        {
            float rx, ry;
#ifdef GAMELIB_PLUS_UP
            rx = cosf(arc * i) * radius * scale.x;
            ry = sinf(arc * i) * radius * scale.y;
#else
            rx = cosf(arc * -i) * radius * scale.x;
            ry = sinf(arc * -i) * radius * scale.y;
#endif
            p->position.x = center.x + rx * cosValue - ry * sinValue;
            p->position.y = center.y + rx * sinValue + ry * cosValue;

            p->position.x = -1.0f + p->position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            p->position.y = -1.0f + p->position.y * 2 / viewport.Height;
#else
            p->position.y = 1.0f - p->position.y * 2 / viewport.Height;
#endif
            p->position.z = z;
            p->color = color;
            p++;

            p->position.x = center.x;
            p->position.y = center.y;

            p->position.x = -1.0f + p->position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            p->position.y = -1.0f + p->position.y * 2 / viewport.Height;
#else
            p->position.y = 1.0f - p->position.y * 2 / viewport.Height;
#endif
            p->position.z = z;
            p->color = color;
            p++;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, vertices, sizeof(vertex) * (n + 1) * 2);
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, NULL, 0);
        context->PSSetShader(pixelShader, NULL, 0);
        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw((n + 1) * 2 - 1, 0);
    }

    //--------------------------------------------------------------
    //  四角ポリゴンの描画
    //--------------------------------------------------------------
    void Primitive::quad(ID3D11DeviceContext* context,
        const VECTOR2(&v)[4], const VECTOR4& color, float z) const
    {
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[4]{};
#ifdef GAMELIB_PLUS_UP
        vertices[0] = { VECTOR3(v[3].x, v[3].y, z), color };
        vertices[1] = { VECTOR3(v[2].x, v[2].y, z), color };
        vertices[2] = { VECTOR3(v[0].x, v[0].y, z), color };
        vertices[3] = { VECTOR3(v[1].x, v[1].y, z), color };
#else
        vertices[0] = { VECTOR3(v[0].x, v[0].y, z), color };
        vertices[1] = { VECTOR3(v[1].x, v[1].y, z), color };
        vertices[2] = { VECTOR3(v[3].x, v[3].y, z), color };
        vertices[3] = { VECTOR3(v[2].x, v[2].y, z), color };
#endif

        for (int i = 0; i < 4; i++)
        {
            vertices[i].position.x = -1.0f + vertices[i].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[i].position.y = -1.0f + vertices[i].position.y * 2 / viewport.Height;
#else
            vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;
#endif
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
        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(4, 0);
    }

    void Primitive::draw_triangle_fan(ID3D11DeviceContext* context,
        const VECTOR2 v[], const VECTOR4& color, int nPolygon, float z) const
    {
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);
        vertex vertices[PRIMITIVE_VERTEX_NUM] = {};
        int index = 0;
        for (int i = 0; i < nPolygon; i++)
        {
            vertices[index].position = v[0];
            vertices[index].position.x = -1.0f + vertices[index].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].position.y = -1.0f + vertices[index].position.y * 2 / viewport.Height;
#else
            vertices[index].position.y = 1.0f - vertices[index].position.y * 2 / viewport.Height;
#endif
            vertices[index].position.z = z;
            vertices[index].color = color;
            index++;

            vertices[index].position = v[i + 1];
            vertices[index].position.x = -1.0f + vertices[index].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].position.y = -1.0f + vertices[index].position.y * 2 / viewport.Height;
#else
            vertices[index].position.y = 1.0f - vertices[index].position.y * 2 / viewport.Height;
#endif
            vertices[index].color = color;
            index++;

            vertices[index].position = v[i + 2];
            vertices[index].position.x = -1.0f + vertices[index].position.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].position.y = -1.0f + vertices[index].position.y * 2 / viewport.Height;
#else
            vertices[index].position.y = 1.0f - vertices[index].position.y * 2 / viewport.Height;
#endif
            vertices[index].position.z = z;
            vertices[index].color = color;
            index++;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, vertices, sizeof(vertex) * index);
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, NULL, 0);
        context->PSSetShader(pixelShader, NULL, 0);
        //context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(nPolygon * 3, 0);
    }

    //******************************************************************************
    //
    //      PrimitiveBatchクラス
    //
    //******************************************************************************

    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    PrimitiveBatch::PrimitiveBatch(ID3D11Device* device, size_t maxInstance) {
        MAX_INSTANCES = maxInstance;

        //VertexBufferの作成
        vertex vertices[4]{};
#ifdef GAMELIB_PLUS_UP
        vertices[0] = { VECTOR3(0, 0, 0) };
        vertices[1] = { VECTOR3(1, 0, 0) };
        vertices[2] = { VECTOR3(0, 1, 0) };
        vertices[3] = { VECTOR3(1, 1, 0) };
#else
        vertices[0] = { VECTOR3(0, 1, 0) };
        vertices[1] = { VECTOR3(1, 1, 0) };
        vertices[2] = { VECTOR3(0, 0, 0) };
        vertices[3] = { VECTOR3(1, 0, 0) };
#endif

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
            assert(!"頂点バッファの作成に失敗(PrimitiveBatch)");
            return;
        }

        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION",	   0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,	  0 },
            { "NDC_TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "NDC_TRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
            { "COLOR",		   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        };
        UINT numElements = ARRAYSIZE(layout);
        if (!ResourceManager::loadVertexShader(device, "./Data/Shaders/primitive_batch_vs.cso", layout, numElements, &vertexShader, &inputLayout))
        {
            assert(!"頂点シェーダーの読み込みに失敗(PrimitiveBatch)");
            return;
        }
        if (!ResourceManager::loadPixelShader(device, "./Data/Shaders/primitive_batch_ps.cso", &pixelShader))
        {
            assert(!"ピクセルシェーダーの作成に失敗(PrimitiveBatch)");
            return;
        }

        instance *inst = new instance[MAX_INSTANCES];
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
                assert(!"バッファの作成に失敗(PrimitiveBatch)");
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
            assert(!"ラスタライザステートの作成に失敗(PrimitiveBatch)");
            return;
        }

        //D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        //depthStencilDesc.DepthEnable = FALSE;
        //depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        //depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        //depthStencilDesc.StencilEnable = FALSE;
        //depthStencilDesc.StencilReadMask = 0xFF;
        //depthStencilDesc.StencilWriteMask = 0xFF;
        //depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        //depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        //depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        //depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        //depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        //depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        //depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        //depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        //if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState)))
        //{
        //    assert(!"デプスステンシルステートの作成に失敗(PrimitiveBatch)");
        //    return;
        //}
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    PrimitiveBatch::~PrimitiveBatch()
    {
        safe_release(instanceBuffer);
        //safe_release(depthStencilState);
        safe_release(rasterizerState);
        safe_release(buffer);
        ResourceManager::releasePixelShader(pixelShader);
        ResourceManager::releaseVertexShader(vertexShader, inputLayout);
    }

    //--------------------------------------------------------------
    //  前処理（描画前に1度呼ぶ）
    //--------------------------------------------------------------
    void PrimitiveBatch::begin(ID3D11DeviceContext* context)
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
    //	四角形の描画
    //--------------------------------------------------------------
    void PrimitiveBatch::rect(const VECTOR2& pos, const VECTOR2& size, const VECTOR2& center, float angle, const VECTOR4& color, float z)
    {
        _ASSERT_EXPR(instanceCount < MAX_INSTANCES, L"Number of instances must be less than MAX_INSTANCES.");
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

        instances[instanceCount].ndcTransform._11 = w * size.x * c;
        instances[instanceCount].ndcTransform._21 = h * size.x * s;
        instances[instanceCount].ndcTransform._31 = 0.0f;
        instances[instanceCount].ndcTransform._41 = 0.0f;
        instances[instanceCount].ndcTransform._12 = w * size.y * -s;
        instances[instanceCount].ndcTransform._22 = h * size.y * c;
        instances[instanceCount].ndcTransform._32 = 0.0f;
        instances[instanceCount].ndcTransform._42 = 0.0f;
        instances[instanceCount].ndcTransform._13 = 0.0f;
        instances[instanceCount].ndcTransform._23 = 0.0f;
        instances[instanceCount].ndcTransform._33 = 1.0f;
        instances[instanceCount].ndcTransform._43 = 0.0f;
        instances[instanceCount].ndcTransform._14 = w * (-center.x * c + -center.y * -s + pos.x) - 1.0f;
        instances[instanceCount].ndcTransform._24 = h * (-center.x * s + -center.y *  c + pos.y) + 1.0f;
        instances[instanceCount].ndcTransform._34 = z;
        instances[instanceCount].ndcTransform._44 = 1.0f;

#ifdef GAMELIB_PLUS_UP
        instances[instanceCount].ndcTransform._21 = -instances[instanceCount].ndcTransform._21;
        instances[instanceCount].ndcTransform._22 = -instances[instanceCount].ndcTransform._22;
        instances[instanceCount].ndcTransform._24 = -instances[instanceCount].ndcTransform._24;
#endif

#endif
        instances[instanceCount].color = color;

        instanceCount++;
    }

    //--------------------------------------------------------------
    //  線の描画
    //--------------------------------------------------------------
    void PrimitiveBatch::line(const VECTOR2& from, const VECTOR2& to, const VECTOR4& color, float width, float z)
    {
        if (width <= 0.0f) return;
        VECTOR2 v1(from.x, from.y);
        VECTOR2 v2(to.x, to.y);
        float w = vec2Length(v2 - v1);
        float h = width;
        float cx = w * 0.5f;
        float cy = h * 0.5f;
        float x = (from.x + to.x) * 0.5f;
        float y = (from.y + to.y) * 0.5f;
        float angle = atan2(to.y - from.y, to.x - from.x);

        rect(VECTOR2(x, y), VECTOR2(w, h), VECTOR2(cx, cy), angle, color, z);
    }

    //--------------------------------------------------------------
    //  後処理（描画後に1度呼ぶ）
    //--------------------------------------------------------------
    void PrimitiveBatch::end(ID3D11DeviceContext* context)
    {
        context->Unmap(instanceBuffer, 0);
        context->DrawInstanced(4, instanceCount, 0, 0);
    }

}

//******************************************************************************

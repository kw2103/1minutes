#pragma once

#include "game_lib.h"
#include <wrl.h>

class DepthStencil {
public:
    enum class MODE {
        NONE,       // 通常
        DEPTH,      // 深度
        MASK,       // マスク生成
        APPLY_MASK, // マスクに描画
        EXCLUSIVE,  // マスク以外に描画
        NUM         // モードの総数
    };

public:
    static DepthStencil& instance() {
        static DepthStencil inst(GameLib::DirectX11::getDevice());
        return inst;
    }
    void set(MODE mode);    // モードの設定
    void clear();           // マスクをクリアする

private:
    DepthStencil(ID3D11Device* device);
    ~DepthStencil() {}
    DepthStencil(const DepthStencil&) = delete; // コピーコンストラクタが存在しないことを明示

private:
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> modes[static_cast<size_t>(MODE::NUM)];
};

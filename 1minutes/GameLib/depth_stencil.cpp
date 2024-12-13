#include "depth_stencil.h"
#include "misc.h"

DepthStencil::DepthStencil(ID3D11Device* device)
	:modes() {
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
	HRESULT hr{ S_OK };

	// MODE::NONE
	depth_stencil_desc = {};
	depth_stencil_desc.DepthEnable = FALSE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.StencilEnable = FALSE;
	hr = device->CreateDepthStencilState(&depth_stencil_desc,
		modes[static_cast<size_t>(MODE::NONE)].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), GameLib::hr_trace(hr));

    // MODE::DEPTH
    depth_stencil_desc = {};
    depth_stencil_desc.DepthEnable = TRUE;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL; // Allow writing to the depth buffer.
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; // Only pass if the current depth value is greater than or equal to the stored depth value.
    depth_stencil_desc.StencilEnable = FALSE; // Stencil test disabled for simplicity.
    hr = device->CreateDepthStencilState(&depth_stencil_desc,
        modes[static_cast<size_t>(MODE::DEPTH)].GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), GameLib::hr_trace(hr));

	// MODE::MASK
	depth_stencil_desc = {};
	depth_stencil_desc.DepthEnable = TRUE;//深度テストを行う
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;//深度値を書き込まない
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;//深度テストに必ず失敗する（描画はしないがステンシル値は書き込む）
	depth_stencil_desc.StencilEnable = TRUE;//ステンシルテストを行う
	depth_stencil_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_stencil_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;//ステンシルテストには必ず合格
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE;//深度テストに失敗してステンシルテストに成功した場合
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	hr = device->CreateDepthStencilState(&depth_stencil_desc,
		modes[static_cast<size_t>(MODE::MASK)].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), GameLib::hr_trace(hr));

	// MODE::APPLY_MASK
	depth_stencil_desc = {};
	depth_stencil_desc.DepthEnable = TRUE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;//Turn off writes to the depth-stencil buffer.
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;//Never pass the comparison.
	depth_stencil_desc.StencilEnable = TRUE;
	depth_stencil_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_stencil_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	hr = device->CreateDepthStencilState(&depth_stencil_desc,
		modes[static_cast<size_t>(MODE::APPLY_MASK)].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), GameLib::hr_trace(hr));

	// MODE::EXCLUSIVE
	depth_stencil_desc = {};
	depth_stencil_desc.DepthEnable = TRUE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;//Turn off writes to the depth-stencil buffer.
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;//Never pass the comparison.
	depth_stencil_desc.StencilEnable = TRUE;
	depth_stencil_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_stencil_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_EQUAL;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	hr = device->CreateDepthStencilState(&depth_stencil_desc,
		modes[static_cast<size_t>(MODE::EXCLUSIVE)].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), GameLib::hr_trace(hr));
}

// モードをセットする
void DepthStencil::set(MODE mode) {
	GameLib::DirectX11::getContext()->OMSetDepthStencilState(
		modes[static_cast<size_t>(mode)].Get(), mode == MODE::MASK ? 1 : 0);
}

// マスクをクリアする
void DepthStencil::clear() {
	GameLib::clearStencil();
}

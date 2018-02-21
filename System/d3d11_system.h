#pragma once

#include <ecs.h>

template<class T>
constexpr T pi_ = 3.14159265358979323846f;

template<class T>
void SafeRelease(T *& p)
{
	if (p)
		p->Release();

	p = nullptr;
}

struct VT
{
	ID3D11RenderTargetView * rtv_ = nullptr;
	ID3D11Texture2D * rtv_tex_ = nullptr;
	ID3D11DepthStencilView * dsv_ = nullptr;
	ID3D11Texture2D * dsv_tex_ = nullptr;
	ID3D11ShaderResourceView * srv_ = nullptr;
};

struct TEX_VERTEX
{
	D3DXVECTOR3 position_;
	D3DXVECTOR2 texcoord_;
};

struct SHADER
{
	ID3D11VertexShader * vs_;
	ID3D11GeometryShader * gs_;
	ID3D11PixelShader * ps_;
	ID3D11InputLayout * il_;
	ID3D11Buffer * cb_[10];
};

struct POLYGON
{
	SHADER shader_;
	VT vt_;
	ID3D11Buffer * vb_ = nullptr;
};

class D3D11System final : public System
{
private:
	IDXGISwapChain * swap_chain_ = nullptr;
	ID3D11Device * device_ = nullptr;
	ID3D11DeviceContext * context_ = nullptr;
	VT back_buffer_;
	POLYGON screen_;
	
public:
	D3D11System(void)
	{
		DXGI_SWAP_CHAIN_DESC sd;
		memset(&sd, 0, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = window::size::width_<unsigned int>;
		sd.BufferDesc.Height = window::size::height_<unsigned int>;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = Game::Get<WindowSystem>()->hwnd_;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;

		D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL* feature_level = nullptr;

		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			0, &feature_levels, 1, D3D11_SDK_VERSION, &sd, &this->swap_chain_, &this->device_,
			feature_level, &this->context_);

		this->swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&this->back_buffer_.rtv_tex_);

		this->device_->CreateRenderTargetView(this->back_buffer_.rtv_tex_, nullptr, &this->back_buffer_.rtv_);
		SafeRelease(this->back_buffer_.rtv_tex_);

		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = window::size::width_<unsigned int>;
		descDepth.Height = window::size::height_<unsigned int>;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		
		this->device_->CreateTexture2D(&descDepth, nullptr, &this->back_buffer_.dsv_tex_);
		this->device_->CreateDepthStencilView(this->back_buffer_.dsv_tex_, nullptr, &this->back_buffer_.dsv_);
		this->context_->OMSetRenderTargets(1, &this->back_buffer_.rtv_, this->back_buffer_.dsv_);

		D3D11_TEXTURE2D_DESC desc;
		memset(&desc, 0, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = window::size::width_<unsigned int>;
		desc.Height = window::size::height_<unsigned int>;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		this->device_->CreateTexture2D(&desc, nullptr, &this->screen_.vt_.rtv_tex_);

		D3D11_RENDER_TARGET_VIEW_DESC rdesc;
		memset(&rdesc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rdesc.Texture2DArray.MipSlice = 0;
		this->device_->CreateRenderTargetView(this->screen_.vt_.rtv_tex_, &rdesc, &this->screen_.vt_.rtv_);

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		memset(&srv_desc, 0, sizeof(srv_desc));
		srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
		this->device_->CreateShaderResourceView(this->screen_.vt_.rtv_tex_, &srv_desc, &this->screen_.vt_.srv_);

		descDepth.Width = window::size::width_<unsigned int>;
		descDepth.Height = window::size::height_<unsigned int>;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		this->device_->CreateTexture2D(&descDepth, nullptr, &this->screen_.vt_.dsv_tex_);

		this->device_->CreateDepthStencilView(this->screen_.vt_.dsv_tex_, nullptr, &this->screen_.vt_.dsv_);

		TEX_VERTEX VertexData[] =
		{
			D3DXVECTOR3(-1.f,-1.f,0),D3DXVECTOR2(0,1),
			D3DXVECTOR3(-1.f,+1.f,0),D3DXVECTOR2(0,0),
			D3DXVECTOR3(+1.f,-1.f,0),D3DXVECTOR2(1,1),
			D3DXVECTOR3(+1.f,+1.f,0),D3DXVECTOR2(1,0)
		};

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(TEX_VERTEX) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA init_data;
		init_data.pSysMem = VertexData;
		this->device_->CreateBuffer(&bd, &init_data, &this->screen_.vb_);

		ID3DBlob *compiled_shader = nullptr;
		ID3DBlob *errors = nullptr;
		D3DX11CompileFromFileA("RadialBlur.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, nullptr, &compiled_shader, &errors, nullptr);
		SafeRelease(errors);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = sizeof(layout) / sizeof(layout[0]);

		this->device_->CreateInputLayout(layout, numElements, compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), &this->screen_.shader_.il_);
		this->device_->CreateVertexShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &this->screen_.shader_.vs_);
		SafeRelease(compiled_shader);
		D3DX11CompileFromFileA("RadialBlur.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, nullptr, &compiled_shader, &errors, nullptr);
		SafeRelease(errors);
		this->device_->CreatePixelShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &this->screen_.shader_.ps_);
		SafeRelease(compiled_shader);

		D3D11_VIEWPORT vp;
		vp.Width = window::size::width_<float>;
		vp.Height = window::size::height_<float>;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		this->context_->RSSetViewports(1, &vp);
		//ラスタライズ設定
		D3D11_RASTERIZER_DESC rdc;
		memset(&rdc, 0, sizeof(rdc));
		rdc.CullMode = D3D11_CULL_NONE;
		rdc.FillMode = D3D11_FILL_SOLID;
		ID3D11RasterizerState * rs = nullptr;
		this->device_->CreateRasterizerState(&rdc, &rs);
		this->context_->RSSetState(rs);
		SafeRelease(rs);
	}
	~D3D11System(void)
	{
		SafeRelease(this->swap_chain_);
		SafeRelease(this->back_buffer_.rtv_tex_);
		SafeRelease(this->back_buffer_.dsv_);
		SafeRelease(this->back_buffer_.dsv_tex_);
		SafeRelease(this->device_);
	}

private:
	void $Begin(void) override {};
	void $Run(void) override
	{
		D3DXMATRIX mView;
		D3DXMATRIX mProj;
		D3DXCOLOR clear_color = { .2f, .4f, .8f, 1.f };

		//1パス目 シーンを画面ポリゴンテクスチャー上にレンダリング
		{
			this->context_->OMSetRenderTargets(1, &this->screen_.vt_.rtv_, this->screen_.vt_.dsv_);

			this->context_->ClearRenderTargetView(this->screen_.vt_.rtv_, clear_color);
			this->context_->ClearDepthStencilView(this->screen_.vt_.dsv_, D3D11_CLEAR_DEPTH, 1.0f, 0);				

			D3DXVECTOR3 vEyePt(0.0f, 0.0f, -4.0f);
			D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
			D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
			D3DXMatrixPerspectiveFovLH(&mProj, pi_<float> / 4, 1.w / 1.h, 0.1f, 1000.0f);
			//レンダリング
			/*m_pMesh->Render(mView, mProj, D3DXVECTOR3(1, 1, -1), vEyePt);
			m_pMesh->m_fYaw += 0.001;*/
		}

		//2パス目 画面ポリゴンを通常のレンダーターゲット上にレンダリング
		{
			this->context_->OMSetRenderTargets(1, &this->back_buffer_.rtv_, this->back_buffer_.dsv_);

			this->context_->ClearRenderTargetView(this->back_buffer_.rtv_, clear_color);
			this->context_->ClearDepthStencilView(this->back_buffer_.dsv_, D3D11_CLEAR_DEPTH, 1.0f, 0);

			this->context_->VSSetShader(this->screen_.shader_.vs_, nullptr, 0);
			this->context_->PSSetShader(this->screen_.shader_.ps_, nullptr, 0);

			this->context_->PSSetShaderResources(0, 1, &this->screen_.vt_.srv_);

			this->context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			UINT stride = sizeof(TEX_VERTEX);
			UINT offset = 0;

			this->context_->IASetInputLayout(this->screen_.shader_.il_);
			this->context_->IASetVertexBuffers(0, 1, &this->screen_.vb_, &stride, &offset);
			this->context_->Draw(4, 0);
		}

		this->swap_chain_->Present(0, 0);
	};
	void $End(void) override {};
};
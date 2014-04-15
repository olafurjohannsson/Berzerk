
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message("Header Trace: BRZ_Display.h")
#endif

#include "BRZ_Define.h"
#include <Windows.h>

namespace BRZ
{
	class Vertex
	{
	public:
		Vertex()		{ BRZ::ZeroMem(&pos);		}
		~Vertex()		{							}


	public:
		DirectX::XMFLOAT3	pos;
	};


	struct ShaderConstants
	{
	public:
		ShaderConstants()	{ BRZ::ZeroMem(this);		}
		~ShaderConstants()	{ 							}

	public:
		BRZMATRIX		world;
		BRZMATRIX		view;
		BRZMATRIX		proj;
	};


	template <class T_type>
	class D3DResource
	{
	public:
		D3DResource() : ptr(NULL)					{	 }
		~D3DResource()								{ if (ptr != NULL) { ptr->Release(); }	}

		inline operator const T_type *() const		{ return ptr;		}
		inline operator T_type *()					{ return ptr;		}

		inline T_type * operator->()				{ return ptr;		}
		inline const T_type * operator->() const	{ return ptr;		}	

		inline T_type ** operator&()				{ return &ptr;		}
		inline const T_type ** operator&() const	{ return &ptr;		}

	private:
		T_type * ptr;
	};


	class Display
	{
	public:
		Display(std::ofstream & logOut);
		~Display();

	public:
		BRZRESULT Initialize(HWND window);
		BRZRESULT GenerateGeo();
		BRZRESULT LoadShaders();
		BRZRESULT Render(float xPos, float yPos, float rot);

	private:
		// Video Card Info;
		bool		vSync;
		int			vRam;
		BRZCHAR		vName[128];

		HWND		window;
		bool		fullScreen;

		BRZ::D3DResource<IDXGISwapChain>			chain;
		BRZ::D3DResource<ID3D11Device>				device;
		BRZ::D3DResource<ID3D11DeviceContext>		context;
		BRZ::D3DResource<ID3D11RenderTargetView>	renderTarget;
		BRZ::D3DResource<ID3D11Texture2D>			depthStencilBuffer;
		BRZ::D3DResource<ID3D11DepthStencilState>	depthStencilState;
		BRZ::D3DResource<ID3D11DepthStencilView>	depthStencilView;
		BRZ::D3DResource<ID3D11RasterizerState>		rasterState;

		BRZMATRIX		view;
		BRZMATRIX		projection;
		BRZMATRIX		world;

		std::ofstream & log;


		// Object Testing Code
		BRZ::D3DResource<ID3D11Buffer>	vertex;
		BRZ::D3DResource<ID3D11Buffer>	index;

		unsigned int					indexCount;
		unsigned int					vertCount;
		unsigned int					lineCount;

		// Shader Testing Code
		BRZ::D3DResource<ID3D11VertexShader>	vShader;
		BRZ::D3DResource<ID3D11PixelShader>		pShader;
		BRZ::D3DResource<ID3D11InputLayout>		inLayout;
		BRZ::D3DResource<ID3D11Buffer>			matBuffer;
	};
}

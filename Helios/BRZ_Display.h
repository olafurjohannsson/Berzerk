
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message("Header Trace: BRZ_Display.h")
#endif

#include "BRZ_Define.h"
#include <Windows.h>

#include "BRZ_ASEImport.h"
#include "BRZ_Colour.h"
#include "BRZ_Coord2.h"


namespace BRZ
{
	const unsigned int	WND_HEIGHT = 600;
	const unsigned int	WND_WIDTH = 800;



	class Vertex
	{
	public:
		Vertex()		{ BRZ::ZeroMem(&pos);	}
		~Vertex()		{						}

	public:
		DirectX::XMFLOAT3	pos;
	};



	struct ShaderConstants
	{
	public:
		ShaderConstants()	{	world = DirectX::XMMatrixIdentity();
								view = DirectX::XMMatrixIdentity();
								proj = DirectX::XMMatrixIdentity();		}
		~ShaderConstants()	{ 							}

	public:
		BRZMATRIX			world;
		BRZMATRIX			view;
		BRZMATRIX			proj;
		DirectX::XMFLOAT4	col;
	};


	struct RenderTicket
	{
		RenderTicket() :
		ptOffset(0),
		ptCount(0),
		idxOffset(0),
		idxCount(0)
		{ 	}

		BRZ::Colour		colour;
		unsigned int	ptOffset;
		unsigned int	ptCount;
		unsigned int	idxOffset;
		unsigned int	idxCount;
		DirectX::XMFLOAT4X4		transform;
	};

	struct ParticleTicket
	{
		ParticleTicket() { BRZ::ZeroMem(this);	}

		BRZ::Colour				colour;
		DirectX::XMFLOAT4X4		transform;
	};



	template <class T_type>
	class D3DResource
	{
	public:
		D3DResource() : ptr(NULL)					{	 }
		~D3DResource()								{ if (ptr != NULL) { ptr->Release(); }	}

		// Undefined copy constructor -- this is not available for use:
		D3DResource(const D3DResource<T_type> & ref);

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

		// Undefined copy constructor -- should never be used:
		Display(const Display & ref);

	public:
		BRZRESULT Initialize(HWND window);

		BRZRESULT SetDataDirectory(const BRZSTRING & dir);
		BRZRESULT LoadGeometry(const BRZSTRING & file, const BRZSTRING & name);
		BRZRESULT BakeGeometry(const BRZ::RawGeometry & geometry, const BRZSTRING & name);
		BRZRESULT LockGeometry();

		BRZRESULT Link(BRZ::LineObject & object, const BRZSTRING & objName);
		BRZRESULT Queue(const BRZ::LineElement & element, const DirectX::XMFLOAT4X4 & transform, const BRZ::Colour & colour);
		BRZRESULT QueueParticle(const BRZ::Vec2 & position, const BRZ::Colour & colour);
		BRZRESULT Render();

		// For testing purposes:
		BRZRESULT GenerateGrid();

	protected:
		// Internal sub-routines -- called during initialization:
		BRZRESULT InitD3DDevice();
		BRZRESULT InitD3DView();
		BRZRESULT InitPipeline(unsigned int maxTickets);
		BRZRESULT InitCache(unsigned int maxPoints, unsigned int maxLines);
		BRZRESULT InitLibrary(unsigned int maxElements, unsigned int maxTemplates);

		BRZRESULT LoadShaderRaw(const BRZSTRING & file, BRZ::D3DResource<ID3D10Blob> & blob) const;

	private:
		// Video card information:
		unsigned int	vc_ram;
		unsigned int	vc_sys;
		BRZCHAR			vc_name[128];

		// Display settings:
		HWND				wnd_handle;
		BRZ::Coord2			wnd_size;
		bool				wnd_vSync;
		bool				wnd_fullScreen;
		std::ofstream &		wnd_log;

		// Direct3D objects [General]:
		BRZ::D3DResource<IDXGISwapChain>			d3d_swapChain;
		BRZ::D3DResource<ID3D11Device>				d3d_device;
		BRZ::D3DResource<ID3D11DeviceContext>		d3d_context;
		BRZ::D3DResource<ID3D11RenderTargetView>	d3d_renderTarget;
		BRZ::D3DResource<ID3D11Texture2D>			d3d_depthStencilBuffer;
		BRZ::D3DResource<ID3D11DepthStencilState>	d3d_depthStencilState;
		BRZ::D3DResource<ID3D11DepthStencilView>	d3d_depthStencilView;
		BRZ::D3DResource<ID3D11RasterizerState>		d3d_rasterState;
		BRZ::D3DResource<ID3D11BlendState>			d3d_blendState;

		// Direct3D objects [Pipeline]:
		// BRZ::ShaderConstants					out_constants;
		DirectX::XMFLOAT4X4						out_view;
		DirectX::XMFLOAT4X4						out_projection;
		BRZ::D3DResource<ID3D11VertexShader>	out_vertexShader;
		BRZ::D3DResource<ID3D11PixelShader>		out_pixelShader;
		BRZ::D3DResource<ID3D11InputLayout>		out_inputLayout;
		BRZ::D3DResource<ID3D11Buffer>			out_shaderInput;
		unsigned int							out_maxTickets;
		unsigned int							out_usedTickets;
		BRZ::RenderTicket *						out_queue;
		std::vector<BRZ::ParticleTicket>		out_particles;


		// Geometry cache objects:
		BRZSTRING			cache_dir;
		BRZ::ASEImport		cache_importer;
		unsigned int		cache_maxPoints;
		unsigned int		cache_maxLines;
		unsigned int		cache_usedPoints;
		unsigned int		cache_usedLines;
		BRZ::Vertex *		cache_points;
		BRZ::Coord2 *		cache_lines;
		bool				cache_locked;

		// Direct3D objects [Geometry]:
		BRZ::D3DResource<ID3D11Buffer>	geo_points;
		BRZ::D3DResource<ID3D11Buffer>	geo_indices;
		unsigned int					geo_totalPoints;
		unsigned int					geo_totalIndices;
		unsigned int					geo_totalLines;

		// Template library objects:
		unsigned int			obj_maxElements;
		unsigned int			obj_maxTemplates;
		unsigned int			obj_usedElements;
		unsigned int			obj_usedTemplates;
		BRZ::LineElement *		obj_elements;
		BRZ::LineTemplate *		obj_templates;
	};
}

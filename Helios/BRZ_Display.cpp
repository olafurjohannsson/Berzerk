
#include "BRZ_Display.h"

#include "BRZ_AsteroidGen.h"
#include "BRZ_Colour.h"
#include "BRZ_LineElement.h"
#include "BRZ_LineObject.h"
#include "BRZ_LineTemplate.h"
#include "BRZ_RawElement.h"
#include "BRZ_RawGeometry.h"

// Required for ID3D10Blob:
#include <d3d10.h>


namespace BRZ
{
	std::ofstream *		out = NULL;
	HRESULT *			check = NULL;
	std::string			prefix;

	bool Test(const std::string & A_msg)
	{
		if (FAILED(*check))
		{
			*out << prefix << A_msg << std::endl;
			out->flush();
			return false;
		}

		return true;
	}

	void Log(const std::string & A_msg)
	{
		*out << prefix << A_msg << std::endl;
		out->flush();
	}
}


BRZRESULT BRZ::Display::SetDataDirectory(const BRZSTRING & A_dir)
{
	cache_dir = A_dir;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::LoadGeometry(const BRZSTRING & A_file, const BRZSTRING & A_name)
{
	BRZ::RawGeometry	geo;

	// Set up testing variables:
	BRZ::prefix = "[Display::LoadGeometry]: ";

	BRZSTRING file = cache_dir + A_file;

	if (cache_importer.Import(file) != BRZ_SUCCESS)
	{
		BRZ::Log("Failed to import ASE file.");
		return BRZ_FAILURE;
	}

	cache_importer.CreateGeometry(geo);

	return this->BakeGeometry(geo, A_name);
}


BRZRESULT BRZ::Display::GenerateGrid()
{
	BRZ::RawGeometry	geo;

	geo.count = 1;
	geo.elem = new BRZ::RawElement[1];

	geo.elem[0].colour = BRZ::Colour(0.0f, 0.0f, 0.0f, 0.5f);
	geo.elem[0].AllocPoints(52);
	geo.elem[0].AllocLines(26);

	float startX = -350.0f;
	float modX = 50.0f;

	for (unsigned int i = 0; i < 15; ++i)
	{
		geo.elem[0].points[i * 2] = BRZ::Vec2(startX + (i * modX), 295.0f);
		geo.elem[0].points[(i * 2) + 1] = BRZ::Vec2(startX + (i * modX), -295.0f);
		geo.elem[0].lines[i] = BRZ::Coord2(i * 2, (i * 2) + 1);
	}

	float startY = -250.0f;
	float modY = 50.0f;

	for (unsigned int i = 15; i < 26; ++i)
	{
		float offset = i - 15.0f;

		geo.elem[0].points[i * 2] = BRZ::Vec2(395.0f, startY + (offset * modY));
		geo.elem[0].points[(i * 2) + 1] = BRZ::Vec2(-395.0f, startY + (offset * modY));
		geo.elem[0].lines[i] = BRZ::Coord2(i * 2, (i * 2) + 1);
	}

	this->BakeGeometry(geo, L"grid");

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::Link(BRZ::LineObject & A_obj, const BRZSTRING & A_name)
{
	BRZ::prefix = "[Display::Link]: ";


	if (A_obj.display != NULL)
	{
		BRZ::Log("Error: Object previously linked.");
		return BRZ_FAILURE;
	}


	for (unsigned int i = 0; i < obj_maxTemplates; ++i)
	{
		if (obj_templates[i].name == A_name)
		{
			const BRZ::LineTemplate * temp = &obj_templates[i];
			A_obj.display = this;

			if (temp->numElements == 0)
				return BRZ_SUCCESS;

			A_obj.numElements = temp->numElements;
			A_obj.elements = new BRZ::LineObject::Reference[temp->numElements];

			for (unsigned int j = 0; j < temp->numElements; ++j)
			{
				const BRZ::LineElement * elem = &obj_elements[temp->idElements[j]];
				A_obj.elements[j].base = elem;
				A_obj.elements[j].visible = true;
				A_obj.elements[j].colour = elem->colour;
				A_obj.elements[j].rotation = 0.0f;
				A_obj.elements[j].offset = elem->localPos;
			}

			return BRZ_SUCCESS;
		}
	}

	std::string msg = "Could not find object (";
	std::string msg_end = ") for linking.";

	BRZ::Log(msg + BRZ::Narrow(A_name) + msg_end);

	return BRZ_FAILURE;
}


BRZRESULT BRZ::Display::Queue(const BRZ::LineElement & A_elem, const DirectX::XMFLOAT4X4 & A_trans, const BRZ::Colour & A_col)
{
	// Set up testing variables:
	BRZ::prefix = "[Display::Queue]: ";

	if (out_usedTickets == out_maxTickets)
	{
		BRZ::Log("No available tickets.");
		return BRZ_FAILURE;
	}

	BRZ::RenderTicket * target = &out_queue[out_usedTickets];
	target->colour = A_col;
	target->idxCount = A_elem.idxCount;
	target->idxOffset = A_elem.idxOffset;
	target->ptCount = A_elem.ptCount;
	target->ptOffset = A_elem.ptOffset;
	target->transform = A_trans;

	++out_usedTickets;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::LockGeometry()
{
	HRESULT					result;
	D3D11_BUFFER_DESC		vertexBufferDesc;
	D3D11_BUFFER_DESC		indexBufferDesc;
    D3D11_SUBRESOURCE_DATA	vertexData;
	D3D11_SUBRESOURCE_DATA	indexData;
	unsigned int			vertexCount = cache_usedPoints;
	unsigned int			indexCount = cache_usedLines * 2;
	unsigned int *			indices = NULL;
	
	if (indexCount > 0)
		indices = new unsigned int[indexCount];

	// Set up logging variables:
	BRZ::prefix = "[Display::LockGeometry]: ";
	BRZ::check = &result;

	// If cache is already locked, exit.
	if (cache_locked)
	{
		BRZ::Log("Cache already locked.");
		return BRZ_FAILURE;
	}


	// Set up the static vertex buffer:
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(BRZ::Vertex) * vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = cache_points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = d3d_device->CreateBuffer(&vertexBufferDesc, &vertexData, &geo_points);
	if (!BRZ::Test("Failed to create vertex buffer."))
	{
		delete [] indices;
		return BRZ_FAILURE;
	}



	// Set up the static index buffer:
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	for (unsigned int i = 0; i < cache_usedLines; ++i)
	{
		indices[2 * i] = cache_lines[i].x;
		indices[(2 * i) + 1] = cache_lines[i].y;
	}

    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = d3d_device->CreateBuffer(&indexBufferDesc, &indexData, &geo_indices);
	if (!BRZ::Test("Failed to create index buffer."))
	{
		delete [] indices;
		return BRZ_FAILURE;
	}

	delete [] indices;
	cache_locked = true;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::BakeGeometry(const BRZ::RawGeometry & A_geo, const BRZSTRING & A_name)
{
	// Set up logging variables:
	BRZ::prefix = "[Display::BakeGeometry ~ ";
	std::string suffix = "]: ";
	BRZ::prefix = BRZ::prefix + BRZ::Narrow(A_name) + suffix;


	// Verify the cache is open for loading:
	if (cache_locked)
	{
		BRZ::Log("Cache is already locked; cannot bake geometry.");
		return BRZ_FAILURE;
	};

	// Verify the integrity of input geometry:
	if (A_geo.elem == NULL)
	{
		BRZ::Log("Geometry possesses no RawElement information.");
		return BRZ_FAILURE;
	}
	else if (A_geo.count == 0)
	{
		BRZ::Log("Geometry described as having no elements.");
		return BRZ_FAILURE;
	}


	unsigned int	totalPt = 0;
	unsigned int	totalLn = 0;


	// Determine total points and lines present in raw geometry -- make sure space exists in the cache:
	for (unsigned int i = 0; i < A_geo.count; ++i)
	{
		totalPt += A_geo.elem[i].pointCount;
		totalLn += A_geo.elem[i].lineCount;
	}

	if (totalPt > (cache_maxPoints - cache_usedPoints))
	{
		BRZ::Log("Cache does not possess enough free points.");
		return BRZ_FAILURE;
	}
	else if (totalLn > (cache_maxLines - cache_usedLines))
	{
		BRZ::Log("Cache does not possess enough free lines.");
		return BRZ_FAILURE;
	}
	
	// Make sure enough free elements & templates exist in the object library:
	if (A_geo.count > (obj_maxElements - obj_usedElements))
	{
		BRZ::Log("Library does not possess enough free elements.");
		return BRZ_FAILURE;
	}
	else if (obj_maxTemplates == obj_usedTemplates)
	{
		BRZ::Log("Library does not possess enough free templates.");
		return BRZ_FAILURE;
	}


	// Set up the LineTemplate to be filled out:
	BRZ::LineTemplate * temp = &obj_templates[obj_usedTemplates];
	temp->AllocElems(A_geo.count);

	BRZ::LineElement *	elem = NULL;
	BRZ::RawElement *	raw = NULL;
	BRZ::Coord2			idxLocal;

	// Iterate over all the elements present in the raw geometry, copy their info into
	//	the cache and update the related LineElement in the library:
	for (unsigned int i = 0; i < A_geo.count; ++i)
	{
		raw = &A_geo.elem[i];
		elem = &obj_elements[obj_usedElements];

		// Set colour and local position:
		elem->colour = raw->colour;
		elem->localPos = raw->offset;

		// Store the offset into the index buffer the indices will need to be adjusted by:
		idxLocal.x = cache_usedPoints;
		idxLocal.y = cache_usedPoints;

		// Copy over point data:
		elem->ptCount = raw->pointCount;
		elem->ptOffset = cache_usedPoints;
		for (unsigned int j = 0; j < raw->pointCount; ++j)
		{
			cache_points[cache_usedPoints].pos.x = raw->points[j].x;
			cache_points[cache_usedPoints].pos.y = raw->points[j].y;
			cache_points[cache_usedPoints].pos.z = 10.0f;
			++cache_usedPoints;
		}

		// Copy over line data:
		elem->idxCount = raw->lineCount * 2;
		elem->idxOffset = cache_usedLines * 2;
		for (unsigned int j = 0; j < raw->lineCount; ++j)
		{
			cache_lines[cache_usedLines] = raw->lines[j] + idxLocal;
			++cache_usedLines;
		}

		temp->idElements[i] = obj_usedElements;
		++obj_usedElements;
	}

	temp->name = A_name;
	++obj_usedTemplates;

	return BRZ_SUCCESS;
}



// BRZRESULT LockGeometry();


BRZRESULT BRZ::Display::Render()
{
	HRESULT						result;
    D3D11_MAPPED_SUBRESOURCE	mappedResource;
	BRZ::ShaderConstants *		dataPtr;
	BRZMATRIX					lView = DirectX::XMLoadFloat4x4(&out_view);
	BRZMATRIX					lProj = DirectX::XMLoadFloat4x4(&out_projection);
	BRZMATRIX					tView = DirectX::XMMatrixTranspose(lView); //DirectX::XMMatrixTranspose(out_constants.view);
	BRZMATRIX					tProj = DirectX::XMMatrixTranspose(lProj);
//	BRZ::Colour					bg(0.2f, 0.2f, 0.2f, 1.0f);
	unsigned int				stride = sizeof(BRZ::Vertex);
	unsigned int				offset = 0;
	float						bg[4]= {0.2f, 0.2f, 0.2f, 0.2f};

	// Set up testing variables:
	BRZ::prefix = "[Display::Render]: ";
	BRZ::check = &result;

	d3d_context->ClearRenderTargetView(d3d_renderTarget, bg);
	d3d_context->ClearDepthStencilView(d3d_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	// Since all geometry data is in one vertex and one index buffer, set those up now:
	d3d_context->IASetVertexBuffers(0, 1, &geo_points, &stride, &offset);
	d3d_context->IASetIndexBuffer(geo_indices, DXGI_FORMAT_R32_UINT, 0);
	d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	
	// For each render ticket in the queue, draw:
	for (unsigned int i = 0; i < out_usedTickets; ++i)
	{
		unsigned int j = i;//(out_usedTickets - 1) - i;
		// Set up the shader constant buffer:
		result = d3d_context->Map(out_shaderInput, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (!BRZ::Test("Failed to map shader constant buffer."))
			return BRZ_FAILURE;

		dataPtr = (BRZ::ShaderConstants *)mappedResource.pData;
		BRZMATRIX lWorld = DirectX::XMLoadFloat4x4(&out_queue[j].transform);
		dataPtr->world = DirectX::XMMatrixTranspose(lWorld);
		dataPtr->view = tView;
		dataPtr->proj = tProj;
		BRZ::CopyMem((float *)&dataPtr->col, out_queue[j].colour.ptr, 4);
		
		d3d_context->Unmap(out_shaderInput, 0);
		d3d_context->VSSetConstantBuffers(0, 1, &out_shaderInput);

		// Set the input layout and shaders:
		d3d_context->IASetInputLayout(out_inputLayout);
		d3d_context->VSSetShader(out_vertexShader, NULL, 0);
		d3d_context->PSSetShader(out_pixelShader, NULL, 0);

		// Draw lines:
		d3d_context->DrawIndexed(out_queue[j].idxCount, out_queue[j].idxOffset, 0);
	}


	if(wnd_vSync)
	{
		// Lock to screen refresh rate.
		d3d_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		d3d_swapChain->Present(0, 0);
	}

	out_usedTickets = 0;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::InitLibrary(unsigned int A_maxElem, unsigned int A_maxTem)
{
	if (A_maxElem == 0)
	{
		wnd_log << "[Display::InitLibrary]: Cannot initialize library with a maximum element count of 0." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}
	else if (A_maxTem == 0)
	{
		wnd_log << "[Display::InitLibrary]: Cannot initialize library with a maximum template count of 0." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	obj_elements = new BRZ::LineElement[A_maxElem];
	obj_maxElements = A_maxElem;
	obj_usedElements = 0;

	obj_templates = new BRZ::LineTemplate[A_maxTem];
	obj_maxTemplates = A_maxTem;
	obj_usedTemplates = 0;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::InitCache(unsigned int A_maxPt, unsigned int A_maxLn)
{
	if (A_maxPt == 0)
	{
		wnd_log << "[Display::InitCache]: Cannot initialize cache with a maximum point count of 0." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}
	else if (A_maxLn == 0)
	{
		wnd_log << "[Display::InitCache]: Cannot initialize cache with a maximum line count of 0." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	cache_points = new BRZ::Vertex[A_maxPt];
	cache_maxPoints = A_maxPt;
	cache_usedPoints = 0;

	cache_lines = new BRZ::Coord2[A_maxLn];
	cache_maxLines = A_maxLn;
	cache_usedLines = 0;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::InitPipeline(unsigned int A_maxTick)
{
	if (A_maxTick == 0)
	{
		wnd_log << "[Display::InitPipeline]: Cannot initialize pipeline with a ticket queue of size 0." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	HRESULT							result;
	BRZ::D3DResource<ID3D10Blob>	vertexShaderBuffer;
	BRZ::D3DResource<ID3D10Blob>	pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC		polygonLayout;
	unsigned int					numElements = 1;
	D3D11_BUFFER_DESC				matrixBufferDesc;
	unsigned int					screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	unsigned int					screenHeight = ::GetSystemMetrics(SM_CYSCREEN);


	// Set up the testing variables:
	BRZ::check = &result;
	BRZ::prefix = "[Display::InitPipeline]: ";




	// Set up shader constant matricies:
//	out_constants.world = DirectX::XMMatrixIdentity();

	if (wnd_fullScreen)
		DirectX::XMStoreFloat4x4(&out_projection, DirectX::XMMatrixOrthographicLH(screenWidth, screenHeight, 0.1f, 1000.0f));
	else
		DirectX::XMStoreFloat4x4(&out_projection, DirectX::XMMatrixOrthographicLH(BRZ::WND_WIDTH, BRZ::WND_HEIGHT, 0.1f, 1000.0f));

	DirectX::XMVECTORF32 eye = {0.0f, 0.0f, -10.0f };
	DirectX::XMVECTORF32 focus = { 0.0f, 0.0f, 100.0f };
	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMStoreFloat4x4(&out_view, DirectX::XMMatrixLookAtLH(eye, focus, up));





	// Load vertex and pixel shaders from disk:
	if (this->LoadShaderRaw(L"vs_default.cso", vertexShaderBuffer) != BRZ_SUCCESS)
	{
		wnd_log << "[Display::InitPipeline]: failed to load compiled vertex shader." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	result = d3d_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &out_vertexShader);
	if (!BRZ::Test("Could not create vertex shader for usage."))
		return BRZ_FAILURE;

	if (this->LoadShaderRaw(L"ps_default.cso", pixelShaderBuffer) != BRZ_SUCCESS)
	{
		wnd_log << "[Display::InitPipeline]: failed to load compiled pixel shader." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	result = d3d_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &out_pixelShader);
	if (!BRZ::Test("Could not create pixel shader for usage."))
		return BRZ_FAILURE;




	// Create the vertex input layout description.
	BRZ::ZeroMem(&polygonLayout);

	polygonLayout.SemanticName = "POSITION";
	polygonLayout.SemanticIndex = 0;
	polygonLayout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout.InputSlot = 0;
	polygonLayout.AlignedByteOffset = 0;
	polygonLayout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout.InstanceDataStepRate = 0;

	/*
	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	*/


	// Create the vertex input layout.
	result = d3d_device->CreateInputLayout(&polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
									   vertexShaderBuffer->GetBufferSize(), &out_inputLayout);
	if (!BRZ::Test("Failed to create vertex input layout."))
		return BRZ_FAILURE;





	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	BRZ::ZeroMem(&matrixBufferDesc);

    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(BRZ::ShaderConstants);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = d3d_device->CreateBuffer(&matrixBufferDesc, NULL, &out_shaderInput);
	if (!BRZ::Test("Failed to create shader constant buffer."))
		return BRZ_FAILURE;


	// Create the render ticket queue:
	out_maxTickets = A_maxTick;
	out_usedTickets = 0;
	out_queue = new BRZ::RenderTicket[A_maxTick];


	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::InitD3DDevice()
{
	HRESULT							result;
	BRZ::D3DResource<IDXGIFactory>	factory;
	BRZ::D3DResource<IDXGIAdapter>	adapter;
	BRZ::D3DResource<IDXGIOutput>	adapterOutput;
	unsigned int					numModes = 0;
	unsigned int					numerator = 0;
	unsigned int					denominator = 0;
	DXGI_MODE_DESC *				displayModeList = NULL;
	DXGI_ADAPTER_DESC				adapterDesc;
	unsigned int					screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	unsigned int					screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	DXGI_SWAP_CHAIN_DESC			swapChainDesc;
	D3D_FEATURE_LEVEL				featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Set up the testing variables:
	BRZ::check = &result;
	BRZ::prefix = "[Display::InitD3DDevice]: ";

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (!BRZ::Test("Failed to create DXGI Factory."))
		return BRZ_FAILURE;

	result = factory->EnumAdapters(0, &adapter);
	if (!BRZ::Test("Failed to enumerate adapters."))
		return BRZ_FAILURE;

	result = adapter->EnumOutputs(0, &adapterOutput);
	if (!BRZ::Test("Failed to enumerate outputs on primary adapter."))
		return BRZ_FAILURE;

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (!BRZ::Test("Failed to find number of valid display modes for chosen adapter output."))
		return BRZ_FAILURE;

	displayModeList = new DXGI_MODE_DESC[numModes];
	BRZ::ZeroMem(displayModeList, numModes);
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (!BRZ::Test("Failed to acquire valid display mode list."))
		return BRZ_FAILURE;

	// Find the refresh rate numerator and denominator for the screen in use:
	for(unsigned int i=0; i < numModes; ++i)
	{
		if(displayModeList[i].Width == screenWidth)
		{
			if(displayModeList[i].Height == screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	delete [] displayModeList;


	// Acquire the video card info and log it for future reference:
	BRZ::ZeroMem(&adapterDesc);
	result = adapter->GetDesc(&adapterDesc);
	if (!BRZ::Test("Failed to acquire adapter description."))
		return BRZ_FAILURE;

	vc_ram = adapterDesc.DedicatedVideoMemory / (1024 * 1024);
	vc_sys = adapterDesc.DedicatedSystemMemory / (1024 * 1024);
	BRZ::CopyMem(vc_name, adapterDesc.Description, 128);

	wnd_log << "\tVideo Card Information:" << std::endl;
	wnd_log << "----------------------------------------------" << std::endl;
	wnd_log << " Description:\t" << BRZ::Narrow(vc_name) << std::endl;
	wnd_log << " Video Memory:\t" << vc_ram << " MB" << std::endl;
	wnd_log << " Auxiliary Memory:\t" << vc_sys << " MB" << std::endl;
	wnd_log << " Refresh Rate (N):\t" << numerator << std::endl;
	wnd_log << " Refresh Rate (D):\t" << denominator << std::endl;
	wnd_log << " Screen Width:\t" << screenWidth << " pixels" << std::endl;
	wnd_log << " Screen Height:\t" << screenHeight << " pixels" << std::endl;
	wnd_log.flush();


	// Fill out the swap chain description:
	BRZ::ZeroMem(&swapChainDesc);

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = wnd_handle;
	swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	if (wnd_fullScreen)
	{
		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.BufferDesc.Width = BRZ::WND_WIDTH;
		swapChainDesc.BufferDesc.Height = BRZ::WND_HEIGHT;
		swapChainDesc.Windowed = true;
	}

	if(wnd_vSync)
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
		D3D11_SDK_VERSION, &swapChainDesc, &(this->d3d_swapChain), &(this->d3d_device), NULL, &(this->d3d_context));
	if (!BRZ::Test("Failed to create device and swap chain with specified settings."))
		return BRZ_FAILURE;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::InitD3DView()
{
	HRESULT									result;
	BRZ::D3DResource<ID3D11Texture2D>		backBufferPtr;
	D3D11_TEXTURE2D_DESC					depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC				depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC			depthStencilViewDesc;
	D3D11_RASTERIZER_DESC					rasterDesc;
	D3D11_VIEWPORT							viewport;
	D3D11_BLEND_DESC						blendDesc;
	unsigned int							screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	unsigned int							screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	// Set up the testing variables:
	BRZ::check = &result;
	BRZ::prefix = "[Display::InitD3DView]: ";




	result = d3d_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (!BRZ::Test("Failed to acquire back buffer pointer from swap chain."))
		return BRZ_FAILURE;

	result = d3d_device->CreateRenderTargetView(backBufferPtr, NULL, &d3d_renderTarget);
	if (!BRZ::Test("Failed to create render target view."))
		return BRZ_FAILURE;




	// Create the depth stencil buffer from description:
	BRZ::ZeroMem(&depthBufferDesc);
	if (wnd_fullScreen)
	{
		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
	}
	else
	{
		depthBufferDesc.Width = BRZ::WND_WIDTH;
		depthBufferDesc.Height = BRZ::WND_HEIGHT;
	}
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = d3d_device->CreateTexture2D(&depthBufferDesc, NULL, &d3d_depthStencilBuffer);
	if (!BRZ::Test("Failed to create depth stencil buffer."))
		return BRZ_FAILURE;



	// Create the depth stencil state from description:
	BRZ::ZeroMem(&depthStencilDesc);

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = d3d_device->CreateDepthStencilState(&depthStencilDesc, &d3d_depthStencilState);
	if(!BRZ::Test("Failed to create depth stencil state."))
		return BRZ_FAILURE;

	// Set the depth stencil state.
	d3d_context->OMSetDepthStencilState(d3d_depthStencilState, 1);




	// Create the depth stencil view from description:
	BRZ::ZeroMem(&depthStencilViewDesc);

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = d3d_device->CreateDepthStencilView(d3d_depthStencilBuffer, &depthStencilViewDesc, &d3d_depthStencilView);
	if(!BRZ::Test("Failed to create depth stencil view."))
		return BRZ_FAILURE;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	d3d_context->OMSetRenderTargets(1, &d3d_renderTarget, d3d_depthStencilView);




	// Setup the raster description which will determine how and what polygons will be drawn.
	BRZ::ZeroMem(&rasterDesc);
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = d3d_device->CreateRasterizerState(&rasterDesc, &d3d_rasterState);
	if(!BRZ::Test("Failed to create rasterizer state."))
		return BRZ_FAILURE;

	d3d_context->RSSetState(d3d_rasterState);



	
	// Setup the viewport for rendering.
	if (wnd_fullScreen)
	{
		viewport.Width = screenWidth;
		viewport.Height = screenHeight;
	}
	else
	{
		viewport.Width = BRZ::WND_WIDTH;
		viewport.Height = BRZ::WND_HEIGHT;
	}
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

	// Create the viewport.
    d3d_context->RSSetViewports(1, &viewport);

	// Set up the blending state for alpha blend enabling:
	BRZ::ZeroMem(&blendDesc);

	// Create an alpha enabled blend state description.
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = d3d_device->CreateBlendState(&blendDesc, &d3d_blendState);
	if (!BRZ::Test("Failed to create alpha shading blend state."))
		return BRZ_FAILURE;

	float	blend[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	d3d_context->OMSetBlendState(d3d_blendState, blend, 0xFFFFFFFF);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::Initialize(HWND A_wnd)
{
	wnd_handle = A_wnd;

	if (this->InitD3DDevice() != BRZ_SUCCESS)
	{
		wnd_log << "[Display::Initialize] failed: Could not initialize Direct3D device." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	if (this->InitD3DView() != BRZ_SUCCESS)
	{
		wnd_log << "[Display::Initialize] failed: Could not initialize Direct3D viewports." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	if (this->InitPipeline(64) != BRZ_SUCCESS)
	{
		wnd_log << "[Display::Initialize] failed: Could not initialize pipeline." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	if (this->InitCache(16384, 16384) != BRZ_SUCCESS)
	{
		wnd_log << "[Display::Initialize] failed: Could not initialize geometry cache." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	if (this->InitLibrary(512, 128) != BRZ_SUCCESS)
	{
		wnd_log << "[Display::Initialize] failed: Could not initialize object library." << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}

	wnd_log << "[Display::Initialize] successful." << std::endl;
	wnd_log.flush();
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::LoadShaderRaw(const BRZSTRING & A_src, BRZ::D3DResource<ID3D10Blob> & A_dest) const
{
	BRZCHAR dir[MAX_PATH];
	BRZ::ZeroMem(dir, MAX_PATH);
	::GetCurrentDirectory(MAX_PATH, dir);
	BRZSTRING file(dir);
	BRZSTRING sep(L"\\");
	file = file + sep + A_src;

	BRZBYTE *		raw = NULL;
	unsigned int	rawSize = 0;
	unsigned int	outSize = 0;
	
	HANDLE input = ::CreateFile(file.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (input == INVALID_HANDLE_VALUE)
	{
		wnd_log << "[Display::LoadShaderRaw]: Could not open shader file (" << BRZ::Narrow(file) << ")" << std::endl;
		wnd_log.flush();
		return BRZ_FAILURE;
	}
	else
	{
		LARGE_INTEGER	holder;
		::GetFileSizeEx(input, &holder);
		rawSize = holder.LowPart;
	}

	raw = new BRZBYTE[rawSize];
	::ReadFile(input, (LPVOID)raw, rawSize, (LPDWORD)&outSize, NULL);
	if (rawSize != outSize)
	{
		wnd_log << "[Display::LoadShaderRaw]: Invalid read on shader file (" << BRZ::Narrow(file) << ")" << std::endl;
		wnd_log.flush();
		delete [] raw;
		return BRZ_FAILURE;
	}
	
	::D3D10CreateBlob(rawSize, &A_dest);
	BRZ::CopyMem((BRZBYTE *)A_dest->GetBufferPointer(), raw, rawSize);
	delete [] raw;
	::CloseHandle(input);

	return BRZ_SUCCESS;
}


BRZ::Display::Display(std::ofstream & A_log) :
	vc_ram(0),
	vc_sys(0),
	wnd_handle(NULL),
	wnd_vSync(true),
	wnd_fullScreen(true),
	wnd_log(A_log),
	out_maxTickets(0),
	out_usedTickets(0),
	out_queue(NULL),
	cache_dir(L""),
	cache_importer(A_log),
	cache_maxPoints(0),
	cache_maxLines(0),
	cache_usedPoints(0),
	cache_usedLines(0),
	cache_points(NULL),
	cache_lines(NULL),
	cache_locked(false),
	geo_totalPoints(0),
	geo_totalIndices(0),
	geo_totalLines(0),
	obj_maxElements(0),
	obj_maxTemplates(0),
	obj_usedElements(0),
	obj_usedTemplates(0),
	obj_elements(NULL),
	obj_templates(NULL)
{
	BRZ::ZeroMem(vc_name, 128);
	DirectX::XMStoreFloat4x4(&out_view, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&out_projection, DirectX::XMMatrixIdentity());

	BRZ::out = &A_log;
}


BRZ::Display::~Display()
{
	if (out_queue != NULL)
		delete [] out_queue;

	if (cache_points != NULL)
		delete [] cache_points;

	if (cache_lines != NULL)
		delete [] cache_lines;

	if (obj_elements != NULL)
		delete [] obj_elements;

	if (obj_templates != NULL)
		delete [] obj_templates;
}
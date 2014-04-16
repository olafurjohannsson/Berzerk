
#include "BRZ_Display.h"
#include <fstream>

#include <d3d10.h>

/*
	class Display
	{
	public:
		Display(std::ofstream & logOut);
		~Display();

	public:
		BRZRESULT Initialize(HWND window);
		BRZRESULT GenerateGeo();
		BRZRESULT Render();

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
	};
*/


BRZRESULT BRZ::Display::LoadShaders()
{
	HRESULT						result;
	ID3D10Blob *					errorMessage;
	ID3D10Blob *					vertexShaderBuffer;
	ID3D10Blob *					pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC	polygonLayout[1];
	unsigned int				numElements;
	D3D11_BUFFER_DESC			matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	BRZCHAR dir[MAX_PATH];
	BRZ::ZeroMem(dir, MAX_PATH);
	::GetCurrentDirectory(MAX_PATH, dir);
	BRZSTRING vsFile(dir);
	vsFile = vsFile + BRZSTRING(L"\\vs_default.cso");

	BRZSTRING psFile(dir);
	psFile = psFile + BRZSTRING(L"\\ps_default.cso");


	BRZBYTE *		raw = NULL;
	unsigned int	rawSize = 0;
	unsigned int	outSize = 0;
	
	HANDLE input = ::CreateFile(vsFile.c_str()/*L"C:\\Dev\\Berzerk\\Debug\\vs_default.cso"*/, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (input == INVALID_HANDLE_VALUE)
	{
		log << "Could not open vertex shader file vs_default.cso" << std::endl;
		log.flush();
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
		log << "Invalid read on vs_default.cso" << std::endl;
		log.flush();
		delete [] raw;
		return BRZ_FAILURE;
	}
	
	::D3D10CreateBlob(rawSize, &vertexShaderBuffer);
	BRZ::CopyMem((BRZBYTE *)vertexShaderBuffer->GetBufferPointer(), raw, rawSize);
	delete [] raw;
	::CloseHandle(input);


	raw = NULL;
	rawSize = 0;
	outSize = 0;
	input = ::CreateFile(psFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (input == INVALID_HANDLE_VALUE)
	{
		log << "Could not open vertex shader file ps_default.cso" << std::endl;
		log.flush();
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
		log << "Invalid read on ps_default.cso" << std::endl;
		log.flush();
		delete [] raw;
		return BRZ_FAILURE;
	}
	
	::D3D10CreateBlob(rawSize, &pixelShaderBuffer);
	BRZ::CopyMem((BRZBYTE *)pixelShaderBuffer->GetBufferPointer(), raw, rawSize);
	delete [] raw;
	::CloseHandle(input);


    

    // Create the vertex shader from the buffer.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vShader);
	if(FAILED(result))
	{
		log << "Failed to create vertex shader." << std::endl;
		log.flush();
		return BRZ_FAILURE;
	}

    // Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pShader);
	if(FAILED(result))
	{
		log << "Failed to create pixel shader." << std::endl;
		log.flush();
		return BRZ_FAILURE;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	/*
	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	*/

	// Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
									   vertexShaderBuffer->GetBufferSize(), &inLayout);
	if(FAILED(result))
	{
		log << "Failed to create input layout." << std::endl;
		log.flush();
		return BRZ_FAILURE;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(BRZ::ShaderConstants);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &matBuffer);
	if(FAILED(result))
	{
		log << "Failed to create shader constant buffer." << std::endl;
		log.flush();
		return BRZ_FAILURE;
	}

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::GenerateGeo()
{
	BRZ::Vertex	*		vertices = new BRZ::Vertex[4];
	unsigned int *		indices = new unsigned int[8];
	D3D11_BUFFER_DESC	vertexBufferDesc;
	D3D11_BUFFER_DESC	indexBufferDesc;

    D3D11_SUBRESOURCE_DATA	vertexData;
	D3D11_SUBRESOURCE_DATA	indexData;
	HRESULT					result;


	// Set the number of vertices in the vertex array.
	vertCount = 4;

	// Set the number of indices in the index array.
	indexCount = 8;

	lineCount = 3;


	// Load the vertex array with data.
	vertices[0].pos = DirectX::XMFLOAT3(-10.0f, -10.0f, 10.0f);
	vertices[1].pos = DirectX::XMFLOAT3(10.0f, -10.0f, 10.0f);
	vertices[2].pos = DirectX::XMFLOAT3(-10.0f, 10.0f, 10.0f);
	vertices[3].pos = DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 1;  // Bottom right
	indices[3] = 3;
	indices[4] = 3;
	indices[5] = 2;
	indices[6] = 0;
	indices[7] = 2;

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(BRZ::Vertex) * vertCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertex);
	if(FAILED(result))
	{
		log << "Failed to create vertex buffer." << std::endl;
		return BRZ_FAILURE;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &index);
	if(FAILED(result))
	{
		log << "Failed to create index buffer." << std::endl;
		return BRZ_FAILURE;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::Render(float A_x, float A_y, float A_r)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = 0.2f;
	color[1] = 0.2f;
	color[2] = 0.2f;
	color[3] = 1.0f;

	// Clear the back buffer.
	context->ClearRenderTargetView(renderTarget, color);
    
	// Clear the depth buffer.
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);




	// Set up shader constants:
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	BRZ::ShaderConstants * dataPtr;
	unsigned int bufferNumber;


	world = DirectX::XMMatrixIdentity() * DirectX::XMMatrixRotationZ(A_r) * DirectX::XMMatrixTranslation(A_x, A_y, 0);
	// Transpose the matrices to prepare them for the shader.
	BRZMATRIX	tWorld = DirectX::XMMatrixTranspose(world);
	BRZMATRIX	tView = DirectX::XMMatrixTranspose(view);
	BRZMATRIX	tProj = DirectX::XMMatrixTranspose(projection);
	

	// Lock the constant buffer so it can be written to.
	result = context->Map(matBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		log << "Failed to lock the constant buffer." << std::endl;
		return BRZ_FAILURE;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (BRZ::ShaderConstants*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tWorld;
	dataPtr->view = tView;
	dataPtr->proj = tProj;

	// Unlock the constant buffer.
    context->Unmap(matBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
    context->VSSetConstantBuffers(bufferNumber, 1, &matBuffer);


	// Lock buffers:
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(BRZ::Vertex); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, &vertex, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(index, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	// Draw Primatives:
	// Set the vertex input layout.
	context->IASetInputLayout(inLayout);

    // Set the vertex and pixel shaders that will be used to render this triangle.
    context->VSSetShader(vShader, NULL, 0);
    context->PSSetShader(pShader, NULL, 0);

	// Render the triangle.
	context->DrawIndexed(indexCount, 0, 0);




	if(vSync)
	{
		// Lock to screen refresh rate.
		chain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		chain->Present(0, 0);
	}

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Display::Initialize(HWND A_wnd)
{
	window = A_wnd;


	HRESULT							result;
	IDXGIFactory *					factory = NULL;
	IDXGIAdapter *					adapter = NULL;
	IDXGIOutput *					adapterOutput = NULL;
	unsigned int numModes, i, numerator, denominator;
	DXGI_MODE_DESC *				displayModeList = NULL;
	DXGI_ADAPTER_DESC				adapterDesc;
	int								error = 0;
	DXGI_SWAP_CHAIN_DESC			swapChainDesc;
	D3D_FEATURE_LEVEL				featureLevel;
	ID3D11Texture2D *				backBufferPtr = NULL;
	D3D11_TEXTURE2D_DESC			depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC		depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;
	D3D11_RASTERIZER_DESC			rasterDesc;
	D3D11_VIEWPORT					viewport;
	float fieldOfView, screenAspect;

	unsigned int					screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	unsigned int					screenHeight = ::GetSystemMetrics(SM_CYSCREEN);


	// Store the vsync setting.
	// m_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		log << "Failed to create DXGI Factory Object." << std::endl;
		return BRZ_FAILURE;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		log << "Failed to Enumerate Adapters." << std::endl;
		return BRZ_FAILURE;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
	{
		log << "Failed to Enumerate Outputs on Primary Adapter." << std::endl;
		return BRZ_FAILURE;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		log << "Failed to find valid display modes." << std::endl;
		return BRZ_FAILURE;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		log << "Failed to create display mode list." << std::endl;
		return BRZ_FAILURE;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		log << "Failed to fill display mode list." << std::endl;
		return BRZ_FAILURE;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(i=0; i<numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if(displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		log << "Failed to acquire adapter description." << std::endl;
		return BRZ_FAILURE;
	}

	// Store the dedicated video card memory in megabytes.
	vRam = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	BRZ::CopyMem(vName, adapterDesc.Description, 128);
	/*
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		return false;
	}
	*/

	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	BRZ::ZeroMem(&swapChainDesc);
    // ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	if (fullScreen)
	{
		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;
	}
	else
	{
		swapChainDesc.BufferDesc.Width = BRZ::WND_WIDTH;
		swapChainDesc.BufferDesc.Height = BRZ::WND_HEIGHT;
	}

	// Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if(vSync)
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
    swapChainDesc.OutputWindow = window;

	// Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(fullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
		D3D11_SDK_VERSION, &swapChainDesc, &(this->chain), &(this->device), NULL, &(this->context));
	if(FAILED(result))
	{
		log << "Failed to create device and swap chain." << std::endl;
		return BRZ_FAILURE;
	}

	// Get the pointer to the back buffer.
	result = chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		log << "Failed to get back buffer pointer." << std::endl;
		return BRZ_FAILURE;
	}

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTarget);
	if(FAILED(result))
	{
		log << "Failed to create render target view." << std::endl;
		return BRZ_FAILURE;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	BRZ::ZeroMem(&depthBufferDesc);
	// ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	if (fullScreen)
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
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if(FAILED(result))
	{
		log << "Failed to create depth stencil buffer." << std::endl;
		return BRZ_FAILURE;
	}

	// Initialize the description of the stencil state.
	BRZ::ZeroMem(&depthStencilDesc);
	// ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if(FAILED(result))
	{
		log << "Failed to create depth stencil state." << std::endl;
		return BRZ_FAILURE;
	}

	// Set the depth stencil state.
	context->OMSetDepthStencilState(depthStencilState, 1);

	// Initialize the depth stencil view.
	BRZ::ZeroMem(&depthStencilViewDesc);
	// ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if(FAILED(result))
	{
		log << "Failed to create depth stencil view." << std::endl;
		return BRZ_FAILURE;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	context->OMSetRenderTargets(1, &renderTarget, depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	BRZ::ZeroMem(&rasterDesc);
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if(FAILED(result))
	{
		log << "Failed to create rasterizer state." << std::endl;
		return BRZ_FAILURE;
	}

	// Now set the rasterizer state.
	context->RSSetState(rasterState);
	
	// Setup the viewport for rendering.
	if (fullScreen)
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
    context->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	// fieldOfView = (float)BRZ::PI / 4.0f;
	// screenAspect = (float)800 / (float)600;

	// Create the projection matrix for 3D rendering.
	// projection = DirectX::XMMatrixPerspectiveLH(800, 600, 0.1f, 1000f);
	// D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

    // Initialize the world matrix to the identity matrix.
	world = DirectX::XMMatrixIdentity();
    // D3DXMatrixIdentity(&m_worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	if (fullScreen)
		projection = DirectX::XMMatrixOrthographicLH(screenWidth, screenHeight, 0.1f, 1000.0f);
	else
		projection = DirectX::XMMatrixOrthographicLH(BRZ::WND_WIDTH, BRZ::WND_HEIGHT, 0.1f, 1000.0f);
	// D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	DirectX::XMVECTORF32 eye = {0.0f, 0.0f, -10.0f };
	DirectX::XMVECTORF32 focus = { 0.0f, 0.0f, 100.0f };
	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f };


	view = DirectX::XMMatrixLookAtLH(eye, focus, up);

    return BRZ_SUCCESS;
}


BRZ::Display::Display(std::ofstream & A_log) :
	vSync(false),
	vRam(0),
	log(A_log),
	window(NULL),
	fullScreen(false)
{
	BRZ::ZeroMem(vName, 128);
	BRZ::ZeroMem(&view);
	BRZ::ZeroMem(&projection);
	BRZ::ZeroMem(&world);
}


BRZ::Display::~Display()
{
	if (fullScreen)
	{
		if(chain)
		{
			chain->SetFullscreenState(false, NULL);
		}
	}
}
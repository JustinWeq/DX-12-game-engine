#include "BufferedView.h"

//defualt constructor-- creates a new instance of BufferedView with defualt parameters
BufferedView::BufferedView()
{
	m_frameIndex = 0;
	m_rtvDescriptorHeap = 0;
	m_rtvDescriptorSize = 0;
	m_swapChain = 0;
}

//decontructor-- cleans up memory for the BufferedView instance
BufferedView::~BufferedView()
{
	//call close
	close();
}

//cleans up memory for the BufferedView instance
void BufferedView::close() 
{
	//clean up the swap chain if it is initalized
	if (m_swapChain)
	{
		m_swapChain->Release();
		delete m_swapChain;
		m_swapChain = 0;
	}

	//clean up the resource view descriptor heap if it exists
	if (m_rtvDescriptorHeap)
	{
		m_rtvDescriptorHeap->Release();
		delete m_rtvDescriptorHeap;
		m_rtvDescriptorHeap = 0;
	}
}

//initializes the BufferedView instance.
//handle- a pointer to the D3DInterface to use for initalizing the buffered view instance
//the windows form to use for the output window
bool BufferedView::initBufferedView(D3DInterface* handle, WindowsForm* form)
{
	HRESULT result;
	//create the swap chain with triple buffering
	DXGI_MODE_DESC backBufferDesc = {};

	backBufferDesc.Width = 0;
	backBufferDesc.Height = 0;
	backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//describe multisampling
	DXGI_SAMPLE_DESC sampleDesc = {};
	//set the sample count to 2 for 2x MSAA
	sampleDesc.Count = 2;

	//describe and create the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = D3DInterface::frameBufferCount;
	swapChainDesc.BufferDesc = backBufferDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = form->getHwnd();
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.Windowed = !WindowsForm::FULL_SCREEN;

	IDXGISwapChain* tempSwapChain;

	//create the swap chain
	handle->getFactory()->CreateSwapChain(handle->getCommandQueue(),
		&swapChainDesc, &tempSwapChain);

	//static cast the swap chain to the swapchain3
	m_swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);

	//set the current frame index
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	//create the redner target views for the back buffers(there are three of them)
	
	//create the descriptor heap and describe it
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = D3DInterface::frameBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	result = handle->getDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvDescriptorHeap));
	if (FAILED(result))
	{
		return false;
	}

	//get the size of the descriptor heap
	m_rtvDescriptorSize = handle->getDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//get handle to the first descriptor in the descriptor heap
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	//create the rtv for each buffer
	for (int i = 0; i < D3DInterface::frameBufferCount; i++)
	{
		result = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		if (FAILED(result))
		{
			return false;
		}

		handle->getDevice()->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);

		//increment the rtv handle
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}
}
#pragma once
#include "stdafx.h"
#include "D3DInterface.h"
#include "win32app.h"
class BufferedView
{

public:
	//defualt constructor-- creates a new instance of BufferedView with defualt parameters
	BufferedView();

	//decontructor-- cleans up memory for the BufferedView instanceees
	~BufferedView();

	//cleans up memory for the BufferedView instance
	void close();

	//initializes the BufferedView instance.
	//handle- a pointer to the D3DInterface to use for initalizing the buffered view instance
	//the windows form to use for the output window
	bool initBufferedView(D3DInterface* handle, WindowsForm* form);

	void present(ID3D12CommandList** commandLists,D3DInterface* handle);

private:
	//The swap chain that is used to transition frames
	IDXGISwapChain3* m_swapChain;

	//the render targets, this is exactly equal to the number of frame buffers being used
	ID3D12Resource* m_renderTargets[D3DInterface::frameBufferCount];

	//The descriptor heap for resource target views, such as the render targets
	ID3D12DescriptorHeap* m_rtvDescriptorHeap;

	// The current resource target view(buffer) we are on
	int m_frameIndex;

	//the size of the rtv descriptor
	int m_rtvDescriptorSize;
};
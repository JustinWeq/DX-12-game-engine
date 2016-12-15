#pragma once
#include "stdafx.h"
#include "D3DInterface.h"
#include "win32app.h"
#include "CommandThread.h"
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

	// exectutes the command lists on the command queue that is in the passed in
	// d3d12 interface instance
	// commandLists- a pointer to the array of command lists to execute
	// handle- a pointer to the d3d interface instance with the queue to use for execution
	bool present(unsigned char numberOfLists,CommandThread** commandThreads,D3DInterface* handle);

	//returns the frame index
	int getFrameIndex();

	//returns the render targets
	ID3D12Resource* getRenderTarget(int index);

	//returns the descriptor heap
	ID3D12DescriptorHeap* getDescriptorHeap();

	//returns the descriptor size
	int getDescriptorSize();

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
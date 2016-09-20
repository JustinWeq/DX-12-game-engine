#pragma once
#include "stdafx.h"
class D3DInterface
{


public:

	//number of frame buffers, in this case it is three since triple buffering is being used
	static const int frameBufferCount = 3;

	//defualt contructor-- creates a new instance of D3DInterface with defualt values
	D3DInterface();

	//initializes d3d 12
	bool initD3D();

	//returns the command queue for the interface
	ID3D12CommandQueue* getCommandQueue();

	//returns the device for the interface
	ID3D12Device* getDevice();

	//returns the dxgi factory for the interface
	IDXGIFactory4* getFactory();

	//presents the command lists and puts them on the queue to be presented and rendered
	void present(ID3D12CommandList** commandLists);

private:

private:

	//The main direct 3d 12 device
	ID3D12Device* m_device;



	//The container for the command lists
	ID3D12CommandQueue* m_commandQueue;

	//the dxgi factory
	IDXGIFactory4* m_dxgiFactory;

	////The command allocator is used to connect and send the command lists to the gpu, one allocator is needed for every buffer
	//ID3D12CommandAllocator* m_commandAllocator[frameBufferCount];

	////Fences are locked while a command list is being executed by the gpu, this prevents syncronization issues
	//ID3D12Fence* m_fence[frameBufferCount];

	//// A handle to a fence event, this is used to signal when the gpu is done with a command list
	//HANDLE m_fenceEvent;

	////the fence value is incremented each frame, each fence has a seperate value
	//UINT64 fenceValue[frameBufferCount];




};
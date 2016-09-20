#include "D3DInterface.h"

//defualt contructor-- creates a new instance of D3DInterface with defualt values
D3DInterface::D3DInterface()
{

	//set all data to null
	m_commandQueue = 0;
	m_device = 0;
	m_dxgiFactory = 0;
}

//initializes d3d 12
// returns true if successfull false otherwise
bool D3DInterface::initD3D()
{
	HRESULT result;

	//create the device
	
	result = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
	if (FAILED(result))
	{
		return false;
	}

	//the graphics card that will be used
	IDXGIAdapter1* adapter;

	//start looking for dx 12 compatible hardware devices at index 0
	int adapterIndex = 0;

	//set this to false for now, since a compatible hardware device has not yet been found
	bool adapterFound = false;

	//find the first gpu that supports direct 3d 12
	while (m_dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			//do not use a software device.
			adapterIndex++;
			continue;
		}

		//Only use a device that is compatible with feature level 11 or higher
		result = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(result))
		{
			//a compatible hardware device was found
			adapterFound = true;
			break;
		}

		adapterIndex++;
	}

	if (!adapterFound)
	{
		// no compatible hardware device was found so do not continue with the application
		return false;
	}

	//create the hardware device interface
	result = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
	if (FAILED(result))
	{
		return false;
	}

	//create the command queue

	//use all of the defualt values
	D3D12_COMMAND_QUEUE_DESC cqDesc{};
	
	//create the actual command queue
	result = m_device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(result))
	{
		//failed to create the command queue so return false
		return false;
	}

	return true;
}

//returns the command queue for the interface
ID3D12CommandQueue* D3DInterface::getCommandQueue()
{
	return m_commandQueue;
}

//returns the device for the interface
ID3D12Device* D3DInterface::getDevice()
{
	return m_device;
}

//returns the dxgi factory for the interface
IDXGIFactory4* D3DInterface::getFactory()
{
	return m_dxgiFactory;
}

//presents the command lists and puts them on the queue to be presented and rendered
//commandLists-- the command lists to send to the command queue
//numberOfLists-- the number of lists to present
void present(ID3D12CommandList** commandLists,unsigned char offset,unsigned char numberOfLists);
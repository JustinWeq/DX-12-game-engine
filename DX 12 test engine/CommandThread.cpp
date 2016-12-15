#include "CommandThread.h"


// defualt constructor-- creates a new instance of CommandThread with defualt parameters
CommandThread::CommandThread()
{
	//set all pointers to the defualt value of NULL
	m_commandList = NULL;
}

// decontructor-- cleams up memory for this instance of command thread by calling close
CommandThread::~CommandThread()
{
	//call close
	close();
}

// initializes the command thread class
// returns true if succsessfull
// handle- a pointer to the d3d interface to use for intialization
 bool CommandThread::init(D3DInterface* handle)
{
	HRESULT result;
	//initalize the command allocators
	for (int i = 0; i < 3; i++)
	{
		result = handle->getDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[i]));
		if (FAILED(result))
		{
			//failed to create the command allocator so return false
			return false;
		}
	}

	//create the command list using the first command allocator
	result = handle->getDevice()->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT,m_commandAllocators[0],
		NULL, IID_PPV_ARGS(&m_commandList));
	if (FAILED(result))
	{
		//failed to create he command list so return false
		return false;
	}

	//command list is in a recording state by defualt so close it
	m_commandList->Close();

	//create the fence and the fence event

	//create the fences
	for (int i = 0; i < 3; i++)
	{
		result = handle->getDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence[i]));
		if (FAILED(result))
		{
			return false;
		}
		//set the intial fence value to zero
		m_fenceValue[0];
	}

	// create a handle to a fence event
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		//fence failed to create so return false
		return false;
	}

	//return true since everything was a success
	return true;

}

 //signals the fence for this command thread
 // commandQueue- the command queue to signal the fence with
 // frameIndex- the current frame the buffer is on
 bool CommandThread::signalFence(ID3D12CommandQueue* commandQueue,int frameIndex)
 {
	 HRESULT result;

	 //signal the fence
	 result = commandQueue->Signal(m_fence[frameIndex], m_fenceValue[frameIndex]);
	 if (FAILED(result))
	 {
		 //return false since the command queue failed to signal the fence
		 return false;
	 }
	 m_frameIndex = frameIndex;
	 return true;
 }

 // resets the command list and the command allocator
bool CommandThread::reset()
{
	HRESULT result;

	//wiat for the previous frame
	waitForLastFrame();

	//reset the allocator since the gpu is done with it
	result = m_commandAllocators[m_frameIndex]->Reset();
	if (FAILED(result))
	{
		//the allocator failed to reset so return false
		return false;
	}

	//reset the command list
	result = m_commandList->Reset(m_commandAllocators[m_frameIndex],NULL);
	if (FAILED(result))
	{
		return false;
	}

	//command thread was reset so return true
	return true;
}

//returns the command list
ID3D12CommandList* CommandThread::getCommandList()
{
	return m_commandList;
}

// cleans up memory for the command thread
void CommandThread::close()
{
	//delete the memory for the fences and the command allocators
	for (int i = 0; i < 3; i++)
	{
		//release the command allocator at i
		m_commandAllocators[i]->Release();
		//release the fence at i
		m_fence[i]->Release();
	}

	//delete command allocators
	delete[] m_commandAllocators;
	//delete fences
	delete[] m_fence;
}

//waits for the last frame to finish
// frameIndex- the current frame the fence is on
void CommandThread::waitForLastFrame()
{
	HRESULT result;
	
	//if th4e current frame index is less then the fence value then the
	// gpu has not finshed executing its command yet
	if (m_fence[m_frameIndex]->GetCompletedValue() < m_fenceValue[m_frameIndex])
	{
		//have the fence create an event to be singled once the current value is the fence value
		result = m_fence[m_frameIndex]->SetEventOnCompletion(m_fenceValue[m_frameIndex], m_fenceEvent);
		if (FAILED(result))
		{
			throw "Failed to create a fence event";
		}

		//wait untill the fence has completed its event
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	//increment the fence value for the next frame
	m_fenceValue[m_frameIndex]++;
}
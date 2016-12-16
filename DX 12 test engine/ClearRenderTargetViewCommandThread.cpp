#include "ClearRenderTargerViewCommandThread.h"
bool red = true;
int blue = 0;
// a pure virtual function, this needs to be overridden with a specific class that inherits from this abstract base class,
// it is recommended you call this function inside of a thread if you are using more then one command thread instance
// to save compute time.
// D3DInterface* -- You must pass in a d3d interface here
// BufferedView* -- You must pass in a BufferedView here
bool ClearRenderTargetViewCommandThread::update(int flags,...)
{
	HRESULT result;
	// call reset, note this is always called at the start of the overloaded function
	reset();
	va_list list;
	va_start(list,flags);
	

	D3DInterface* handle = (D3DInterface*)va_arg(list, D3DInterface*);

	BufferedView*  view = (BufferedView*) va_arg(list,BufferedView*);

	//start recording commands into the command list

	//transition the frame index render target so that the command list draws to it
	m_commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(view->getRenderTarget(view->getFrameIndex()),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//get the handle to the current render target view
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(view->getDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		view->getFrameIndex(), view->getDescriptorSize());

	//set the render target for the output merger state
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	//Clear the render target for the output merger state transition
	red = !red;
	const float clearColor[] = { red,!red,(float)(blue+=1)/100.0f,1 };
	blue %= 100;
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	
	//transition the frame index
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(view->getRenderTarget(view->getFrameIndex()),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// close the command list recording state
	result = m_commandList->Close();
	if (FAILED(result))
	{
		//command list failed to close so return false
		return false;
	}

	//return true since nothing went wrong
	return true;
}
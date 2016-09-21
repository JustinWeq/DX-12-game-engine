#include "ClearRenderTargerViewCommandThread.h"
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
	const float clearColor[] = { 1,0,0,1 };
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
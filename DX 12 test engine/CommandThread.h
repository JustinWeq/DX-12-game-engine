#pragma once
#include "stdafx.h"
#include "D3DInterface.h"

// a class that contains methods and data for running concurrent command lists
class CommandThread
{

public:
	// defualt constructor-- creates a new instance of CommandThread with defualt parameters
	CommandThread();

	// decontructor-- cleams up memory for this instance of command thread by calling close
	~CommandThread();

	// initializes the command thread class
	// returns true if succsessfull
	// handle- a pointer to the d3d interface to use for intialization
	virtual bool init(D3DInterface* handle);

	// a pure virtual function, this needs to be overridden with a specific class that inherits from this abstract base class,
	// it is recommended you call this function inside of a thread if you are using more then one command thread instance
	// to save compute time.
	// params- a multiparameter list of pointers, paramters need to be manually interpreted from this list
	virtual bool update(int flags,...) = 0;

	// cleans up memory for the command thread
	virtual void close();
private:

	

	// the command allocators
	ID3D12CommandAllocator* m_commandAllocators[3];

	//the fences
	ID3D12Fence* m_fence[3];

	// the fence event
	HANDLE m_fenceEvent;

	// the fence value
	UINT64 m_fenceValue[3];

	// the current frame this instance of the command thread is on.
	int m_frameIndex;

    //waits for the last frame to finish
	void waitForLastFrame();

protected:

	

	// resets the command list and the command allocator
	// call this before anything else when you override the update method
	bool reset();

	// the command list
	ID3D12GraphicsCommandList* m_commandList;

};

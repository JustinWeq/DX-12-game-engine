#pragma once
#include "CommandThread.h"
#include "BufferedView.h"
class ClearRenderTargetViewCommandThread : public CommandThread
{
public:
	// a pure virtual function, this needs to be overridden with a specific class that inherits from this abstract base class,
	// it is recommended you call this function inside of a thread if you are using more then one command thread instance
	// to save compute time.
	// D3DInterface* -- You must pass in a d3d interface here
	// BufferedView* -- You must pass in a BufferedView here
	bool update(int flags,...) override;
};
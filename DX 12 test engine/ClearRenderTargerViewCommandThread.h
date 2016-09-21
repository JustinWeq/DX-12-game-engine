#pragma once
#include "CommandThread.h"
#include "BufferedView.h"
class ClearRenderTargetViewCommandThread : CommandThread
{
public:
	bool update(int flags,...) override;
};
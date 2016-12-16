#include "stdafx.h"
#include "win32app.h"
#include "D3DInterface.h"
#include "BufferedView.h"
#include "ClearRenderTargerViewCommandThread.h"

WindowsForm* form;
D3DInterface* device;
BufferedView* renderTarget;
ClearRenderTargetViewCommandThread* commandThread;
bool quit;
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int cmdShow)
{
	//set quit to false;
	quit = false;

	std::thread formThread;

	//construct the objects

	//construct the windows form
	form = new WindowsForm();

	//construct the D3DInterface
	device = new D3DInterface();

	//construct the buffered view
	renderTarget = new BufferedView();

	commandThread = new ClearRenderTargetViewCommandThread();

	//init and run the form
	bool result = form->initialize();
	if (!result)
	{
		//failed to create windows form
		MessageBox(0, TEXT("Failed to create windows form"), TEXT("The creation of the windows form failed"), MB_OK);
		quit = true;
	}

	//set up the D3D interface
	result = device->initD3D();
	if (!result)
	{
		//failed to create d3d interface
		MessageBox(0, TEXT("Failed to create d3dinterface"), TEXT("The creation of the d3d interface failed"), MB_OK);
		quit = true;
	}

	//set up the clear render target command thread
	result = commandThread->init(device);
	if (!result)
	{
		//failed to create the clear rendertarget command thread
		MessageBox(0, TEXT("Failed to create windows form"), TEXT("The creation of the windows form failed"), MB_OK);
		quit = true;
	}

	//set up the buffered view
	result = renderTarget->initBufferedView(device, form);
	if (!result)
	{
		//failed to create the render target
		MessageBox(0, TEXT("Failed to create render target"), TEXT("The creation of the render target failed"), MB_OK);
		quit = true;
	}

	// update and render untill the program crashes or the user tells the program to quit
	while(!quit)
	{
		//update the windwos form
		quit = !form->run();


		//update the command thread
		quit = !commandThread->update(2, device, renderTarget);

		CommandThread** commands = new CommandThread*[1];
		commands[0] = commandThread;

		//present the command threads
		quit = !renderTarget->present(1, commands, device);
		int text = 0;
	}
	commandThread->update(2, device, renderTarget);
	

	//close the form
	form->close();

	//delete the form memory
	delete form;
	form = 0;

	//delete the render target from memory
	delete device;
	device = 0;

	//close the render taarget and then free the memory for it
	renderTarget->close();
	delete renderTarget;
	renderTarget = 0;
	return 0;
}


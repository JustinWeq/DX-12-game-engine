#include "stdafx.h"
#include "win32app.h"
#include "D3DInterface.h"
#include "BufferedView.h"
#include "ClearRenderTargerViewCommandThread.h"

void runWindowsForm();
WindowsForm* form;
D3DInterface* device;
BufferedView* renderTarget;
ClearRenderTargetViewCommandThread* commandThread;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int cmdShow)
{
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
	if (result)
	{
		

		//set up the D3D interface
		device->initD3D();

		//set up the clear render target command thread
		commandThread->init(device);

		//set up the buffered view
		renderTarget->initBufferedView(device, form);



		//run the windows form in  a thread
		formThread = std::thread(runWindowsForm);

	    commandThread->update(2, device, renderTarget);

	}
	
	//sync the form thread
	formThread.join();

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

void runWindowsForm()
{
	form->run();
}
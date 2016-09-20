#include "stdafx.h"
#include "win32app.h"
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, LPSTR, int cmdShow)
{
	WindowsForm form = WindowsForm();

	//init and run the form
	bool result = form.initialize();
	if (result)
	{
		form.run();
	}

	//close the form
	form.close();
	return 0;
}
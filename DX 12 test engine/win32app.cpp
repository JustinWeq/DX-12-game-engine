#include "win32app.h"

/// defualt constructor-- creates a new instance of WindowsForm with defualt parameters
WindowsForm::WindowsForm() {}
/// deconstructor-- an overloaded operator that is called when an instance of WindowsForm is deleted
WindowsForm::~WindowsForm() {}

/// calls to initialize the windows form
bool WindowsForm::initialize()
{
	int screenHieght = 0, screenWidth = 0;

	// init the windows api
	initWindows(screenHieght, screenWidth);

	return true;
}

/// leans up memeory for the windows form so that it can be safely deleted
bool WindowsForm::close()
{
	//unregister and delete this windows class
	closeWindows();
	return true;
}

/// runs the windows form and starts it
bool WindowsForm::run()
{
	MSG msg;
	bool result;
	//init memeory for the message
	ZeroMemory(&msg, sizeof(MSG));

	//keep running untill the user decides to quit
	m_quit = false;
	while (!m_quit)
	{
		//handle all of the windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//if windows signals to quit then exit the app
		if (msg.message == WM_QUIT)
		{
			m_quit = true;
		}
		else
		{
			//other wise process one frame
			result = update();
			if (!result)
			{
				m_quit = true;
			}
		}
	}
	return true;
}

/// called to update this windows form
bool WindowsForm::update()
{
	bool result;

	return true;
}

/// called to initialize the windows class for this windows form
/// width- the width of the window
/// height the height of the window
void WindowsForm::initWindows(int& width, int& height)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//get the external pointer to this object
	AppHandle = this;

	//get the instance of this application
	m_hinstance = GetModuleHandle(NULL);

	//give the application a name
	m_appName = L"Color shooter";

	//set up this windows class with the defualt settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the windows class
	RegisterClassEx(&wc);

	//Determine the resolution of the clients desktop
	height = GetSystemMetrics(SM_CYSCREEN);
	width = GetSystemMetrics(SM_CXSCREEN);

	//if the window is fullscreen then set up the form so that it is fullscreen
	if (FULL_SCREEN)
	{
		//if fullscreen set the screen to maximum size of the users desktop and 64 bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//change the display settings for fullscreen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//set the position of the windows to the top left corner
		posX = posY = 0;
	}
	else
	{
		//if windows mode then set it to 640x480 resolution
		width = 640;
		height = 480;

		//Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	//create the windows with the screen settins and get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, width, height, NULL, NULL, m_hinstance, NULL);

	//Bring the windows up on the screen and set it as the main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//Hide the mouse cursor
	ShowCursor(false);

	//store the height and width of the window
	m_width = width;
	m_height = height;
}
/// closes this windows class
void WindowsForm::closeWindows()
{
	//Show the mouse cursor
	ShowCursor(true);

	//fix the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//Remove the applicaiton instance
	UnregisterClass(m_appName, m_hinstance);
	m_hinstance = NULL;

	//release the pinter to this class
	AppHandle = NULL;
}

/// the message handler for the windows form, this is called when the form receives a new message such as the exit button being clicked
/// or a key being pressed
/// hwnd- a handle to the window
/// uint- the uint parameter
/// wparam- the wparam parameter
/// lparam- he lparam parameter
LRESULT CALLBACK WindowsForm::MessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lParam)
{
	switch (uint)
	{
	case WM_KEYDOWN:
	{
		//if escaped was pressed then quit
		if ((unsigned int)wparam == VK_ESCAPE)
		{
			m_quit = true;
		}
		break;
	}
	default:
	{
		return DefWindowProc(hwnd, uint, wparam, lParam);
	}
	}
}

/// used to intialize the windows form
/// hwnd- a handle to the window
/// uint- the uint parameter
/// wparam- the wparam parameter
/// lparam- he lparam parameter
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
{
	switch (uint)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return AppHandle->MessageHandler(hwnd, uint, wparam, lparam);
	}
		
	}
}

//returns the hwnd for the windows form
HWND WindowsForm::getHwnd()
{
	return m_hwnd;
}

//returns the height of the window for this form
int WindowsForm::GetWindowHeight()
{
	//return the height of the main window
	return m_height;
}

//reutrns the width of the windows for this form
int WindowsForm::GetWindowWidth()
{
	//return the width of the window
	return m_width;
}
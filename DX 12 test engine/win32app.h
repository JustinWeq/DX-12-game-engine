#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
/// A simple windows form class
class WindowsForm
{
public:
	/// defualt constructor-- creates a new instance of WindowsForm with defualt parameters
	WindowsForm();
	/// deconstructor-- an overloaded operator that is called when an instance of WindowsForm is deleted
	~WindowsForm();

	/// calls to initialize the windows form
	bool initialize();

	/// leans up memeory for the windows form so that it can be safely deleted
	bool close();

	/// runs the windows form and starts it
	bool run();

	//returns the hwnd for the windows form
	HWND getHwnd();

	//returns the height of the window for this form
	int GetWindowHeight();

	//reutrns the width of the windows for this form
	int GetWindowWidth();

	const static bool FULL_SCREEN = false;

	/// the message handler for the windows form, this is called when the form receives a new message such as the exit button being clicked
	/// or a key being pressed
	/// hwnd- a handle to the window
	/// uint- the uint parameter
	/// wparam- the wparam parameter
	/// lparam- he lparam parameter
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lParam);
private:

	/// called to update this windows form
	bool update();

	/// called to initialize the windows class for this windows form
	/// width- the width of the window
	/// height the height of the window
	void initWindows(int& width,int& height);

	/// closes this windows class
	void closeWindows();
private:

	/// the name of the app
	LPCWSTR m_appName;

	/// a handle to the instance of the windows class for this windwos form
	HINSTANCE m_hinstance;

	/// a handle to the window of this windows form class
	HWND m_hwnd;

	/// a bool indicatiing whether to quit the form or not
	bool m_quit;

	//the width of the window
	int m_width;

	//the height of the window
	int m_height;

	

};

/// used to intialize the windows form
/// hwnd- a handle to the window
/// uint- the uint parameter
/// wparam- the wparam parameter
/// lparam- he lparam parameter
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);

/// a handle to the main windows form application, to be used externally by other classes
static WindowsForm* AppHandle = 0;
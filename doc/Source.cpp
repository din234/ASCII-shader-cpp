#include <Windows.h>

using namespace std;

/*
	- Instance of application is also the id of the application: eg. if 2 window that same program run 
	=> 2 different instant
	- LPSTR or PSTR is UNICODE or ACSII
*/
/*
	- Windows procedure: part of event_driven programming system
	- wndProc take care of all the task that windows has to preform + message loop
	- Interact with the program eg. click button = message send to message loop; message loop process the message and sent to wndProc;
	wndProc take care of message => take the require action on the windows
*/
/*
	Windows calls WndProc when a window is first created. Windows calls WndProc when the window is
	eventually destroyed. Windows calls WndProc when the window has been resized or moved or
	minimized. Windows calls WndProc when a user clicks on the window with the mouse. Windows calls
	WndProc when characters are typed from the keyboard. Windows calls WndProc when an item has
	been selected from a menu. Windows calls WndProc when a scroll bar is manipulated or clicked with
	the mouse. Windows calls WndProc to tell it when it must repaint its client area.
*/
/*QUEUE MESSAGE AND NON-QUEUE MESSAGE:
	QUEUE MESSAGE: WM_KEYDOWN, WM_KEYUP, WM_CHAR, WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_TIMER, WM_PAINT, WM_QUIT
	NON-QUEUE MESSAGE: WM_CREATE, WM_SIZE, WM_SHOWWINDOW
*/
// The messages that Windows can send to a program are generally identified with names that begin with the letters WM and are defined in the WINUSER.H header file.

LRESULT CALLBACK winproc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR agrs, int cmd) { //WINAPI is calling convention: _stdcall; name must be WinMain
// Create windows class that hold data for the windows => reuse that class to create mutiple windows, use the same windows procedure
	WNDCLASS wc = {0}; // struct, member ??
	MSG msg = { 0 };
	static CHAR className[] = "SOMETHING";

	wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH); //type HBRUSH = painting inside; COLOR_WINDOW = default color of windows
	wc.hCursor = LoadCursor(NULL, IDC_HAND); // set cursor image https://docs.microsoft.com/en-us/windows/win32/learnwin32/setting-the-cursor-image
	wc.hInstance = hInstance;
	// when set customized icon from disk first argument must set to hInstance or else NULL
	wc.hIcon = LoadIcon(NULL,IDI_INFORMATION); //source: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadicona
	wc.lpszClassName = className; // class name / identify???
	wc.lpfnWndProc = winproc; // get win procedure
	if (!RegisterClassA(&wc)) { //return -1 depend on &wc value is true or false; pointer to a structure of type WNDCLASS
		MessageBoxA(NULL, "Error Not working", "TEST",0x10L);
		return -1; // unsuccess
	}
	
	/*
	if (!prevInstance)
	{
		wc.style = CS_HREDRAW | CS_VREDRAW;
		//[other wndclass initialization]
		RegisterClass(&wc);
	}
	*/
	CreateWindowA(className, "ab", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 600, NULL, NULL, NULL, NULL); //UTF-16
	//HWND at = CreateWindowA(className, "ab", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL); //UTF-16
	//ShowWindow(at,cmd);
	//UpdateWindow(at); // cause client area to be painter eg. "Hello,Windows"; windows send a window proc WM_PAINT message

	// The message loop: without message loop, the windows will appear in a short time and then disapear
	// the message loop and the window procedure do not run concurrently/simutalenously
	while (GetMessage(&msg, NULL, NULL, NULL)) //Obtain a message from the message queue (memory location of msg); get input like keybroad and mouse
	{
		// Read more: message queue
		TranslateMessage(&msg); //retrieve message from message queue and translate it
		DispatchMessage(&msg); //dispatch/send to windows and then windows send to appropriate winproc
		// WM_QUIT => msg = 0x0012 => get message return 0; 
	}
	return 0;
}

	// function will be use as procedure; HWND is windows handler receiving message; UINT unsigned interger
	/*
		The hwnd parameter lets the window procedure know which window is receiving the message
		(one window procedure could be handling messages for multiple windows base on one windows class);
		The message parameter (UINT structure) will = WM_SIZE then;
		WPARAM wp is = SIZE_RESTORED, SIZE_MINIMIZED, SIZE_MAXIMIZED, SIZE_MAXSHOW, or SIZE_MAXHIDE; (16 bits for old com; now 32 bits)???
		The lParam parameter contains the new size of the window. The new-width (16 bits) and the new-height (16 bits) stuck together in 32 bits lParam
	*/
	// WPARAM and LPARAM provide more information about the message
LRESULT CALLBACK winproc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) { //CALLING CONVENTION: callback _stdcall;
	HDC hdc; //Custormizable menu system; device context handler (handle to divice contex) refer to physical output (video/graphical)
	// HDC is the window's passport to the GDI function
	PAINTSTRUCT paint; // contains some information that a window procedure can use to painting the client area
	RECT rect; // Client Area (Free to draw anything); This structure has four LONG fields named left, top, right, and bottom
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0); //This function inserts a WM_QUIT message in the program's message queue; return 0 from wndproc; GetMessage return 0 => winmain drop out message loop
		break; 
	case WM_CREATE: 
		PlaySound("test.wav",NULL,SND_FILENAME | SND_LOOP | SND_ASYNC); //https://docs.microsoft.com/en-us/previous-versions/dd743680(v%3Dvs.85)
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &paint); // erase using hbrBackground brush; return to hdc
		GetClientRect(hWnd, &rect);
		DrawText(hdc, "Hello, Windows!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		// -1 to indicate that the text string is terminated with a zero character ???
		EndPaint(hWnd, &paint);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);// default processing of all messages that your wndproc does not process
	}

	//int a = GetWindowLongA(hWnd, msg);
	//AllocConsole();
}

/*	UPPERCASE IDENTIFIER:
		CS Class style option
		CW Create window option
		DT Draw text option
		IDI ID number for an icon
		IDC ID number for a cursor
		MB Message box options
		SND Sound option
		WM Window message
		WS Window style
*/
// A handle is simply a number (usually 32 bits in size) that refers to an object
/*
	Sometimes messages generate other messages as a result of DefWindowProc processing. For
example, suppose you run HELLOWIN and you eventually click the Close button, or suppose you
select Close from the system menu using either the keyboard or the mouse. DefWindowProc
processes this keyboard or mouse input. When it detects that you have selected the Close option, it
sends a WM_SYSCOMMAND message to the window procedure. WndProc passes this message to
DefWindowProc. DefWindowProc responds by sending a WM_CLOSE message to the window
procedure. WndProc again passes this message to DefWindowProc. DefWindowProc responds to the
WM_CLOSE message by calling DestroyWindow. DestroyWindow causes Windows to send a
WM_DESTROY message to the window procedure. WndProc finally responds to this message by
calling PostQuitMessage to put a WM_QUIT message in the message queue. This message causes
the message loop in WinMain to terminate and the program to end.
*/

/*
	See more at : SendMessage; DefWindowProc; UpdateWindow; DestroyWindow
	fopen: function to open a file; page 73, 77
	Graphics Device Interface (GDI)
	Learning about: the basics of device-independent programming
*/
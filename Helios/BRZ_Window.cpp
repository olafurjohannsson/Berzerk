
#include "BRZ_Window.h"

/*
	LRESULT CALLBACK WindowDispatch(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	class Window
	{
	public:
		Window(unsigned int iconResource);
		~Window();

	public:
		BRZRESULT	Construct(const BRZSTRING & title, unsigned int x_Size, unsigned int y_Size);
		BRZRESULT	Place(unsigned int x_Pos, unsigned int y_Pos);
		BRZRESULT	Message(unsigned int message, unsigned int wParam, unsigned int lParam);

		bool				Focused() const;
		bool				Active() const;
		HWND				Handle() const; 
		const BRZSTRING &	Title() const;

	protected:
	private:
		HWND			wnd_handle;
//		WNDCLASSEX		wnd_class;

		BRZSTRING		title;
		unsigned int	icon;
		unsigned int	x_Pos;
		unsigned int	y_Pos;
		unsigned int	x_Size;
		unsigned int	y_Size;
		bool			visible;
		bool			focused;

	private:
		static BRZRESULT	DeclareClass(unsigned int icon);
		static bool			isReg;
	};
*/


BRZRESULT BRZ::Window::Construct(const BRZSTRING & A_title, unsigned int A_x, unsigned int A_y)
{
	// Create the window:
	this->wnd_handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"BRZWIN",
		A_title.c_str(),
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, (A_x + 20), (A_y + 42),
		NULL, NULL, ::GetModuleHandle(NULL), NULL);


	if(this->wnd_handle == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed! [ASCWIN]", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return BRZ_FAILURE;
    }

	this->Place(100, 100);
	title = A_title;
	x_Size = A_x;
	y_Size = A_y;

	ShowWindow(this->wnd_handle, SW_SHOWNORMAL);
    UpdateWindow(this->wnd_handle);

	// Link the HWND to the ASC::Window object:
	::SetWindowLong(this->wnd_handle, GWL_USERDATA, (unsigned int)this);

	visible = true;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Window::Place(unsigned int A_x, unsigned int A_y)
{
	::SetWindowPos(wnd_handle, NULL, A_x, A_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	x_Pos = A_x;
	y_Pos = A_y;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Window::Message(unsigned int A_msg, unsigned int A_wParam, unsigned int A_lParam)
{
	if (A_msg == WM_CLOSE)
		visible = false;
	else if (A_msg == WM_ACTIVATE)
	{
		if (A_wParam == WA_INACTIVE)
			focused = false;
		else
			focused = true;
	}

	return BRZ_SUCCESS;
}


bool BRZ::Window::Focused() const
{
	return focused;
}


bool BRZ::Window::Active() const
{
	return visible;
}


const BRZSTRING & BRZ::Window::Title() const
{
	return title;
}


HWND BRZ::Window::Handle() const
{
	return wnd_handle;
}



BRZ::Window::Window(unsigned int A_ico) :	
	wnd_handle(NULL),
	title(L""),
	icon(A_ico),
	x_Pos(0),
	y_Pos(0),
	x_Size(0),
	y_Size(0),
	visible(false),
	focused(false)
{
	BRZ::Window::DeclareClass(icon);
}

BRZ::Window::~Window()
{
	if (this->Active())
		::DestroyWindow(wnd_handle);
}




bool BRZ::Window::isReg = false;

BRZRESULT BRZ::Window::DeclareClass(unsigned int A_icon)
{
	if (BRZ::Window::isReg)
		return BRZ_SUCCESS;

	WNDCLASSEX		wnd_class;
	HICON			winIcon = NULL;
	BRZSTRING		className = L"BRZWIN";

	BRZ::ZeroMem(&wnd_class);

	if (A_icon != 0)
	{
		winIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(A_icon));
		if (winIcon == NULL)
		{
			// Note: Error -- Failed to load icon.
		}
	}

	// Setting up the window class:
	wnd_class.cbSize        = sizeof(WNDCLASSEX);
    wnd_class.style         = 0;
	wnd_class.lpfnWndProc   = BRZ::Window::Dispatch;
    wnd_class.cbClsExtra    = 0;
    wnd_class.cbWndExtra    = sizeof(void *);
	wnd_class.hInstance     = ::GetModuleHandle(NULL);
    wnd_class.hIcon         = winIcon;
    wnd_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wnd_class.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wnd_class.lpszMenuName  = NULL;
    wnd_class.lpszClassName = className.c_str();
    wnd_class.hIconSm       = NULL;

	if(!RegisterClassEx(&wnd_class))
		return BRZ_FAILURE;

	BRZ::Window::isReg = true;

	return BRZ_SUCCESS;
};


LRESULT CALLBACK BRZ::Window::Dispatch(HWND A_hwnd, UINT A_msg, WPARAM A_wParam, LPARAM A_lParam)
{
	BRZ::Window * wind = (BRZ::Window *)::GetWindowLong(A_hwnd, GWL_USERDATA);
	if (wind != NULL)
		wind->Message(A_msg, A_wParam, A_lParam);

    switch(A_msg)
    {
		// Some default behaviour for windows:
        case WM_CLOSE:
            DestroyWindow(A_hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(A_hwnd, A_msg, A_wParam, A_lParam);
    }
    return 0;
}
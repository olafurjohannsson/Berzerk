
#pragma once;
#ifdef BRZ_HEADERTRACE
#pragma message("Header Trace: BRZ_Window.h")
#endif

#include "BRZ_Define.h"
#include <Windows.h>

namespace BRZ
{
	class Window
	{
	public:
		Window(unsigned int iconResource = 0);
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
		static LRESULT CALLBACK		Dispatch(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static BRZRESULT			DeclareClass(unsigned int icon);
		static bool					isReg;
	};
}
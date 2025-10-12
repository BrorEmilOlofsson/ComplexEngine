#include "Engine/Precompiled/EnginePch.hpp"
#include "WinWindowClass.hpp"

#ifdef _WIN32

namespace Simple
{

	Win_WindowClass::Win_WindowClass(HINSTANCE hInstance, std::wstring name, LRESULT(*winProc)(HWND, UINT, WPARAM, LPARAM))
		: mInstance(hInstance)
		, mName(std::move(name))
	{
		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_OWNDC;
		wcex.lpfnWndProc = winProc;
		wcex.hInstance = mInstance;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszClassName = mName.c_str();
		wcex.hIcon = LoadIcon(mInstance, MAKEINTRESOURCE(101));
		wcex.hIconSm = LoadIcon(mInstance, MAKEINTRESOURCE(101));

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(nullptr, L"Failed to register window class", L"Error", MB_ICONERROR);
			assert(false);
		}
	}

	Win_WindowClass::~Win_WindowClass()
	{
		UnregisterClass(mName.c_str(), mInstance);
	}

	HINSTANCE Win_WindowClass::GetInstance() const
	{
		return mInstance;
	}

	std::wstring_view Win_WindowClass::GetName() const
	{
		return mName.c_str();
	}
}

#endif
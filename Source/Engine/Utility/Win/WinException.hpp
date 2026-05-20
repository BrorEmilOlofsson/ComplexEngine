#pragma once

#ifdef _WIN32

#define NOMINMAX

#include <exception>
#include <string>
#include <Windows.h>

namespace CLX
{

	class WinException : public std::exception
	{
	public:

		WinException(unsigned int line, const char* file, HRESULT hr);


		const char* what() const noexcept override
		{
			return mWhatString.c_str();
		}


	private:

		unsigned int mLine = 0;
		std::string mFile;
		HRESULT mHR;
		std::string mWhatString;
	};
}

#define WIN_EXCEPTION(hr) CLX::WinException(__LINE__, __FILE__, hr);
#define WIN_CHECK_HRESULT(hr) if (FAILED(hr)) throw WIN_EXCEPTION(hr);

#endif
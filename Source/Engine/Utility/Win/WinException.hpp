#pragma once
#include <exception>
#include <string>

#ifdef _WIN32
#include <Windows.h>

namespace Simple
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

#define WIN_EXCEPTION(hr) Simple::WinException(__LINE__, __FILE__, hr);
#define WIN_CHECK_HRESULT(hr) if (FAILED(hr)) throw WIN_EXCEPTION(hr);

#endif
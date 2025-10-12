#include "Utility/Precompiled/UtilityPch.hpp"
#include "WinException.hpp"

#ifdef _WIN32

namespace Simple
{

	static std::string TranslateHRESULT(HRESULT hr)
	{
		char* msgBuf = nullptr;
		DWORD msgLength = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&msgBuf),
			0,
			nullptr
		);

		if (msgLength == 0)
		{
			return "Unidentified error code";
		}

		std::string errorString = msgBuf;
		LocalFree(msgBuf);
		return errorString;
	}
	
	WinException::WinException(unsigned int line, const char* file, HRESULT hr)
		: mLine(line)
		, mFile(file)
		, mHR(hr)
	{
		mWhatString = TranslateHRESULT(hr);
	}
}

#endif
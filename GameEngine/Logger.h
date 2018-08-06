#pragma once
#include <fstream>

typedef long HRESULT;

#define FAILED(hr) (((HRESULT)(hr)) < 0)

template  <class Var>
inline void Log(Var x)
{
	std::ofstream fout("Logger.txt", std::ofstream::app);
	fout << x << "\n";	
	fout.close();
}

inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch Win32 API errors.
			//throw Platform::Exception::CreateException(hr);
		}
	}
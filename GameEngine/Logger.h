#pragma once
#include <fstream>
#include<mutex>

class Logger
{
	//Data
private:
	static std::ofstream _fout;
	static std::mutex _mx;

public:
	template  <typename Var>
	static void Log(Var x)
	{
		std::lock_guard<std::mutex> lock(_mx);
		_fout.open("Logger.txt", std::ofstream::app);
		_fout << x << "\n";
		_fout.close();
	}
};
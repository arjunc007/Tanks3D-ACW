#include "Logger.h"

std::ofstream Logger::_fout;
bool Logger::_fileOpen = false;
std::mutex Logger::_mx;
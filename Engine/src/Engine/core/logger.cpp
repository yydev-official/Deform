#include "Engine.h"

#include <cstdio>
#include <string>
#include <vector>
#include <mutex>
#include <winuser.h>

std::vector<std::string> deform::Logger::s_Logs;
std::mutex deform::Logger::s_LogMutex;

void deform::Logger::Log(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(s_LogMutex);
    s_Logs.push_back(msg);
	printf("[LOG] %s\n", msg.c_str());
}

void deform::Logger::FatalError(const char* message)
{
    MessageBoxA(nullptr, message, "Fatal Error", MB_ICONERROR | MB_OK);
    printf("[LOG] %s\n", message);
}

const std::vector<std::string>& deform::Logger::GetLogs()
{
    std::lock_guard<std::mutex> lock(s_LogMutex);
    return s_Logs;
}

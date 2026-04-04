#include "../../Engine.h"

std::vector<std::string> deform::Logger::s_Logs;

void deform::Logger::Log(const std::string& msg)
{
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
    return s_Logs;
}

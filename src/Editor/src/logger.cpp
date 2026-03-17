#include "../Editor.h"

std::vector<std::string> Editor::Logger::s_Logs;

void Editor::Logger::Log(const std::string& msg)
{
    s_Logs.push_back(msg);
}

void Editor::Logger::FatalError(const char* message)
{
    MessageBoxA(nullptr, message, "Fatal Error", MB_ICONERROR | MB_OK);
}

const std::vector<std::string>& Editor::Logger::GetLogs()
{
    return s_Logs;
}
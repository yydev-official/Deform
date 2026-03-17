#include "../Editor.h"

std::vector<std::string> Editor::Logger::s_Logs;

void Editor::Logger::Log(const std::string& msg)
{
    s_Logs.push_back(msg);
}

const std::vector<std::string>& Editor::Logger::GetLogs()
{
    return s_Logs;
}
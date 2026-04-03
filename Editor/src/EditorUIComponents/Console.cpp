#include "../Editor.h"

void Editor::UI::Console::UpdateConsole()
{
    ImGui::Begin("Console");
    for (const auto& log : deform::Logger::GetLogs())
        ImGui::Text("%s", log.c_str());
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::End();
}
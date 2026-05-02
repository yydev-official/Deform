#include "../Editor.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

// ─────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────
static std::string GetFileIcon(const std::string& ext)
{
    if (ext == ".lua")  return "[LUA]";
    if (ext == ".hlsl" || ext == ".glsl") return "[SHD]";
    if (ext == ".png"  || ext == ".jpg")  return "[IMG]";
    if (ext == ".obj"  || ext == ".fbx")  return "[MDL]";
    if (ext == ".mp3"  || ext == ".wav")  return "[SND]";
    return "[FILE]";
}

static std::string ReadFile(const std::string& path)
{
    std::ifstream f(path);
    if (!f.is_open()) return "";
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// ─────────────────────────────────────────────
//  Assets Panel
// ─────────────────────────────────────────────
void Editor::UI::Assets::UpdateAssets(UIState& state)
{
    ImGui::Begin("Assets", nullptr);

    // ── Toolbar ─────────────────────────────────────
    if (ImGui::Button("< Back") && state.assets.currentPath != state.assets.rootPath)
    {
        state.assets.currentPath = state.assets.currentPath.parent_path();
        state.assets.selectedFile.clear();
    }

    ImGui::SameLine();
    ImGui::TextUnformatted(
        fs::relative(state.assets.currentPath, state.assets.rootPath)
            .string().c_str()
    );

    // Refresh button
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 60);
    if (ImGui::Button("Refresh"))
        state.assets.selectedFile.clear();

    ImGui::Separator();

    // ── Left: file browser ──────────────────────────
    float browserWidth = 220.0f;
    ImGui::BeginChild("##asset_browser", {browserWidth, 0}, true);

    if (!fs::exists(state.assets.currentPath))
    {
        ImGui::TextUnformatted("assets/ folder not found.");
        ImGui::EndChild();
        ImGui::End();
        return;
    }

    // Folders first
    for (auto& entry : fs::directory_iterator(state.assets.currentPath))
    {
        if (!entry.is_directory()) continue;
        std::string name = entry.path().filename().string();
        if (ImGui::Selectable(("[DIR] " + name).c_str(), false))
            state.assets.currentPath = entry.path();
    }

    // Files
    for (auto& entry : fs::directory_iterator(state.assets.currentPath))
    {
        if (!entry.is_file()) continue;
        std::string name = entry.path().filename().string();
        std::string ext  = entry.path().extension().string();
        std::string label = GetFileIcon(ext) + " " + name;
        bool selected = (state.assets.selectedFile == entry.path().string());

        if (ImGui::Selectable(label.c_str(), selected))
        {
            state.assets.selectedFile   = entry.path().string();
            state.assets.fileContent    = ReadFile(state.assets.selectedFile);
            state.assets.editedContent  = state.assets.fileContent;
        }
    }

    ImGui::EndChild();

    // ── Right: file viewer / editor ─────────────────
    ImGui::SameLine();
    ImGui::BeginChild("##asset_viewer", {0, 0}, true);

    if (state.assets.selectedFile.empty())
    {
        ImGui::TextUnformatted("Select a file to view.");
    }
    else
    {
        fs::path p(state.assets.selectedFile);
        std::string ext = p.extension().string();

        ImGui::Text("%s  %s", GetFileIcon(ext).c_str(),
                    p.filename().string().c_str());
        ImGui::Separator();

        bool isText = (ext == ".lua"  || ext == ".hlsl" ||
                       ext == ".glsl" || ext == ".txt"  ||
                       ext == ".json" || ext == ".md");

        if (isText)
        {
            // Editable text box
            static char editBuf[1024 * 64] = {};
            if (state.assets.editedContent.size() < sizeof(editBuf))
                std::memcpy(editBuf, state.assets.editedContent.c_str(),
                            state.assets.editedContent.size() + 1);

            float availH = ImGui::GetContentRegionAvail().y - 32;
            ImGui::InputTextMultiline("##file_editor", editBuf, sizeof(editBuf),
                                      {-1, availH});

            state.assets.editedContent = editBuf;

            // Save button
            if (ImGui::Button("Save"))
            {
                std::ofstream f(state.assets.selectedFile);
                f << state.assets.editedContent;
                state.assets.fileContent = state.assets.editedContent;
            }

            // Run Lua button
            if (ext == ".lua")
            {
                ImGui::SameLine();
                if (ImGui::Button("Run Script") && state.luaState)
                    state.luaState->ExecuteFile(state.assets.selectedFile);
            }
        }
        else
        {
            ImGui::TextUnformatted("(Binary file — preview not available)");
        }
    }

    ImGui::EndChild();
    ImGui::End();
}

#include "imgui.h"

#include "ModelManager.h"

#include "imfilebrowser.h"
#include "../rendering/Renderer.h"
#include <filesystem>

//Definition of the instance
PAG::ModelManager* PAG::ModelManager::instance = nullptr;

PAG::ModelManager::ModelManager(): GUIElement("Model manager") {
    _selectedModel = 0;
    _selectedMaterial = 0;
}

/**
 * @brief Method that wakes up all subscribers to the events of this class
 */
void PAG::ModelManager::warnListeners() const
{
    for (auto listener: _listeners) {
        listener->wakeUp(WindowType::ModelEditor, &_package);
    }
}

/**
 * @brief Destructor of this class
 */
PAG::ModelManager::~ModelManager() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

/**
 * @brief Method that creates (first time is called) and return the only instance for this class
 * @return The only instance for this class
 */
PAG::ModelManager* PAG::ModelManager::getInstance()
{
    if (!instance)
        instance = new ModelManager();
    return instance;
}

/**
 * @brief Method that renders the GUI window for transformations on models
 */
void PAG::ModelManager::render()
{
    if (visible) {
        if (ImGui::Begin(title.c_str(), &visible)) {
            //Get model names currently in the app
            auto modelNames = Renderer::getInstance()->getModelNames();
            if (modelNames.empty()) {
                ImGui::Text("No models loaded yet");
                ImGui::End();
                return;
            }

            std::vector<const char*> cModelNames;
            cModelNames.reserve(modelNames.size());
            for (const auto& name : modelNames) {
                cModelNames.push_back(name.c_str());
            }

            // Safety check
            if (_selectedModel >= cModelNames.size()) {
                _selectedModel = 0;
            }

            static int _lastSelectedModel = -1;

            ImGui::Combo("Model", &_selectedModel, cModelNames.data(), cModelNames.size());

            if (_selectedModel != _lastSelectedModel) {

                int currentMatID = Renderer::getInstance()->getIdMaterialModel(_selectedModel);

                _selectedMaterial = currentMatID;

                _lastSelectedModel = _selectedModel;
            }

            ImGui::Separator();

            //Transformations
            const char* transformTypes[] = { "Translation", "Rotation", "Scale" };
            ImGui::Combo("Tipo", &_selectedTransformation, transformTypes, IM_ARRAYSIZE(transformTypes));

            switch (_selectedTransformation) {
                case 0:
                {
                    ImGui::InputFloat3("Translation (X, Y, Z)", _translateVec);
                    break;
                }
                case 1:
                {
                    ImGui::InputFloat("Angle (Degrees)", &_rotateAngle);
                    const char* axes[] = { "X axis", "Y axis", "Z axis" };
                    ImGui::Combo("Eje", &_rotateAxis, axes, IM_ARRAYSIZE(axes));
                    break;
                }
                case 2:
                {
                    ImGui::InputFloat3("Scale (X, Y, Z)", _scaleVec);
                    break;
                }
            }


            //Apply changes on transformations
            if (ImGui::Button("Apply")) {
                _package.modelId = _selectedModel;

                switch (_selectedTransformation) {
                    case 0:
                    {
                        _package.type = ModelEditType::TRANSLATE;
                        _package.transf = glm::vec3(_translateVec[0], _translateVec[1], _translateVec[2]);
                        break;
                    }
                    case 1:
                    {
                        _package.type = ModelEditType::ROTATE;
                        if (_rotateAxis == 0) _package.transf = glm::vec3(1.0f, 0.0f, 0.0f); // X
                        if (_rotateAxis == 1) _package.transf = glm::vec3(0.0f, 1.0f, 0.0f); // Y
                        if (_rotateAxis == 2) _package.transf = glm::vec3(0.0f, 0.0f, 1.0f); // Z
                        _package.angleDegrees = _rotateAngle;
                        break;
                    }
                    case 2:
                    {
                        _package.type = ModelEditType::SCALE;
                        _package.transf = glm::vec3(_scaleVec[0], _scaleVec[1], _scaleVec[2]);
                        break;
                    }
                }

                warnListeners();
            }

            //Reset model
            ImGui::SameLine();
            if (ImGui::Button("Reset model")) {
                _package.modelId = _selectedModel;
                _package.type = ModelEditType::RESET;
                _package.transf = glm::vec3(0.0f);
                _package.angleDegrees = 0.0f;

                warnListeners();
            }

            //Delete model
            if (ImGui::Button("Delete model")) {
                ImGui::OpenPopup("Confirm elimination");
            }

            // Popup window for deleting model
            if (ImGui::BeginPopupModal("Confirm elimination", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("¿Are you sure you want to delete '%s'?", cModelNames[_selectedModel]);
                ImGui::Separator();
                if (ImGui::Button("Yes, delete", ImVec2(120, 0))) {
                    _package.modelId = _selectedModel;
                    _package.type = ModelEditType::DELETE;

                    _selectedModel = 0;
                    _lastSelectedModel = -1;

                    warnListeners();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            ImGui::Separator();

            ImGui::Text("Material assigment");

            auto materialNames = Renderer::getInstance()->getMaterialNames();
            std::vector<const char*> cMatNames;

            for (const auto& name : materialNames) {
                cMatNames.push_back(name.c_str());
            }

            // Assign material button
            ImGui::Combo("Material", &_selectedMaterial, cMatNames.data(), cMatNames.size());

            ImGui::SameLine();
            if (ImGui::Button("Assign")) {
                _package.type = ModelEditType::MATERIAL_ASSIGN;
                _package.modelId = _selectedModel;

                _package.materialId = _selectedMaterial;

                warnListeners();
            }

            ImGui::Separator();
            ImGui::Text("Texture");

            std::string currentTexturePath = Renderer::getInstance()->getTextureModel(_selectedModel);

            if (currentTexturePath.empty())
                ImGui::Text("Current Texture: [None]");
            else {
                std::filesystem::path p (currentTexturePath);
                ImGui::Text("Current texture: %s", p.filename().string().c_str());

                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("%s", currentTexturePath.c_str());
            }

            static ImGui::FileBrowser fileDialogTexture;
            fileDialogTexture.SetTitle("Select a texture");
            fileDialogTexture.SetTypeFilters({".png"});

            if (ImGui::Button("Load texture...")) {
                fileDialogTexture.Open();
            }

            fileDialogTexture.Display();

            if (fileDialogTexture.HasSelected()) {
                std::string path = fileDialogTexture.GetSelected().string();
                fileDialogTexture.ClearSelected();

                _package.modelId = _selectedModel;
                _package.type = ModelEditType::TEXTURE_ASSIGN;
                _package.texturePath = path;

                warnListeners();
            }

            static ImGui::FileBrowser fileDialogNormalMap;
            fileDialogNormalMap.SetTitle("Select a normal map");
            fileDialogNormalMap.SetTypeFilters({".png", ".jpg"});

            if (ImGui::Button("Load normal map...")) {
                fileDialogNormalMap.Open();
            }

            fileDialogNormalMap.Display();

            if (fileDialogNormalMap.HasSelected()) {
                std::string path = fileDialogNormalMap.GetSelected().string();
                fileDialogNormalMap.ClearSelected();

                _package.modelId = _selectedModel;
                _package.type = ModelEditType::NORMAL_MAP_ASSIGN;
                _package.texturePath = path;

                warnListeners();
            }
        }
        ImGui::End();
    }
}
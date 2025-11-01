#include "imgui.h"

#include "ModelTransformationWindow.h"
#include "../rendering/Renderer.h"

//Definition of the instance
PAG::ModelTransformationWindow* PAG::ModelTransformationWindow::instance = nullptr;

void PAG::ModelTransformationWindow::warnListeners() const
{
    for (auto listener: _listeners) {
        listener->wakeUp(WindowType::ModelTransformation, _package);
    }
}

PAG::ModelTransformationWindow* PAG::ModelTransformationWindow::getInstance()
{
    if (!instance)
        instance = new ModelTransformationWindow();
    return instance;
}

void PAG::ModelTransformationWindow::render()
{
    ImGui::Begin("Model transformation");

    auto modelNames = Renderer::getInstance()->getModelNames();
    if (modelNames.empty()) {
        ImGui::Text("No models loaded yet");
        ImGui::End();
        return;
    }
    std::vector<const char*> cNames;
    cNames.reserve(modelNames.size());
    for (const auto& name : modelNames) {
        cNames.push_back(name.c_str());
    }
    if (_selectedModel >= cNames.size()) {
        _selectedModel = 0;
    }
    ImGui::Combo("Model", &_selectedModel, cNames.data(), cNames.size());
    ImGui::Separator();

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


    if (ImGui::Button("Apply")) {
        _package.modelId = _selectedModel;

        switch (_selectedTransformation) {
            case 0:
                {
                    _package.type = TransformType::TRANSLATE;
                    _package.transf = glm::vec3(_translateVec[0], _translateVec[1], _translateVec[2]);
                    break;
                }
            case 1:
                {
                    _package.type = TransformType::ROTATE;
                    if (_rotateAxis == 0) _package.transf = glm::vec3(1.0f, 0.0f, 0.0f); // X
                    if (_rotateAxis == 1) _package.transf = glm::vec3(0.0f, 1.0f, 0.0f); // Y
                    if (_rotateAxis == 2) _package.transf = glm::vec3(0.0f, 0.0f, 1.0f); // Z
                    _package.angleDegrees = _rotateAngle;
                    break;
                }
            case 2:
                {
                    _package.type = TransformType::SCALE;
                    _package.transf = glm::vec3(_scaleVec[0], _scaleVec[1], _scaleVec[2]);
                    break;
                }
        }

        warnListeners();
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset model")) {
        _package.modelId = _selectedModel;
        _package.type = TransformType::RESET;
        _package.transf = glm::vec3(0.0f);
        _package.angleDegrees = 0.0f;

        warnListeners();
    }

    ImGui::End();
}
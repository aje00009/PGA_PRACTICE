#include "imgui.h"

#include "LightManager.h"

#include <glm/gtc/type_ptr.hpp>

#include "../rendering/Renderer.h"

PAG::LightManager* PAG::LightManager::instance = nullptr;

void PAG::LightManager::warnListeners() {
    _payload.name = _nameBuffer;

    for (auto listener: _listeners) {
        listener->wakeUp(WindowType::ManagerLight,&_payload);
    }
}

PAG::LightManager::LightManager() {
    _payload.lightId = -1;
    _payload.type = LightType::POINT_LIGHT;
}

PAG::LightManager::~LightManager() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

PAG::LightManager * PAG::LightManager::getInstance() {
    if (!instance) {
        instance = new LightManager();
    }
    return instance;
}

void PAG::LightManager::render() {
    ImGui::Begin("Light Manager");

    // Light Selector
    auto lightNames = Renderer::getInstance()->getLightNames();
    std::vector<const char*> cNames;
    cNames.push_back("[ CREAR NUEVA LUZ ]");
    for (const auto& name : lightNames) {
        cNames.push_back(name.c_str());
    }

    // If list is changed, we need to reload
    if (_selectedLight >= cNames.size()) {
        _selectedLight = 0;
        _lastSelected = -1;
    }

    if (ImGui::Combo("Selected light", &_selectedLight, cNames.data(), cNames.size())) {
        _lastSelected = -1;
    }

    if (_selectedLight != _lastSelected) {
        if (_selectedLight == 0) {
            // Create new light mode
            _payload = LightPackage(); // Reset default values
            _payload.lightId = -1;
            strcpy(_nameBuffer, "New light");
        } else {
            // Edit light mode
            int realIndex = _selectedLight - 1;
            Light* light = Renderer::getInstance()->getLight(realIndex);
            LightProperties* props = light->getLightProperties();

            // Copy data from existing light to payload
            _payload.lightId = realIndex;
            _payload.name = props->getName();
            _payload.type = light->getType();
            _payload.isEnabled = props->isEnabled();
            _payload.ambient = props->getIa();
            _payload.diffuse = props->getId();
            _payload.specular = props->getIs();
            _payload.position = props->getPos();
            _payload.direction = props->getDirection();
            _payload.angle = props->getAngle();
            _payload.exp = props->getExponent();
            strcpy(_nameBuffer, props->getName().c_str());
        }
        _lastSelected = _selectedLight;
    }

    ImGui::Separator();

    // Property editor
    bool changed = false;

    ImGui::InputText("Name", _nameBuffer, 128);
    changed |= ImGui::Checkbox("On", &_payload.isEnabled);

    const char* lightTypes[] = { "Ambient", "Point", "Directional", "Spot" };
    int typeIndex = static_cast<int>(_payload.type);
    if (ImGui::Combo("Type", &typeIndex, lightTypes, 4)) {
        _payload.type = static_cast<LightType>(typeIndex);
        changed = true;
    }

    // Dynamic controls depending on light type
    if (ImGui::ColorEdit3("Ambient (Ia)", glm::value_ptr(_payload.ambient))) changed = true;

    if (_payload.type != LightType::AMBIENT_LIGHT) {
        if (ImGui::ColorEdit3("Diffuse (Id)", glm::value_ptr(_payload.diffuse))) changed = true;
        if (ImGui::ColorEdit3("Specular (Is)", glm::value_ptr(_payload.specular))) changed = true;
    }

    if (_payload.type == LightType::POINT_LIGHT || _payload.type == LightType::SPOT_LIGHT) {
        if (ImGui::DragFloat3("Position", glm::value_ptr(_payload.position), 0.1f)) changed = true;
    }

    if (_payload.type == LightType::DIRECTIONAL_LIGHT || _payload.type == LightType::SPOT_LIGHT) {
        if (ImGui::DragFloat3("Direction", glm::value_ptr(_payload.direction), 0.01f)) changed = true;
    }

    if (_payload.type == LightType::SPOT_LIGHT) {
        if (ImGui::SliderFloat("Angle (Gamma)", &_payload.angle, 0.0f, 90.0f)) changed = true;
        if (ImGui::SliderFloat("Exponent (s)", &_payload.exp, 0.0f, 128.0f)) changed = true;
    }

    ImGui::Separator();

    // Action buttons
    if (_selectedLight == 0) {
        if (ImGui::Button("Create light")) {
            _isDelete = false;
            warnListeners();
            // Make selected light the one we have just created
            _selectedLight = cNames.size();
            _lastSelected = -1;
        }
    } else {
        if (ImGui::Button("Save")) {
            changed = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            ImGui::OpenPopup("Confirm light delete");
        }
    }

    if (changed && _selectedLight != 0) {
        _isDelete = false;
        warnListeners();
    }

    // Confirmation popup
    if (ImGui::BeginPopupModal("Confirm light delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("¿Sure you wanna delete '%s'?", _payload.name.c_str());
        ImGui::Separator();

        if (ImGui::Button("Yes, delete", ImVec2(120, 0))) {
            _isDelete = true;
            warnListeners();
            _selectedLight = 0;
            _lastSelected = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

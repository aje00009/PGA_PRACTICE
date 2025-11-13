#include "imgui.h"

#include "LightManager.h"

#include <glm/gtc/type_ptr.hpp>

#include "../rendering/Renderer.h"

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
    ImGui::Begin("Gestor de Luces");

    // --- 1. Selector de Luz ---
    // (¡Necesitarás crear getLightNames() en tu Renderer!)
    auto lightNames = Renderer::getInstance()->getLightNames();
    std::vector<const char*> cNames;
    cNames.push_back("[ CREAR NUEVA LUZ ]");
    for (const auto& name : lightNames) {
        cNames.push_back(name.c_str());
    }

    // Si la lista de luces cambia (ej. borramos una), ajustamos el índice
    if (_selectedLight >= cNames.size()) {
        _selectedLight = 0;
        _lastSelected = -1; // Forzar recarga
    }

    if (ImGui::Combo("Luz Seleccionada", &_selectedLight, cNames.data(), cNames.size())) {
        _lastSelected = -1; // El usuario ha cambiado la selección, forzar recarga
    }

    if (_selectedLight != _lastSelected) {
        if (_selectedLight == 0) {
            // --- Modo "Crear Nuevo" ---
            _payload = LightPackage(); // Resetea a valores por defecto
            _payload.lightId = -1;
            strcpy(_nameBuffer, "Nueva Luz");
        } else {
            // --- Modo "Editar" ---
            int realIndex = _selectedLight - 1;
            // (¡Necesitarás crear getLightProperties(index) en tu Renderer!)
            LightProperties* props = Renderer::getInstance()->getLightProperties(realIndex);
            if (props) {
                // Copiamos los datos de la luz existente al payload
                _payload.lightId = realIndex;
                _payload.name = props->_name;
                _payload.type = props->_type;
                _payload.isEnabled = props->_isEnabled;
                _payload.ambient = props->_ambient;
                _payload.diffuse = props->_diffuse;
                _payload.specular = props->_specular;
                _payload.position = props->_position;
                _payload.direction = props->_direction;
                _payload.angle = props->_gamma;
                _payload.exp = props->_s;
                strcpy(_nameBuffer, props->_name.c_str());
            }
        }
        _lastSelected = _selectedLight;
    }

    ImGui::Separator();

    // --- 3. Editores de Propiedades (con "Live Update") ---
    bool changed = false; // Flag para detectar si algo cambia

    ImGui::InputText("Nombre", _nameBuffer, 128);
    changed |= ImGui::Checkbox("Activada", &_payload.isEnabled);

    const char* lightTypes[] = { "Ambiente", "Puntual", "Direccional", "Foco" };
    int typeIndex = static_cast<int>(_payload.type);
    if (ImGui::Combo("Tipo", &typeIndex, lightTypes, 4)) {
        _payload.type = static_cast<LightType>(typeIndex);
        changed = true;
    }

    // --- Controles dinámicos según el tipo de luz ---
    if (ImGui::ColorEdit3("Ambiente (Ia)", glm::value_ptr(_payload.ambient))) changed = true;

    if (_payload.type != LightType::AMBIENT_LIGHT) {
        if (ImGui::ColorEdit3("Difusa (Id)", glm::value_ptr(_payload.diffuse))) changed = true;
        if (ImGui::ColorEdit3("Especular (Is)", glm::value_ptr(_payload.specular))) changed = true;
    }

    if (_payload.type == LightType::POINT_LIGHT || _payload.type == LightType::SPOT_LIGHT) {
        if (ImGui::DragFloat3("Posición", glm::value_ptr(_payload.position), 0.1f)) changed = true;
    }

    if (_payload.type == LightType::DIRECTIONAL_LIGHT || _payload.type == LightType::SPOT_LIGHT) {
        if (ImGui::DragFloat3("Dirección", glm::value_ptr(_payload.direction), 0.01f)) changed = true;
    }

    if (_payload.type == LightType::SPOT_LIGHT) {
        if (ImGui::SliderFloat("Ángulo (Gamma)", &_payload.angle, 0.0f, 90.0f)) changed = true;
        if (ImGui::SliderFloat("Exp. (s)", &_payload.exp, 0.0f, 128.0f)) changed = true;
    }

    ImGui::Separator();

    // --- 4. Botones de Acción ---
    if (_selectedLight == 0) {
        // --- Modo "Crear" ---
        if (ImGui::Button("Crear Luz")) {
            _isDelete = false;
            warnListeners();
            // Hacemos que se seleccione la luz que acabamos de crear
            _selectedLight = cNames.size();
            _lastSelected = -1; // Forzar recarga en el próximo frame
        }
    } else {
        // --- Modo "Editar" ---
        // El botón de "Guardar" es solo para el nombre (los sliders se guardan solos)
        if (ImGui::Button("Guardar Nombre")) {
            changed = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Eliminar")) {
            ImGui::OpenPopup("Confirmar Borrado Luz");
        }
    }

    // Si cualquier control cambió, notificar al Renderer
    // (Solo en modo Editar, para no enviar spam mientras se crea una nueva)
    if (changed && _selectedLight != 0) {
        _isDelete = false;
        warnListeners();
    }

    // --- 5. Popup de Confirmación ---
    if (ImGui::BeginPopupModal("Confirmar Borrado Luz", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("¿Seguro que quieres eliminar '%s'?", _payload.name.c_str());
        ImGui::Separator();

        if (ImGui::Button("Si, Eliminar", ImVec2(120, 0))) {
            _isDelete = true;
            warnListeners(); // ¡Notificar con "delete=true"!
            _selectedLight = 0; // Volver a "Crear Nuevo"
            _lastSelected = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancelar", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

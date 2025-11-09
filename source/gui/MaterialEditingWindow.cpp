#include "imgui.h"

#include "../rendering/Renderer.h"
#include "MaterialEditingWindow.h"

//Definition of the instance
PAG::MaterialEditingWindow* PAG::MaterialEditingWindow::instance = nullptr;

/**
 * @brief Method that wakes up all listeners to the events of this window
 */
void PAG::MaterialEditingWindow::warnListeners() {
    _package.name = _nameBuffer;

    for (auto& listener : _listeners) {
        listener->wakeUp(WindowType::MaterialEditor,&_package);
    }
}

/**
 * @brief Destructor of this class
 */
PAG::MaterialEditingWindow::~MaterialEditingWindow() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

/**
 * @brief Method that creates (first time is called) and returns the only instance for this class
 * @return The only instance of this class
 */
PAG::MaterialEditingWindow * PAG::MaterialEditingWindow::getInstance() {
    if (!instance)
        instance = new MaterialEditingWindow();
    return instance;
}

/**
 * @brief Method that draws the GUI window for this class
 */
void PAG::MaterialEditingWindow::render() {
    ImGui::Begin("Material editor");

    // Material selector
    auto materialNames = Renderer::getInstance()->getMaterialNames();
    std::vector<const char*> cMatNames;
    cMatNames.push_back("[ Create new material ]");
    for (const auto& name : materialNames) cMatNames.push_back(name.c_str());

    ImGui::Combo("Material", &_selectedMaterial, cMatNames.data(), cMatNames.size());

    // Load input user data
    if (_selectedMaterial != _lastSelected) {
        if (_selectedMaterial == 0) {
            // New material initial data
            _package.materialId = -1;
            _package.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            _package.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
            _package.specular = glm::vec3(0.5f, 0.5f, 0.5f);
            _package.shininess = 32.0f;
            strcpy(_nameBuffer, "New material");

        } else {
            // Existing material data
            Material* mat = Renderer::getInstance()->getMaterial(_selectedMaterial - 1);
            if (mat) {
                _package.materialId = _selectedMaterial - 1;
                _package.diffuse = mat->getDiffuseColor();
                _package.ambient = mat->getAmbientColor();
                _package.specular = mat->getSpecularColor();
                _package.shininess = mat->getShininess();
                strcpy(_nameBuffer, mat->getName().c_str());
            }
        }
        _lastSelected = _selectedMaterial;
    }

    // Property editor for material

    ImGui::InputText("Name", _nameBuffer, 128);

    ImGui::ColorEdit3("Diffuse", &_package.diffuse.x);
    ImGui::ColorEdit3("Ambient", &_package.ambient.x);
    ImGui::ColorEdit3("Specular", &_package.specular.x);
    ImGui::InputFloat("Shininess", &_package.shininess);

    ImGui::Separator();

    // Confirmation buttons
    if (_selectedMaterial == 0) {
        if (ImGui::Button("Create material")) {
            warnListeners();
        }
    } else {
        if (ImGui::Button("Save changes")) {
            warnListeners();
        }
    }

    ImGui::End();
}

#include "ModelLoaderWindow.h"
#include "../utils/Logger.h"

PAG::ModelLoaderWindow* PAG::ModelLoaderWindow::instance = nullptr;

void PAG::ModelLoaderWindow::warnListeners() const {
    for (auto listener: _listeners) {
        listener->wakeUp(WindowType::ModelLoader,model.c_str());
    }
}

PAG::ModelLoaderWindow * PAG::ModelLoaderWindow::getInstance() {
    if (!instance) {
        instance = new ModelLoaderWindow();
    }
    return instance;
}

void PAG::ModelLoaderWindow::render() {
    static ImGui::FileBrowser fileDialog;

    fileDialog.SetTitle("Select a model");
    fileDialog.SetTypeFilters({".obj"}); ///< Only allow .obj models

    if(ImGui::Begin("Model loader window"))
    {
        if(ImGui::Button("Open file dialog"))
            fileDialog.Open();
    }
    ImGui::End();

    fileDialog.Display();

    if (fileDialog.HasSelected()) {
        model = fileDialog.GetSelected().string();

        const std::filesystem::path path(model);

        Logger::getInstance()->addMessage("Model selected: " + path.filename().string());
        warnListeners();
        fileDialog.ClearSelected();
    }
}

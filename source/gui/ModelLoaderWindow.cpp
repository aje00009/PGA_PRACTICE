#include "ModelLoaderWindow.h"
#include "../utils/Logger.h"

//Definition of the instance
PAG::ModelLoaderWindow* PAG::ModelLoaderWindow::instance = nullptr;

/**
 * @brief Method that wakes up all listener subscribed to this window events
 */
void PAG::ModelLoaderWindow::warnListeners() const {
    for (auto listener: _listeners) {
        listener->wakeUp(WindowType::ModelLoader,model.c_str());
    }
}

/**
 * @brief Method that creates (first time is called) and returns the only instance for this class
 * @return The only instance for this class
 */
PAG::ModelLoaderWindow * PAG::ModelLoaderWindow::getInstance() {
    if (!instance) {
        instance = new ModelLoaderWindow();
    }
    return instance;
}

/**
 * @brief Method that draws the GUI window for the user in order to be able to select and load a model
 */
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

#include "ModelTransformationWindow.h"

void PAG::ModelTransformationWindow::warnListeners() const
{
}

PAG::ModelTransformationWindow* PAG::ModelTransformationWindow::getInstance()
{
    if (!instance)
        instance = new PAG::ModelTransformationWindow();
    return instance;
}

void PAG::ModelTransformationWindow::render()
{

}



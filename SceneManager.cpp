#include "SceneManager.h"

namespace FW
{
    SceneManager::SceneManager(Application& app)
        : app(app)
    {
    }

    void SceneManager::popScene()
    {
        scenes.pop_back();
    }

    SceneManager::BaseScene& SceneManager::getCurrentScene()
    {
        return *scenes.back();
    }
}
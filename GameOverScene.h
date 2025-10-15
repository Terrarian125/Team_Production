#pragma once
#include "SceneBase.h"

class GameOverScene : public SceneBase {
public:
    SceneBase* Update() override;
    void Draw() override;
};

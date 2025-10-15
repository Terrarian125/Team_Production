#pragma once
#include "SceneBase.h"

class ClearScene : public SceneBase {
    int cl;
public:
    ClearScene();
    ~ClearScene();
    SceneBase* Update() override;
    void Draw() override;
};
